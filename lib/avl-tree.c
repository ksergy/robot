#include "avl-tree.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

typedef avl_tree_node_t *(*node_allocator)(avl_tree_t *t, avl_tree_key_t k);
typedef void (*node_deallocator)(avl_tree_node_t *n);

static
avl_tree_node_t *node_simple_allocator(avl_tree_t *t, avl_tree_key_t k) {
    avl_tree_node_t *n = malloc(sizeof(avl_tree_node_t));
    n->height = 1;
    n->data = NULL;
    n->host = t;
    n->key = k;
    n->left = n->right = n->parent = NULL;
}

static
avl_tree_node_t *node_inplace_allocator(avl_tree_t *t, avl_tree_key_t k) {
    avl_tree_node_t *n = malloc(sizeof(avl_tree_node_t) + t->node_data_size);
    n->height = 1;
    n->data = n + 1;
    n->host = t;
    n->key = k;
    n->left = n->right = n->parent = NULL;
}

static const struct {
    node_allocator      allocator;
    node_deallocator    deallocator;
} node_operators[2] = {
    [false] = {
        .allocator      = node_simple_allocator,
        .deallocator    = free
    },
    [true] = {
        .allocator      = node_inplace_allocator,
        .deallocator    = free
    }
};

static inline
int node_height(const avl_tree_node_t *n) {
    return n ? n->height : 0;
}

static inline
int node_balance_factor(const avl_tree_node_t *n) {
    return node_height(n->left) - node_height(n->right);
}

static inline
void node_fix_height(avl_tree_node_t *n) {
    unsigned char hl = node_height(n->left);
    unsigned char hr = node_height(n->right);

    n->height = (hl > hr ? hl : hr) + 1;
}

static inline
avl_tree_node_t *node_rotate_left(avl_tree_node_t *a) {
    avl_tree_node_t *L = a->left,
                    *b = a->right,
                    *C = b->left,
                    *R = b->right;

    b->parent = a->parent;
    a->parent = b;

    if (C)
        C->parent = a;

    b->left = a;
    a->right = C;

    node_fix_height(a);
    node_fix_height(b);

    return b;
}

static inline
avl_tree_node_t *node_rotate_right(avl_tree_node_t *a) {
    avl_tree_node_t *b = a->left,
                    *L = b->left,
                    *C = b->right,
                    *R = a->right;

    b->parent = a->parent;
    a->parent = b;

    if (C)
        C->parent = a;

    b->right = a;
    a->left = C;

    node_fix_height(a);
    node_fix_height(b);

    return b;
}

static inline
avl_tree_node_t *node_balance(avl_tree_node_t *n) {
    node_fix_height(n);

    switch (node_balance_factor(n)) {
        case 2:
            if (node_balance_factor(n->left) < 0)
                n->left = node_rotate_left(n->left);

            n = node_rotate_right(n);
            break;

        case -2:
            if (node_balance_factor(n->right) > 0)
                n->right = node_rotate_right(n->right);

            n = node_rotate_left(n);
            break;
    }

    return n;
}

static inline
avl_tree_node_t *node_insert(avl_tree_node_t *n,
                             avl_tree_t *t,
                             avl_tree_key_t k) {
    if (!n)
        return node_operators[t->inplace].allocator(t, k);

    if (k < n->key)
        n->left = node_insert(n->left, t, k);
    else
        n->right = node_insert(n->right, t, k);

    return node_balance(n);
}

static inline
avl_tree_node_t *node_leftmost(avl_tree_node_t *n) {
    return n->left ? node_leftmost(n->left) : n;
}

static inline
avl_tree_node_t *node_rightmost(avl_tree_node_t *n) {
    return n->right ? node_rightmost(n->right) : n;
}

static inline
avl_tree_node_t *node_remove_min(avl_tree_node_t *n) {
    if (!n->left)
        return n->right;

    n->left = node_remove_min(n->left);

    return node_balance(n);
}

static inline
bool node_is_left(const avl_tree_node_t *parent,
                  const avl_tree_node_t *left) {
    return !parent || (parent->left == left);
}

static inline
bool node_is_right(const avl_tree_node_t *parent,
                   const avl_tree_node_t *right) {
    return !parent || (parent->right == right);
}

static inline
avl_tree_node_t *node_remove(avl_tree_node_t *n, avl_tree_key_t k) {
    avl_tree_node_t copy, *min;

    if (!n)
        return NULL;

    if (k < n->key)
        n->left = node_remove(n->left, k);
    else if (k > n->key)
        n->right = node_remove(n->right, k);
    else {
        copy = *n;

        node_operators[n->host->inplace].deallocator(n);

        if (!copy.right) {
            copy.left->parent = copy.parent;
            return copy.left;
        }

        min = node_leftmost(copy.right);

        min->right = node_remove_min(copy.right);
        min->left = copy.left;

        min->parent = copy.parent;

        if (min->left)
            min->left->parent = min;

        min->right->parent = min;

        return node_balance(min);
    }

    return node_balance(n);
}

/**************** API ****************/
void avl_tree_init(avl_tree_t *tree, bool inplace, size_t node_data_size) {
    assert(tree);

    tree->inplace = inplace;
    tree->node_data_size = node_data_size;
    tree->count = 0;
}

avl_tree_node_t *avl_tree_get(avl_tree_t *t, avl_tree_key_t k) {
    avl_tree_node_t *n;

    assert(t);

    n = t->root;

    while (n && n->key != k)
        n = k > n->key ? n->right : n->left;

    return n;
}

avl_tree_node_t *avl_tree_add(avl_tree_t *t, avl_tree_key_t k) {
    avl_tree_node_t *n;
    assert(t);

    /* TODO */
}

