#include "set.h"
#include "avl-tree.h"

#include <stdbool.h>
#include <assert.h>

void set_init(set_t *s) {
    assert(s);

    avl_tree_init(&s->tree, true, sizeof(unsigned int));
}

void set_purge(set_t *s) {
    assert(s);

    avl_tree_purge(&s->tree);
}

size_t set_size(set_t *s) {
    assert(s);

    return s->tree.count;
}

unsigned int set_add(set_t *s, set_key_t k) {
    bool inserted = false;
    avl_tree_node_t *atn;
    unsigned int *count;
    unsigned int prev_value;

    assert(s);

    atn = avl_tree_add_or_get(&s->tree, k, &inserted);

    count = atn->data;

    prev_value = (!inserted) * (*count);
    *count = prev_value + 1;

    return prev_value;
}

unsigned int set_count(set_t *s, set_key_t k) {
    avl_tree_node_t *atn;
    unsigned int count;

    assert(s);

    atn = avl_tree_get(&s->tree, k);

    count = atn ? *(unsigned int *)atn->data : 0;

    return count;
}

unsigned int set_remove(set_t *s, set_key_t k) {
    avl_tree_node_t *atn;
    unsigned int *count;
    unsigned int next_value;

    assert(s);

    atn = avl_tree_get(&s->tree, k);

    count = atn->data;

    next_value = *count;
    --next_value;

    switch (next_value) {
        case 0:
            avl_tree_remove_get_data(&s->tree, k);
            break;
        default:
            *count = next_value;
            break;
    }

    return next_value;
}

void *set_begin(set_t *s) {
    assert(s);

    return avl_tree_node_min(s->tree.root);
}

void *set_end(set_t *s) {
    assert(s);

    return avl_tree_node_max(s->tree.root);
}

void *set_next(set_t *s, void *el) {
    assert(s);

    return avl_tree_node_next(el);
}

void *set_prev(set_t *s, void *el) {
    assert(s);

    return avl_tree_node_prev(el);
}

set_key_t set_get_data(void *el) {
    assert(el);
    return ((avl_tree_node_t *)el)->key;
}

unsigned int set_data_count(void *el) {
    return el ? *(unsigned int *)((avl_tree_node_t *)el)->data : 0;
}
