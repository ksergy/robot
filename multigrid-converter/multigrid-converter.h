#ifndef _MULTIGRID_CONVERTOR_H_
# define _MULTIGRID_CONVERTOR_H_

# include "multigrid/multigrid.h"
# include "graph/graph.h"

struct graph_grid_edge {
    const grid_t *min_id;
    const grid_t *max_id;
};

typedef struct graph_grid_edge graph_grid_edge_t;

struct multigrid_graph;
typedef struct multigrid_graph multigrid_graph_t;

struct multigrid_graph {
    graph_t graph;
    avl_tree_t grid_to_graph_vertex;
};

/**
 * Convert multigrid to graph.
 * Graph vertices will represent ungrided only grids.
 * Graph vertex data will point to appropriate grid.
 * Graph edge data will be equal NULL.
 *
 * Ungrided grids pointers will map to graph vertex
 * number with \c multigrid_graph_t::grid_to_graph_vertex
 *
 * Resulting graph should be purged and free'd to clean
 * up used memroy.
 */
multigrid_graph_t *multigrid_to_graph(const multigrid_t const *mg);

void multigrid_purge_graph(multigrid_graph_t *mg_g);

#endif /* _MULTIGRID_CONVERTOR_H_ */