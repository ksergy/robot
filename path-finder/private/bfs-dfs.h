#ifndef _PATH_FINDER_PRIVATE_BFS_DFS_H_
# define _PATH_FINDER_PRIVATE_BFS_DFS_H_

# include "graph/graph.h"

# include <stdbool.h>

bool path_runner(const graph_t const *g,
                 graph_vertex_idx_t v,
                 vector_t *parent, vector_t *distance,
                 const graph_vertex_idx_t const *target);

#endif /* _PATH_FINDER_PRIVATE_BFS_DFS_H_ */