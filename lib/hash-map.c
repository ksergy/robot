#include "hash-map.h"
#include "avl-tree.h"

#include <assert.h>
#include <stdbool.h>

void hash_map_init(hash_map_t *hm,
                   hash_function_t hasher,
                   hash_update_function_t hash_updater) {
    assert(hm);

    avl_tree_init(&hm->tree, true, sizeof(hash_map_node_t));

    hm->hasher = hasher;
    hm->hash_updater = hash_updater;
}

void hash_map_purge(hash_map_t *hm) {
    assert(hm);

    avl_tree_purge(&hm->tree);

    hm->hash_updater = NULL;
    hm->hasher = NULL;
}

size_t hash_map_size(hash_map_t *hm) {
    assert(hm);

    return hm->tree.count;
}

hash_map_node_t *hash_map_add(hash_map_t *hm, hash_t h) {
    avl_tree_node_t *atn;
    hash_map_node_t *hmn;

    assert(hm);

    atn = avl_tree_add(&hm->tree, h);

    hmn = atn->data;
    hmn->host = hm;
    hmn->hash = h;
    hmn->tree_node = atn;

    list_init(&hmn->data_list, true, sizeof(hash_map_node_data_t));

    return hmn;
}

hash_map_node_t *hash_map_add_or_get(hash_map_t *hm, hash_t h) {
    avl_tree_node_t *atn;
    hash_map_node_t *hmn;
    bool inserted = false;

    assert(hm);

    atn = avl_tree_add_or_get(&hm->tree, h, &inserted);

    hmn = atn->data;

    if (inserted) {
        hmn->host = hm;
        hmn->hash = h;
        hmn->tree_node = atn;

        list_init(&hmn->data_list, true, sizeof(hash_map_node_data_t));
    }

    return hmn;
}

hash_map_node_t *hash_map_get(hash_map_t *hm, hash_t h) {
    avl_tree_node_t *atn;
    hash_map_node_t *hmn;

    assert(hm);

    atn = avl_tree_get(&hm->tree, h);

    hmn = atn ? atn->data : NULL;

    return hmn;
}

void hash_map_remove(hash_map_t *hm, hash_t h) {
    avl_tree_node_t *atn;
    hash_map_node_t *hmn;

    assert(hm);

    hmn = avl_tree_remove_get_data(&hm->tree, h);

    list_purge(&hmn->data_list);
}

hash_map_node_t *hash_map_next(hash_map_t *hm, hash_map_node_t *hmn) {
    avl_tree_node_t *atn_next;
    hash_map_node_t *next;

    assert(hm);

    if (!hmn)
        return NULL;

    atn_next = avl_tree_node_next(hmn->tree_node);
    next = atn_next ? atn_next->data : NULL;

    return next;
}

hash_map_node_t *hash_map_prev(hash_map_t *hm, hash_map_node_t *hmn) {
    avl_tree_node_t *atn_prev;
    hash_map_node_t *prev;

    assert(hm);

    if (!hmn)
        return NULL;

    atn_prev = avl_tree_node_prev(hmn->tree_node);
    prev = atn_prev ? atn_prev->data : NULL;

    return prev;
}

hash_map_node_t *hash_map_begin(hash_map_t *hm) {
    avl_tree_node_t *atn_begin;
    hash_map_node_t *begin;

    assert(hm);

    atn_begin = avl_tree_node_min(hm->tree.root);

    begin = atn_begin ? atn_begin->data : NULL;

    return begin;
}

hash_map_node_t *hash_map_end(hash_map_t *hm) {
    avl_tree_node_t *atn_end;
    hash_map_node_t *end;

    assert(hm);

    atn_end = avl_tree_node_max(hm->tree.root);

    end = atn_end ? atn_end->data : NULL;

    return end;
}
