#ifndef _GRAPH_H_
# define _GRAPH_H_

# include "lib/containers.h"
# include "lib/avl-tree.h"

# include <stdbool.h>

# define GRAPH_DIRECTED true

struct graph;
typedef struct graph graph_t;

typedef uint32_t graph_vertex_idx_t;
typedef uint64_t graph_edge_count_t;
typedef uint64_t graph_edge_idx_t;

struct graph {
    graph_vertex_idx_t vertices_number;
    graph_edge_count_t edges_number;
    bool directed;

    /* key = graph_edge_idx(from, to) */
    avl_tree_t adjacency_map;
};

void graph_init(graph_t *g, graph_vertex_idx_t vertices_number, bool directed);
void graph_deinit(graph_t *g);

graph_edge_idx_t graph_add_edge(graph_t *g,
                                graph_vertex_idx_t from, graph_vertex_idx_t to);
graph_edge_count_t graph_remove_edge(graph_t *g,
                                     graph_vertex_idx_t from,
                                     graph_vertex_idx_t to);
graph_edge_count_t graph_remove_edge_idx(graph_t *g,
                                         graph_edge_idx_t idx);
graph_edge_count_t graph_test_edge(graph_t *g,
                                   graph_vertex_idx_t from,
                                   graph_vertex_idx_t to);
graph_edge_count_t graph_test_edge_idx(graph_t *g,
                                       graph_edge_idx_t idx);

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