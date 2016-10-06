#include "types.h"
#include "bfs-dfs.h"
#include "path-finder.h"
#include "graph/graph.h"

#include <stdbool.h>

bool dfs(const path_finder_t *pf,
         graph_vertex_idx_t from, graph_vertex_idx_t to,
         list_t *path) {
    vector_t distance, parent;
    graph_vertex_idx_t v;

    vector_init(&distance, sizeof(graph_vertex_idx_t), pf->g->vertices_number);
    vector_init(&parent, sizeof(graph_vertex_idx_t), pf->g->vertices_number);

    graph_dfs(pf->g, from,
              &parent, &distance,
              (graph_vertex_runner_t)path_runner, &to);

    if (-1 == *(graph_vertex_idx_t *)vector_get(&parent, to)) {
        vector_deinit(&parent);
        vector_deinit(&distance);

        return false;
    }

    /* traceback 'parent' into path from -> to */
    for (v = to; v != (graph_vertex_idx_t)-1;
         v = *(graph_vertex_idx_t *)vector_get(&parent, v))
         *(graph_vertex_idx_t *)(list_prepend(path)->data) = v;

    vector_deinit(&parent);
    vector_deinit(&distance);

    return true;
}
