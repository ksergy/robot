#include "set.h"
#include "avl-tree.h"

#include <stdbool.h>
#include <assert.h>

static
set_iterator_t fill_iterator(avl_tree_node_t *atn) {
    set_iterator_t it;

    if (atn) {
        it.it = atn;
        it.k = atn->key;
        it.count = *(unsigned int *)atn->data;
    } else {
        it.it = NULL;
        it.count = 0;
        it.k = 0;
    }

    return it;
}

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

set_iterator_t set_begin(set_t *s) {
    avl_tree_node_t *atn;

    assert(s);

    atn = avl_tree_node_min(s->tree.root);

    return fill_iterator(atn);
}

set_iterator_t set_end(set_t *s) {
    avl_tree_node_t *atn;

    assert(s);

    atn = avl_tree_node_max(s->tree.root);

    return fill_iterator(atn);
}

set_iterator_t set_next(set_t *s, avl_tree_node_t *el) {
    avl_tree_node_t *atn;

    assert(s);

    atn = avl_tree_node_next(el);

    return fill_iterator(atn);
}

set_iterator_t set_prev(set_t *s, avl_tree_node_t *el) {
    avl_tree_node_t *atn;

    assert(s);

    atn = avl_tree_node_prev(el);


    return fill_iterator(atn);
}

unsigned int set_data_count(avl_tree_node_t *el) {
    return el ? *(unsigned int *)el->data : 0;
}

set_key_t set_get_data(avl_tree_node_t *el) {
    return el ? el->key : 0;
}


