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

typedef uint32_t node_cost_t;

# define NODE_COST_INF ((node_cost_t)-1)

/**
 * Node cost getter will receive at least three arguments:
 * \param ctx user provided context
 * \param pf the path finder object
 * \param node node to retrieve cost for
 * \return \c node_cost_t a value [ 0 .. 2^32-1 )
 *
 * Node cost of 2^32 - 1 is reserved for infinity.
 *
 * Depending on path finding algorithms chosen there may be additional
 * parameters (see \c *_cost_getter_t types).
 */
typedef node_cost_t (*generic_cost_getter_t)(void *ctx,
                                             const path_finder_t const *pf,
                                             graph_vertex_idx_t node);

enum path_finder_a_star_cost_class {
    a_star_distance,
    a_star_heuristic,
    a_star_count
};
/**
 * Retrieve cost for edge \c node -> \c that_node
 * or heuristic cost estimation for path \c node -> \c that_node
 * depending on \c cls calculation class.
 */
typedef node_cost_t (*a_star_cost_getter_t)(void *ctx,
                                            const path_finder_t const *pf,
                                            graph_vertex_idx_t node,
                                            graph_vertex_idx_t that_node,
                                            enum path_finder_a_star_cost_class cls);

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
        generic_cost_getter_t generic;
        a_star_cost_getter_t a_star;
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
