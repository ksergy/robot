#include "graph.h"

#include <string.h>
#include <assert.h>

static void
purge_edges(graph_t *g, graph_edge_purger_t gep) {
    graph_vertex_idx_t from, to;
    list_t *adj;
    list_element_t *adj_el;
    void *edge_data;

    for (from = 0, adj = g->adjacency_list.data.data;
         from < g->vertices_number; ++from, ++adj)
        for (adj_el = list_begin(adj);
             adj_el; adj_el = list_next(adj, adj_el)) {
            to = *(graph_vertex_idx_t *)adj_el->data;

            edge_data = avl_tree_remove_get_data(
                &g->edges,
                graph_edge_idx(g->vertices_number, from, to)
            );

            if (gep)
                gep(g, from, to, edge_data);
        }   /* for (adj_el = list_begin(adj); */
}

static void
purge_vertices(graph_t *g, graph_vertex_purger_t gvp) {
    graph_vertex_idx_t v;
    void **data_ptr;

    for (v = 0, data_ptr = g->vertices_data.data.data;
         v < g->vertices_number; ++v, ++data_ptr)
         if (gvp)
             gvp(g, v, *data_ptr);
}

static inline void
undirect_edge(const graph_t const *g, graph_edge_idx_t *edge) {
    graph_vertex_idx_t from, to, tmp;

    graph_edge_from_idx(g->vertices_number, *edge, &from, &to);

    if (from > to) {
        tmp = from;
        from = to;
        to = tmp;
        *edge = graph_edge_idx(g->vertices_number, from, to);
    }
}

void graph_init(graph_t *g,
                graph_vertex_idx_t vertices_number,
                void **vertices_data,
                bool directed) {
    size_t idx;

    assert(g);

    g->directed = directed;
    g->vertices_number = vertices_number;

    vector_init(&g->vertices_data, sizeof(void *), vertices_number);
    memcpy(g->vertices_data.data.data, vertices_data,
           vertices_number * sizeof(void *));

    vector_init(&g->adjacency_list, sizeof(list_t), vertices_number);
    for (idx = 0; idx < vertices_number; ++idx)
        list_init((list_t *)vector_get(&g->adjacency_list, idx),
                  true, sizeof(graph_vertex_idx_t));

    avl_tree_init(&g->edges, false, 0);
}

void graph_deinit(graph_t *g,
                  graph_edge_purger_t gep,
                  graph_vertex_purger_t gvp) {
    size_t idx;

    assert(g);

    purge_edges(g, gep);
    purge_vertices(g, gvp);

    vector_deinit(&g->vertices_data);

    for (idx = 0; idx < g->vertices_number; ++idx)
        list_purge((list_t *)vector_get(&g->adjacency_list, idx));

    avl_tree_purge(&g->edges);
}

void *graph_vertex_data(const graph_t const *gr, graph_vertex_idx_t v) {
    assert(gr);

    return v < gr->vertices_number
            ? vector_get((vector_t *)&gr->vertices_data, v)
            : NULL;
}

void *graph_edge_data(const graph_t const *gr,
                      graph_vertex_idx_t from, graph_vertex_idx_t to) {
    graph_edge_idx_t edge_idx;
    graph_vertex_idx_t min, max;
    avl_tree_node_t *atn_edge;

    assert(gr);

    switch (gr->directed) {
        case true:
            edge_idx = graph_edge_idx(gr->vertices_number, from, to);
            break;
        case false:
            if (from < to) {
                min = from;
                max = to;
            }
            else {
                max = to;
                min = from;
            }

            edge_idx = graph_edge_idx(gr->vertices_number, min, max);
            break;
    }

    atn_edge = avl_tree_get((avl_tree_t *)&gr->edges, edge_idx);

    return atn_edge ? atn_edge->data : NULL;
}

void *graph_edge_idx_data(const graph_t *gr, graph_edge_idx_t edge) {
    avl_tree_node_t *atn_edge;

    assert(gr);

    if (!gr->directed)
        undirect_edge(gr, &edge);

    atn_edge = avl_tree_get((avl_tree_t *)&gr->edges, edge);

    return atn_edge ? atn_edge->data : NULL;
}

