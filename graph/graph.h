#ifndef _GRAPH_H_
# define _GRAPH_H_

# include "lib/containers.h"
# include "lib/avl-tree.h"

# include <stdbool.h>

# define GRAPH_DIRECTED true

struct graph;
typedef struct graph graph_t;

struct graph_edge_found;
typedef struct graph_edge_found graph_edge_found_t;

typedef uint32_t graph_vertex_idx_t;
typedef uint64_t graph_edge_idx_t;
typedef uint64_t graph_edge_count_t;

typedef void (*graph_edge_purger_t)(const graph_t const *g,
                                    graph_vertex_idx_t from,
                                    graph_vertex_idx_t to,
                                    void *data);
typedef void (*graph_vertex_purger_t)(const graph_t const *g,
                                      graph_vertex_idx_t v,
                                      void *data);
typedef bool (*graph_vertex_runner_t)(const graph_t const *g,
                                      graph_vertex_idx_t v,
                                      void *priv);

struct graph {
    graph_vertex_idx_t vertices_number;
    graph_edge_count_t edges_number;
    bool directed;

    /* vector of void pointers */
    vector_t vertices_data;

    /* vector of lists of graph_vertex_idx_t */
    vector_t adjacency_list;

    /* key: edge_idx, data: edge data pointer */
    avl_tree_t edges;
};

struct graph_edge_found {
    bool found;
    void *data;
};

void graph_init(graph_t *g,
                graph_vertex_idx_t vertices_number,
                void **vertices_data,
                bool directed);
void graph_deinit(graph_t *g,
                  graph_edge_purger_t gep,
                  graph_vertex_purger_t gvp);

void *graph_vertex_data(const graph_t const *gr, graph_vertex_idx_t v);
void *graph_edge_data(const graph_t const *gr,
                      graph_vertex_idx_t from, graph_vertex_idx_t to);
void *graph_edge_idx_data(const graph_t const *gr,
                          graph_edge_idx_t edge);

graph_edge_idx_t graph_add_update_edge(graph_t *g,
                                       graph_vertex_idx_t from, graph_vertex_idx_t to,
                                       void *edge_data);
graph_edge_found_t graph_remove_edge(graph_t *g,
                                     graph_vertex_idx_t from,
                                     graph_vertex_idx_t to);
graph_edge_found_t graph_remove_edge_idx(graph_t *g,
                                         graph_edge_idx_t idx);
graph_edge_found_t graph_test_edge(const graph_t const *g,
                                   graph_vertex_idx_t from,
                                   graph_vertex_idx_t to);
graph_edge_found_t graph_test_edge_idx(const graph_t const *g,
                                       graph_edge_idx_t idx);

void graph_bfs(const graph_t const *g, graph_vertex_idx_t from,
               graph_vertex_runner_t runner, void *priv);
void graph_dfs(const graph_t const *g, graph_vertex_idx_t from,
               graph_vertex_runner_t runner, void *priv);

list_t *graph_random_path(const graph_t const *g,
                          graph_vertex_idx_t from,
                          graph_vertex_idx_t to,
                          uint64_t (*random_generator)(void *ptr),
                          void *ptr);
void graph_untie_path(const graph_t const *g, list_t *path);

static inline
graph_edge_idx_t graph_edge_idx(graph_vertex_idx_t __attribute__((unused)) total,
                                graph_vertex_idx_t from, graph_vertex_idx_t to) {
    return ((graph_edge_idx_t)(from) << 0x20) | to;
}

static inline
void graph_edge_from_idx(graph_vertex_idx_t __attribute__((unused)) total,
                         graph_edge_idx_t idx,
                         graph_vertex_idx_t *from, graph_vertex_idx_t *to) {
    *from = (idx >> 0x20) & 0xffffffff;
    *to = idx & 0xffffffff;
}

static inline
graph_edge_idx_t graph_edge_inverse_idx(graph_vertex_idx_t __attribute__((unused)) total,
                                        graph_edge_idx_t idx) {
    return ((idx & 0xffffffff) << 0x20) | ((idx >> 0x20) & 0xffffffff);
}

#endif /* _GRAPH_H_ */