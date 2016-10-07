#include "graph-data.h"

#include <check.h>

void setup_algorithms(graph_t *g) {
    static void *vdata[10];
    graph_init(g, 10, vdata, !GRAPH_DIRECTED);

    graph_add_update_edge(g, 0, 3, (void *)1);
    graph_add_update_edge(g, 0, 9, (void *)2);

    graph_add_update_edge(g, 1, 2, (void *)3);
    graph_add_update_edge(g, 1, 3, (void *)4);

    graph_add_update_edge(g, 2, 8, (void *)5);
    graph_add_update_edge(g, 2, 9, (void *)6);

    graph_add_update_edge(g, 3, 4, (void *)7);
    graph_add_update_edge(g, 3, 6, (void *)8);
    graph_add_update_edge(g, 3, 9, (void *)9);

    graph_add_update_edge(g, 4, 5, (void *)10);

    graph_add_update_edge(g, 5, 6, (void *)11);
    graph_add_update_edge(g, 5, 7, (void *)12);

    graph_add_update_edge(g, 7, 8, (void *)13);

    graph_add_update_edge(g, 8, 9, (void *)14);

    ck_assert_int_eq(g->edges_number, 14);
}

void teardown_algorithms(graph_t *g) {
    graph_deinit(g, NULL, NULL);
}
