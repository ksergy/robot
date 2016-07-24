#include "graph.h"

#include <assert.h>

void graph_init(graph_t *g, graph_vertex_idx_t vertices_number, bool directed) {
    assert(g);

    g->vertices_number = vertices_number;
    g->edges_number = 0;
    g->directed = directed;

    avl_tree_init(&g->adjacency_map, true, sizeof(graph_edge_count_t));
}

void graph_deinit(graph_t *g) {
    assert(g);

    avl_tree_purge(&g->adjacency_map);
}

graph_edge_idx_t graph_add_edge(graph_t *g,
                                graph_vertex_idx_t from,
                                graph_vertex_idx_t to) {
    avl_tree_node_t *atn;
    bool inserted = false;
    graph_edge_idx_t e;

    assert(g);
    assert(g->vertices_number > from && g->vertices_number > to);

    switch (g->directed) {
        case false:
            e = graph_edge_idx(g->vertices_number, to, from);
            atn = avl_tree_add_or_get(&g->adjacency_map, e, &inserted);

            switch (inserted) {
                case true:
                    *(graph_edge_count_t *)(atn->data) = 0;

                default:
                    (*(graph_edge_count_t *)(atn->data)) ++;
            }

            ++ g->edges_number;

        default:
            e = graph_edge_idx(g->vertices_number, from, to);
            atn = avl_tree_add_or_get(&g->adjacency_map, e, &inserted);

            switch (inserted) {
                case true:
                    *(graph_edge_count_t *)(atn->data) = 0;

                default:
                    (*(graph_edge_count_t *)(atn->data)) ++;
            }

            ++ g->edges_number;
    }

    return e;
}

graph_edge_count_t graph_remove_edge(graph_t *g,
                                     graph_vertex_idx_t from, graph_vertex_idx_t to) {
    avl_tree_node_t *atn;
    graph_edge_idx_t e;
    graph_edge_count_t c = 0;

    assert(g);
    assert(g->vertices_number > from && g->vertices_number > to);

    switch (g->directed) {
        case false:
            e = graph_edge_idx(g->vertices_number, to, from);
            atn = avl_tree_get(&g->adjacency_map, e);
            if (atn && *(graph_edge_count_t *)(atn->data)) {
                c = *(graph_edge_count_t *)(atn->data) - 1;
                if (! (-- *(graph_edge_count_t *)(atn->data)))
                    avl_tree_remove(&g->adjacency_map, e);

                -- g->edges_number;
            }
        default:
            e = graph_edge_idx(g->vertices_number, from, to);
            atn = avl_tree_get(&g->adjacency_map, e);
            if (atn && *(graph_edge_count_t *)(atn->data)) {
                c = *(graph_edge_count_t *)(atn->data) - 1;
                if (! (-- *(graph_edge_count_t *)(atn->data)))
                    avl_tree_remove(&g->adjacency_map, e);

                -- g->edges_number;
            }
    }

    return c;
}

graph_edge_count_t graph_remove_edge_idx(graph_t *g, graph_edge_idx_t idx) {
    avl_tree_node_t *atn;
    graph_edge_count_t c = 0;

    assert(g);

    switch (g->directed) {
        case false:
            atn = avl_tree_get(&g->adjacency_map, idx);
            if (atn && *(graph_edge_count_t *)(atn->data)) {
                c = *(graph_edge_count_t *)(atn->data) - 1;
                if (! (-- *(graph_edge_count_t *)(atn->data)))
                    avl_tree_remove(&g->adjacency_map, idx);

                -- g->edges_number;
            }
        default:
            idx = graph_edge_inverse_idx(g->vertices_number, idx);
            atn = avl_tree_get(&g->adjacency_map, idx);
            if (atn && *(graph_edge_count_t *)(atn->data)) {
                c = *(graph_edge_count_t *)(atn->data) - 1;
                if (! (-- *(graph_edge_count_t *)(atn->data)))
                    avl_tree_remove(&g->adjacency_map, idx);

                -- g->edges_number;
            }
    }

    return c;
}

graph_edge_count_t graph_test_edge(graph_t *g,
                                   graph_vertex_idx_t from, graph_vertex_idx_t to) {
    avl_tree_node_t *atn;

    assert(g);
    assert(g->vertices_number > from && g->vertices_number > to);

    atn = avl_tree_get(
        &g->adjacency_map,
        graph_edge_idx(g->vertices_number, from, to)
    );

    return atn ? *(graph_edge_count_t *)(atn->data) : 0;
}

graph_edge_count_t graph_test_edge_idx(graph_t *g, graph_edge_idx_t idx) {
    avl_tree_node_t *atn;

    assert(g);

    atn = avl_tree_get(
        &g->adjacency_map,
        idx
    );

    return atn ? *(graph_edge_count_t *)(atn->data) : 0;
}
