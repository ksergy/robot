#include "multigrid-converter.h"

#include <stdlib.h>

static
void purge_edge(graph_vertex_idx_t __attribute__((unused)) from,
                graph_vertex_idx_t __attribute__((unused)) to,
                void *data) {
    free(data);
}

graph_t *multigrid_to_graph(const multigrid_t *mg) {
    graph_t *gr;
    list_t queue;
    division_scheme_t pos;
    grid_id_t id, neigh_id, id_min, id_max;
    const grid_t *g, *neigh, *min_id, *max_id;
    list_element_t *qe;
    set_iterator_t neigh_it;
    const set_t *neighs;
    grid_edge_t direction;
    graph_edge_found_t gef;
    graph_grid_edge_t *gge;

    list_init(&queue, false, 0);

    gr = (graph_t *)malloc(sizeof(*gr));
    graph_init(
        gr,
        *(grid_id_t *)vector_get((vector_t *)(&mg->level_capacity), 0) + 1 /* id: 0 */,
        !GRAPH_DIRECTED
    );

    g = multigrid_get_grid(mg, 0);

    qe = list_append(&queue);
    qe->data = (void *)g;

    while (list_size(&queue)) {
        qe = list_begin(&queue);
        g = qe->data;

        list_remove_and_advance(&queue, qe);

        if (!grid_grided(g)) {
            for (direction = GE_MIN; direction < GE_MIN + GE_COUNT; ++direction) {
                neighs = grid_get_neighbours(g, direction);
                for (neigh_it = set_begin((set_t *)neighs); neigh_it.it;
                     neigh_it = set_next((set_t *)neighs, neigh_it.it)) {
                    neigh_id = neigh_it.k;
                    neigh = multigrid_get_grid(mg, neigh_id);

                    if (grid_grided(neigh))
                        continue;

                    id = g->id;

                    if (id < neigh_id) {
                        id_min = id;
                        id_max = neigh_id;
                        min_id = g;
                        max_id = neigh;
                    }
                    else {
                        id_min = neigh_id;
                        id_max = id;
                        min_id = neigh;
                        max_id = g;
                    }

                    gef = graph_test_edge(gr, id_min, id_max);

                    if (gef.found)
                        continue;

                    gge = malloc(sizeof(gge));

                    gge->min_id = min_id;
                    gge->max_id = max_id;

                    graph_add_edge(gr, id_min, id_max, gge);
                }   /* for (neigh_it = set_begin((set_t *)neighs); neigh_it.it; */
            }   /* for (direction = GE_MIN; direction < GE_MIN + GE_COUNT; ++direction) */

            continue;
        }   /* if (!grid_grided(g)) */

        for (X(pos) = 0; X(pos) < X_PTR(mg->ds); ++X(pos)) {
            for (Y(pos) = 0; Y(pos) < Y_PTR(mg->ds); ++Y(pos)) {
                list_append(&queue)->data = (void *)(
                    multigrid_get_grid(
                        mg,
                        grid_child_id_pos(g, &pos)
                    )
                );
            }   /* for (Y(pos) = 0; Y(pos) < Y(mg->ds); ++Y(pos)) */
        }   /* for (X(pos) = 0; X(pos) < X(mg->ds); ++X(pos)) */
    }   /* while (list_size(&queue)) */

    list_purge(&queue);

    return gr;
}

void multigrid_purge_graph(graph_t *gr) {
    if (!gr)
        return;

    graph_deinit(gr, purge_edge);
}
