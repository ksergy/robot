#ifndef _MULTIGRID_CONVERTOR_H_
# define _MULTIGRID_CONVERTOR_H_

# include "multigrid/multigrid.h"
# include "graph/graph.h"

struct graph_grid_edge {
    const grid_t *min_id;
    const grid_t *max_id;
};

typedef struct graph_grid_edge graph_grid_edge_t;

/**
 * Convert multigrid to graph.
 * Graph vertices will represent ungrided only grids.
 * Graph edge data will consist of two pointers
 * to neighbouring grids in graph_grid_edge_t.
 *
 * The graph_grid_edge_t may be free'd upon removing
 * appropriate graph edge.
 *
 * Resulting graph should be purged and free'd to clean
 * up used memroy.
 */
graph_t *multigrid_to_graph(const multigrid_t *mg);

void multigrid_purge_graph(graph_t *gr);

#endif /* _MULTIGRID_CONVERTOR_H_ */