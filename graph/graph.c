#include "graph.h"
#include "lib/set.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

/************************ private ************************/
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

static void
arrangement_random(size_t *v, size_t num,
                   uint64_t (*random_generator)(void *ptr),
                   void *ptr) {
    set_t used;
    size_t r;
    set_init(&used);

    while (used.tree.count != num) {
        r = random_generator(ptr) % num;

        while (set_add(&used, r) > 1)
            r = (r + 1) % num;

        v[used.tree.count - 1] = r;
    }

    set_purge(&used);
}

static
void add_directed_edge_to_adjacency_list(graph_t *g,
                                         graph_vertex_idx_t from,
                                         graph_vertex_idx_t to) {
    list_t *adj_list_from;
    list_element_t *adj_from_to;

    adj_list_from = (list_t *)vector_get(&g->adjacency_list, from);

    for (adj_from_to = list_begin(adj_list_from);
         adj_from_to && (*(graph_vertex_idx_t *)adj_from_to->data < to);
         adj_from_to = list_next(adj_list_from, adj_from_to));

    adj_from_to = list_prev(adj_list_from, adj_from_to);

    if (!adj_from_to)
        adj_from_to = list_prepend(adj_list_from);
    else
        adj_from_to = list_add_after(adj_list_from, adj_from_to);

    *(graph_vertex_idx_t *)adj_from_to->data = to;
}

static
void remove_directed_edge_from_adjacency_list(graph_t *g,
                                              graph_vertex_idx_t from,
                                              graph_vertex_idx_t to) {
    list_t *adj_from;
    list_element_t *adj_from_to;

    adj_from = (list_t *)vector_get(&g->adjacency_list, from);
    for (adj_from_to = list_begin(adj_from);
         adj_from_to && (*(graph_vertex_idx_t *)adj_from_to->data != to);
         adj_from_to = list_next(adj_from, adj_from_to));

    list_remove_and_advance(adj_from, adj_from_to);
}

static inline
void dfs_bfs(const graph_t *g, graph_vertex_idx_t from,
             graph_vertex_runner_t runner, void *priv,
             list_element_t *(*list_op)(list_t *)) {
    list_t series;
    list_element_t *next_element_in_series;
    set_t used;
    list_t *neighbours;
    list_element_t *neighbour;

    set_init(&used);
    list_init(&series, true, sizeof(graph_vertex_idx_t));

    next_element_in_series = list_append(&series);
    *(graph_vertex_idx_t *)next_element_in_series->data = from;

    do {
        next_element_in_series = (*list_op)(&series);
        from = *(graph_vertex_idx_t *)next_element_in_series->data;
        list_remove_and_advance(&series, next_element_in_series);

        if (set_add(&used, from) > 1)
            continue;

        if (runner && !runner(g, from, priv))
            break;

        neighbours = (list_t *)vector_get((vector_t *)&g->adjacency_list, from);
        for (neighbour = list_begin(neighbours);
             neighbour; neighbour = list_next(neighbours, neighbour))
            *(graph_vertex_idx_t *)list_append(&series)->data =
                *(graph_vertex_idx_t *)neighbour->data;
    } while(list_size(&series));

    list_purge(&series);
    set_purge(&used);

    return;
}

