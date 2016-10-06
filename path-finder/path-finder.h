#ifndef _PATH_FINDER_H_
# define _PATH_FINDER_H_

# include "graph/graph.h"
# include "lib/containers.h"

# include <stdbool.h>

/** \file path-finder.h
 * Various graph path finding algorithms library
 * Graph path is inplace list of \c graph_vertex_idx_t typed elements.
 */

/* common types */
/** path finder class */
enum path_finder_class {
    pf_a_star   = 0,
    pf_bfs,
    pf_dfs,
    pf_count
};

struct path_finder;
typedef struct path_finder path_finder_t;

/** the path finder object description */
struct path_finder {
    /** type of path finder */
    enum path_finder_class cls;
    /** graph to find path on */
    graph_t *g;

    /**
     * Callback to rerieve cost for graph vertex.
     * Callback signature depends on path finder class
     */
    union {
        void *f;
    } cost_getter;

    void *cost_getter_ctx;
};

/* common methods */
/**
 * Initialize path finder as of provided class \c cls,
 * for graph \c g, with cost getter \c cost_getter_cb
 * and its context \c cost_getter_ctx
 */
void path_finder_init(path_finder_t *pf,
                      enum path_finder_class cls, graph_t *g,
                      void *cost_getter_cb,
                      void *cost_getter_ctx);
void path_finder_deinit(path_finder_t *pf);

/**
 * Find path \c from -> \c to with path finder \c pf.
 * \param[out] path the resulting path, the list must not be initialized
 * \return \c true if path is found, \c false otherwise
 *
 * Regardless of the return value \c path list is initialized as inplace list
 * of \c graph_vertex_idx_t typed elements and must be purged.
 */
bool path_finder_find_path(const path_finder_t *pf,
                           graph_vertex_idx_t from, graph_vertex_idx_t to,
                           list_t *path);

#endif /* _PATH_FINDER_H_ */
