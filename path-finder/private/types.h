#ifndef _PATH_FINDER_PRIVATE_TYPES_H_
# define _PATH_FINDER_PRIVATE_TYPES_H_

# include "graph/graph.h"
# include "lib/containers.h"

struct path_finder;
typedef struct path_finder path_finder_t;

typedef bool path_finder_func_t(const path_finder_t *pf,
                                graph_vertex_idx_t from, graph_vertex_idx_t to,
                                list_t *path);

#endif /* _PATH_FINDER_PRIVATE_TYPES_H_ */