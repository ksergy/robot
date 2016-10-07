#include "path-finder.h"
#include "private/types.h"

#include <assert.h>

/************************* functions *************************/
extern path_finder_func_t bfs;
extern path_finder_func_t dfs;
extern path_finder_func_t a_star;

/************************* data *************************/
static path_finder_func_t *PATH_FINDER[pf_count] = {
    [pf_bfs] = bfs,
    [pf_dfs] = dfs,
    [pf_a_star] = a_star
};

/************************* API *************************/
void path_finder_init(path_finder_t *pf,
                      enum path_finder_class cls,
                      graph_t *g,
                      generic_cost_getter_t cost_getter_cb,
                      void *cost_getter_ctx) {
    assert(pf && g);

    pf->cls = cls;
    pf->g = g;
    pf->cost_getter.f = cost_getter_cb;
    pf->cost_getter_ctx = cost_getter_ctx;
}

void path_finder_deinit(path_finder_t *pf) {
    assert(pf);
}

bool path_finder_find_path(const path_finder_t *pf,
                           graph_vertex_idx_t from, graph_vertex_idx_t to,
                           list_t *path) {
    assert(pf && path);

    list_init(path, true, sizeof(graph_vertex_idx_t));

    return PATH_FINDER[pf->cls](pf, from, to, path);
}
