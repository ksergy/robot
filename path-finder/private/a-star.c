#include "types.h"
#include "bfs-dfs.h"
#include "path-finder.h"
#include "graph/graph.h"
#include "lib/set.h"
#include "lib/avl-tree.h"

#include <stdbool.h>

static inline
void traverse_path(graph_vertex_idx_t current,
                   avl_tree_t *came_from,
                   list_t *path) {
    avl_tree_node_t *node;

    *(graph_vertex_idx_t *)(list_prepend(path)->data) = current;

    node = avl_tree_get(came_from, current);
    current = *(graph_vertex_idx_t *)(node->data);

    for (node = avl_tree_get(came_from, current); node;
         current = *(graph_vertex_idx_t *)(node->data),
         node = avl_tree_get(came_from, current))
        *(graph_vertex_idx_t *)(list_prepend(path)->data) = current;
}

bool a_star(const path_finder_t *pf,
            graph_vertex_idx_t from, graph_vertex_idx_t to,
            list_t *path) {
    set_t closed_set;
    set_t open_set;
    avl_tree_t came_from;
    avl_tree_t g_score, f_score;
    avl_tree_node_t *current_node;
    graph_vertex_idx_t current;
    list_t *neighbours;
    list_element_t *neighbour_el;
    graph_vertex_idx_t neighbour;

    set_init(&closed_set);
    set_init(&open_set);

    avl_tree_init(&came_from, true, sizeof(graph_vertex_idx_t));
    avl_tree_init(&g_score, true, sizeof(node_cost_t));
    avl_tree_init(&f_score, true, sizeof(node_cost_t));

    /* data init */
    set_add_single(&open_set, from);
    *(node_cost_t *)(avl_tree_add(&g_score, from)->data) = 0;
    *(node_cost_t *)(avl_tree_add(&f_score, from)->data) =
        pf->cost_getter.a_star(pf->cost_getter_ctx,
                               pf,
                               from, to,
                               a_star_heuristic);

    while (set_size(&open_set)) {
        current_node = avl_tree_node_min(f_score.root);
        current = (graph_vertex_idx_t)(current_node->key);

        if (to == current) {
            traverse_path(current, &came_from, path);

            avl_tree_purge(&f_score);
            avl_tree_purge(&g_score);
            avl_tree_purge(&came_from);

            set_purge(&open_set);
            set_purge(&closed_set);

            return true;
        }   /* if (to == current) */

        set_remove(&open_set, current);

        set_add_single(&closed_set, current); /* FIXME not consistent heuristic */

        neighbours = (list_t *)vector_get(&pf->g->adjacency_list, current);

        for (neighbour_el = list_begin(neighbours); neighbour_el;
             neighbour_el = list_next(neighbours, neighbour_el)) {
            node_cost_t tentative_g_score, neighbour_g_score;
            avl_tree_node_t *g_score_node;
            bool inserted = false;

            neighbour = *(graph_vertex_idx_t *)(neighbour_el->data);

            if (set_count(&closed_set, neighbour))
                continue;   /* FIXME not consistent heuristic */

            g_score_node = avl_tree_get(&g_score, current);

            tentative_g_score = g_score_node
                                 ? *(node_cost_t *)(g_score_node->data)
                                 : NODE_COST_INF;

            tentative_g_score += pf->cost_getter.a_star(
                pf->cost_getter_ctx,
                pf,
                current, neighbour,
                a_star_distance
            );

            g_score_node = avl_tree_get(&g_score, neighbour);
            neighbour_g_score = g_score_node
                                 ? *(node_cost_t *)(g_score_node->data)
                                 : NODE_COST_INF;

            if (set_add(&open_set, neighbour) &&
                tentative_g_score >= neighbour_g_score)
                continue;

            /* this path is the best until now. record it */
            *(graph_vertex_idx_t *)(avl_tree_add_or_get(
                &came_from,
                neighbour, &inserted)->data) = current;

            if (g_score_node)
                *(node_cost_t *)(g_score_node->data) = tentative_g_score;
            else
                *(node_cost_t *)(avl_tree_add(&g_score, neighbour)->data) =
                    tentative_g_score;

            *(node_cost_t *)(avl_tree_add_or_get(
                &f_score,
                neighbour, &inserted)->data) =
                tentative_g_score + pf->cost_getter.a_star(
                pf->cost_getter_ctx,
                pf,
                neighbour, to,
                a_star_heuristic
            );
        }   /* for (neighbour_el = list_begin(neighbours); neighbour_el; */
    }   /* while (set_size(&open_set)) */

    avl_tree_purge(&f_score);
    avl_tree_purge(&g_score);
    avl_tree_purge(&came_from);

    set_purge(&open_set);
    set_purge(&closed_set);

    return false;
}