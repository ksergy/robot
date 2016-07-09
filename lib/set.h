#ifndef _SET_H_
# define _SET_H_

# include "avl-tree.h"

# include <stdbool.h>
# include <stddef.h>

typedef struct set {
    avl_tree_t tree;
} set_t;

typedef avl_tree_key_t set_key_t;

typedef struct set_iterator {
    void *it;
    unsigned int count;
    set_key_t k;
} set_iterator_t;

void set_init(set_t *s);
void set_purge(set_t *s);

size_t set_size(set_t *s);
unsigned int set_add(set_t *s, set_key_t k);
unsigned int set_count(set_t *s, set_key_t k);
unsigned int set_remove(set_t *s, set_key_t k);

set_iterator_t set_begin(set_t *s);
set_iterator_t set_end(set_t *s);
set_iterator_t set_next(set_t *s, avl_tree_node_t *el);
set_iterator_t set_prev(set_t *s, avl_tree_node_t *el);

set_key_t set_get_data(avl_tree_node_t *el);
unsigned int set_data_count(avl_tree_node_t *el);

#endif /* _SET_H_ */