/************************ API ************************/
void graph_init(graph_t *g,
                graph_vertex_idx_t vertices_number,
                void **vertices_data,
                bool directed) {
    size_t idx;

    assert(g);

    g->directed = directed;
    g->vertices_number = vertices_number;
    g->edges_number = 0;

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
    avl_tree_node_t *atn_edge;

    assert(gr);

    switch (gr->directed) {
        case false:
            if (from > to) {
                graph_vertex_idx_t t;
                t = from;
                from = to;
                to = t;
            }

        case true:
            edge_idx = graph_edge_idx(gr->vertices_number, from, to);
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
                                       graph_vertex_idx_t from,
                                       graph_vertex_idx_t to,
                                       void *edge_data) {
    bool inserted = false;
    graph_edge_idx_t e;
    avl_tree_node_t *atn_edge;

    assert(g);

    if (!g->directed && from > to) {
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

    switch (g->directed) {
        case false:
            add_directed_edge_to_adjacency_list(g, to, from);

        case true:
            add_directed_edge_to_adjacency_list(g, from, to);
            break;
    }

    ++g->edges_number;

    return e;
}

graph_edge_found_t graph_remove_edge(graph_t *g,
                                     graph_vertex_idx_t _from,
                                     graph_vertex_idx_t _to) {
    graph_edge_idx_t e;
    void *edge_data;
    graph_vertex_idx_t from = _from, to = _to;
    bool removed = false;

    assert(g);

    if (!g->directed && from > to) {
        graph_vertex_idx_t t = from;
        from = to;
        to = t;
    }

    e = graph_edge_idx(g->vertices_number, from, to);
    edge_data = avl_tree_remove_get_data_signal(&g->edges, e, &removed);

    if (!removed)
        return (graph_edge_found_t){ .found = false, .data = NULL };

    switch (g->directed) {
        case false:
            remove_directed_edge_from_adjacency_list(g, to, from);

        case true:
            remove_directed_edge_from_adjacency_list(g, from, to);
            break;
    }

    --g->edges_number;

    return (graph_edge_found_t){ .found = true, .data = edge_data };
}

graph_edge_found_t graph_remove_edge_idx(graph_t *g, graph_edge_idx_t idx) {
    void *edge_data;
    graph_vertex_idx_t from, to;
    bool removed = false;

    assert(g);

    if (!g->directed)
        undirect_edge(g, &idx);

    graph_edge_from_idx(g->vertices_number, idx, &from, &to);

    edge_data = avl_tree_remove_get_data_signal(&g->edges, idx, &removed);

    if (!removed)
        return (graph_edge_found_t){ .found = false, .data = NULL };

    switch (g->directed) {
        case false:
            remove_directed_edge_from_adjacency_list(g, to, from);

        case true:
            remove_directed_edge_from_adjacency_list(g, from, to);
            break;
    }

    --g->edges_number;

    return (graph_edge_found_t){ .found = true, .data = edge_data };
}

graph_edge_found_t graph_test_edge(const graph_t *g,
                                   graph_vertex_idx_t from,
                                   graph_vertex_idx_t to) {
    avl_tree_node_t *atn_edge;
    graph_edge_idx_t e;

    assert(g);

    if (!g->directed && from > to) {
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

void graph_bfs(const graph_t *g, graph_vertex_idx_t from,
               graph_vertex_runner_t runner, void *priv) {
    assert(g && from < g->vertices_number);

    dfs_bfs(g, from, runner, priv, list_begin);
}

void graph_dfs(const graph_t *g,
               graph_vertex_idx_t from,
               graph_vertex_runner_t runner, void *priv) {
    assert(g && from < g->vertices_number);

    dfs_bfs(g, from, runner, priv, list_end);
}

list_t *graph_random_path(const graph_t *g,
                          graph_vertex_idx_t from, graph_vertex_idx_t to,
                          uint64_t (*random_generator)(void *ptr), void *ptr) {
    /* TODO BFS + mix */
    list_t queue;
    list_element_t *queue_element;
    graph_vertex_idx_t v;
    set_t used;
    list_t *path;
    list_t *neighbours;
    list_element_t *neighbour;
    vector_t neighbours_rearranged;
    void **neighbour_rearranged, *neighbour_rearranged_idx;
    vector_t arrangement;

    assert(g && from < g->vertices_number && to < g->vertices_number);

    path = malloc(sizeof(list_t));
    list_init(path, true, sizeof(graph_vertex_idx_t));

    set_init(&used);
    list_init(&queue, true, sizeof(graph_vertex_idx_t));

    *(graph_vertex_idx_t *)list_append(&queue)->data = from;

    /* DFS */
    do {
        queue_element = list_end(&queue);
        v = *(graph_vertex_idx_t *)queue_element->data;
        list_remove_and_advance(&queue, queue_element);

        if (set_add(&used, from) > 1)
            continue;

        *(graph_vertex_idx_t *)list_append(path)->data = v;

        /* rearrange neighbours in random order */
        neighbours = (list_t *)vector_get((vector_t *)&g->adjacency_list, v);
        vector_init(&neighbours_rearranged,
                    sizeof(graph_vertex_idx_t *), list_size(neighbours));

        for (neighbour = list_begin(neighbours),
             neighbour_rearranged = (void **)neighbours_rearranged.data.data;
             neighbour;
             neighbour = list_next(neighbours, neighbour),
             ++neighbour_rearranged)
            *neighbour_rearranged = neighbour->data;

        vector_init(&arrangement, sizeof(size_t),
                    list_size(neighbours));

        arrangement_random((size_t *)arrangement.data.data, list_size(neighbours),
                           random_generator, ptr);

        for (neighbour_rearranged_idx = vector_begin(&arrangement);
             neighbour_rearranged_idx != vector_end(&arrangement);
             neighbour_rearranged_idx = vector_next(
                 &arrangement,
                 neighbour_rearranged_idx))
             *(graph_vertex_idx_t *)list_append(&queue)->data =
                 *(graph_vertex_idx_t *)(*(void **)vector_get(
                     &neighbours_rearranged,
                     *(size_t *)neighbour_rearranged_idx));

        vector_deinit(&arrangement);

        vector_deinit(&neighbours_rearranged);
    } while(list_size(&queue) && v != to);

    list_purge(&queue);
    set_purge(&used);

    return path;
}

void graph_untie_path(const graph_t *g, list_t *path) {
    vector_t used;
    list_element_t *path_el, **prev_path_el_ptr, *remove_el;
    graph_vertex_idx_t v;

    assert(g && path);

    if (list_size(path) < 2)
        return;

    vector_init(&used, sizeof(list_element_t *), g->vertices_number);
    memset(used.data.data, 0, used.data.user_size);

    for (path_el = list_begin(path);
         path_el;) {
        v = *(graph_vertex_idx_t *)path_el->data;
        prev_path_el_ptr = (list_element_t **)vector_get(&used, v);

        if (!(*prev_path_el_ptr)) {
            *prev_path_el_ptr = path_el;
            path_el = list_next(path, path_el);
            continue;
        }

        for (remove_el = list_next(path, *prev_path_el_ptr);
             remove_el && remove_el != path_el;)
            remove_el = list_remove_and_advance(path, remove_el);

        path_el = list_remove_and_advance(path, remove_el);
    }

    vector_deinit(&used);
}