graph_edge_idx_t graph_add_update_edge(graph_t *g,
                                       graph_vertex_idx_t from, graph_vertex_idx_t to,
                                       void *edge_data) {
    bool inserted = false;
    graph_edge_idx_t e;
    avl_tree_node_t *atn_edge;
    list_t *adj_list_from;
    list_element_t *adj_from_to;

    assert(g);

    if (!g->directed)
        if (from > to) {
            graph_vertex_idx_t t = from;
            from = to;
            to = t;
        }

    e = graph_edge_idx(g->vertices_number, from, to);
    atn_edge = avl_tree_add_or_get(&g->edges, e, &inserted);

    if (!inserted) {
        atn_edge->data = edge_data;
        return e;
    }

    atn_edge->data = edge_data;

    adj_list_from = (list_t *)vector_get(&g->adjacency_list, from);

    for (adj_from_to = list_begin(adj_list_from);
         adj_from_to && (*(graph_vertex_idx_t *)adj_from_to->data < to);
         adj_from_to = list_next(adj_list_from, adj_from_to));

    adj_from_to = list_prev(adj_list_from, adj_from_to);

    if (!adj_from_to)
        adj_from_to = list_prepend(adj_list_from);
    else
        adj_from_to = list_add_after(adj_list_from, adj_from_to);

    ++g->edges_number;

    *(graph_vertex_idx_t *)adj_from_to->data = to;
    return e;
}

graph_edge_found_t graph_remove_edge(graph_t *g,
                                     graph_vertex_idx_t _from,
                                     graph_vertex_idx_t _to) {
    graph_edge_idx_t e;
    void *edge_data;
    graph_vertex_idx_t from = _from, to = _to;
    bool removed = false;
    list_t *adj_from;
    list_element_t *adj_from_to;

    assert(g);

    if (!g->directed)
        if (from > to) {
            graph_vertex_idx_t t = from;
            from = to;
            to = t;
        }

    e = graph_edge_idx(g->vertices_number, from, to);
    edge_data = avl_tree_remove_get_data_signal(&g->edges, e, &removed);

    if (!removed)
        return (graph_edge_found_t){ .found = false, .data = NULL };

    adj_from = (list_t *)vector_get(&g->adjacency_list, from);
    for (adj_from_to = list_begin(adj_from);
         adj_from_to && (*(graph_vertex_idx_t *)adj_from_to->data != to);
         adj_from_to = list_next(adj_from, adj_from_to));

    list_remove_and_advance(adj_from, adj_from_to);

    --g->edges_number;

    return (graph_edge_found_t){ .found = true, .data = edge_data };
}

graph_edge_found_t graph_remove_edge_idx(graph_t *g, graph_edge_idx_t idx) {
    void *edge_data;
    graph_vertex_idx_t from, to;
    bool removed = false;
    list_t *adj_from;
    list_element_t *adj_from_to;

    assert(g);

    if (!g->directed)
        undirect_edge(g, &idx);

    graph_edge_from_idx(g->vertices_number, idx, &from, &to);

    edge_data = avl_tree_remove_get_data_signal(&g->edges, idx, &removed);

    if (!removed)
        return (graph_edge_found_t){ .found = false, .data = NULL };

    adj_from = (list_t *)vector_get(&g->adjacency_list, from);
    for (adj_from_to = list_begin(adj_from);
         adj_from_to && (*(graph_vertex_idx_t *)adj_from_to->data != to);
    adj_from_to = list_next(adj_from, adj_from_to));

    list_remove_and_advance(adj_from, adj_from_to);

    --g->edges_number;

    return (graph_edge_found_t){ .found = true, .data = edge_data };
}

graph_edge_found_t graph_test_edge(const graph_t *g,
                                   graph_vertex_idx_t from,
                                   graph_vertex_idx_t to) {
    avl_tree_node_t *atn_edge;
    graph_edge_idx_t e;

    assert(g);

    if (!g->directed)
        if (from > to) {
            graph_vertex_idx_t t = from;
            from = to;
            to = t;
        }

    e = graph_edge_idx(g->vertices_number, from, to);

    atn_edge = avl_tree_get((avl_tree_t *)&g->edges, e);

    return atn_edge
            ? (graph_edge_found_t){ .found = true, .data = atn_edge->data }
            : (graph_edge_found_t){ .found = false, .data = NULL };
}

graph_edge_found_t graph_test_edge_idx(const graph_t *g,
                                       graph_edge_idx_t idx) {
    avl_tree_node_t *atn_edge;

    assert(g);

    if (!g->directed)
        undirect_edge(g, &idx);

    atn_edge = avl_tree_get((avl_tree_t *)&g->edges, idx);

    return atn_edge
            ? (graph_edge_found_t){ .found = true, .data = atn_edge->data }
            : (graph_edge_found_t){ .found = false, .data = NULL };
}
