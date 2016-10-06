#include "bfs-dfs.h"

bool path_runner(const graph_t const *g,
                 graph_vertex_idx_t v,
                 vector_t *parent, vector_t *distance,
                 const graph_vertex_idx_t const *target) {
    return *target != v;
}
