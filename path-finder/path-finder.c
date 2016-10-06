#include "path-finder.h"

#include <assert.h>

/************************* API *************************/
void path_finder_init(path_finder_t *pf,
                      enum path_finder_class cls,
                      graph_t *g,
                      void *cost_getter_cb, void *cost_getter_ctx) {
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
    return false;
}
