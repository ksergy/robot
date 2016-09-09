#include "multigrid-converter.h"

#include "lib/set.h"

#include <stdlib.h>
#include <assert.h>

multigrid_graph_t *multigrid_to_graph(const multigrid_t const *mg) {
    multigrid_graph_t *mg_g;
    list_t queue;
    division_scheme_t pos;
    grid_id_t neigh_id;
    const grid_t *g, *neigh;
    list_element_t *qe;
    set_iterator_t neigh_it;
    const set_t *neighs;
    grid_edge_t direction;
    avl_tree_node_t *atn_ungrided;
    graph_edge_found_t gef;
    graph_vertex_idx_t from, to;
    void **vertices_data;
    void **vertex_data_ptr;

    assert(mg);

    mg_g = (multigrid_graph_t *)malloc(sizeof(multigrid_graph_t));
    avl_tree_init(&mg_g->grid_to_graph_vertex,
                  true, sizeof(graph_vertex_idx_t));

    list_init(&queue, false, 0);

    g = multigrid_get_grid(mg, 0);

    qe = list_append(&queue);
    qe->data = (void *)g;
    from = 0;

    do {
        qe = list_begin(&queue);
        g = qe->data;

        list_remove_and_advance(&queue, qe);

        if (!grid_grided(g)) {
            atn_ungrided = avl_tree_add(&mg_g->grid_to_graph_vertex,
                                        (avl_tree_key_t)g);
            *(graph_vertex_idx_t *)atn_ungrided->data = from ++;
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
    } while (list_size(&queue));   /* do .. while (list_size(&queue)) */

    list_purge(&queue);

    vertices_data = (void **)malloc(
        sizeof(void *) * mg_g->grid_to_graph_vertex.count);

    for (atn_ungrided = avl_tree_node_min(mg_g->grid_to_graph_vertex.root);
         atn_ungrided;
         atn_ungrided = avl_tree_node_next(atn_ungrided))
        vertices_data[*(graph_vertex_idx_t *)atn_ungrided->data] =
            (void *)atn_ungrided->key;

    graph_init(&mg_g->graph, mg_g->grid_to_graph_vertex.count,
               vertices_data, !GRAPH_DIRECTED);

    free(vertices_data);

    for (vertex_data_ptr = vector_begin(&mg_g->graph.vertices_data);
         vertex_data_ptr != vector_end(&mg_g->graph.vertices_data);
         vertex_data_ptr = vector_next(&mg_g->graph.vertices_data,
                                       vertex_data_ptr)) {
        g = (grid_t *)(*vertex_data_ptr);

        from = *(graph_vertex_idx_t *)avl_tree_get(
            &mg_g->grid_to_graph_vertex, (avl_tree_key_t)g)->data;

        for (direction = GE_MIN; direction < GE_MIN + GE_COUNT; ++direction) {
            neighs = grid_get_neighbours(g, direction);

            for (neigh_it = set_begin((set_t *)neighs); neigh_it.it;
                 neigh_it = set_next((set_t *)neighs, neigh_it.it)) {
                neigh_id = neigh_it.k;
                neigh = multigrid_get_grid(mg, neigh_id);

                if (grid_grided(neigh))
                    continue;

                to = *(graph_vertex_idx_t *)avl_tree_get(
                    &mg_g->grid_to_graph_vertex, (avl_tree_key_t)neigh)->data;

                gef = graph_test_edge(&mg_g->graph, from, to);

                if (gef.found)
                    continue;

                graph_add_update_edge(&mg_g->graph, from, to, NULL);
            }   /* for (neigh_it = set_begin((set_t *)neighs); neigh_it.it; */
        }   /* for (direction = GE_MIN; direction < GE_MIN + GE_COUNT; ++direction) */
    }   /*  for (vertex_data = vector_begin(&mg_g->graph.vertices_data); */

    return mg_g;
}

void multigrid_purge_graph(multigrid_graph_t *mg_g) {
    assert(mg_g);

    avl_tree_purge(&mg_g->grid_to_graph_vertex);
    graph_deinit(&mg_g->graph, NULL, NULL);

    free(mg_g);
}
