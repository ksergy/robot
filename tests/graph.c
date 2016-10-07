#include "graph.h"
#include "graph-data.h"
#include "graph/graph.h"
#include "lib/set.h"

#include <stdio.h>

typedef struct {
    /* vector of sets of neighbours */
    vector_t neighbours;

    list_t series;
} bfs_dfs_checker_t;


graph_t *G = NULL;
bool DIRECTED;

void setup(void) {
    graph_edge_count_t i;
    graph_vertex_idx_t from, to;
    graph_edge_idx_t __attribute__((unused)) e;
    void *vertices_data[10] = {
        (void *)0, (void *)1, (void *)2, (void *)3, (void *)4,
        (void *)5, (void *)6, (void *)7, (void *)8, (void *)9
    };

    graph_init(G, 10, vertices_data, DIRECTED);

    for (i = 0, from = 0, to = 1; i < 4; ++i)
        e = graph_add_update_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 1, to = 2; i < 3; ++i)
        e = graph_add_update_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 2, to = 1; i < 2; ++i)
        e = graph_add_update_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 2, to = 4; i < 8; ++i)
        e = graph_add_update_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 4, to = 3; i < 9; ++i)
        e = graph_add_update_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 3, to = 5; i < 5; ++i)
        e = graph_add_update_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 5, to = 6; i < 7; ++i)
        e = graph_add_update_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 6, to = 7; i < 10; ++i)
        e = graph_add_update_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 7, to = 0; i < 13; ++i)
        e = graph_add_update_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 7, to = 8; i < 11; ++i)
        e = graph_add_update_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 9, to = 8; i < 12; ++i)
        e = graph_add_update_edge(G, from, to, (void *) (i + 1));
}

void teardown(void) {
    graph_deinit(G, NULL, NULL);
    G = NULL;
}

START_TEST(test_graph_directed_ok) {
    graph_t g;

    G = &g;
    DIRECTED = GRAPH_DIRECTED;

    setup();

    ck_assert_int_eq(graph_test_edge(G, 0, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 1).data, 4);
    ck_assert_int_eq(graph_test_edge(G, 0, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 1, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 2).data, 3);
    ck_assert_int_eq(graph_test_edge(G, 1, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 2, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 1).data, 2);
    ck_assert_int_eq(graph_test_edge(G, 2, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 4).data, 8);
    ck_assert_int_eq(graph_test_edge(G, 2, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 3, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 5).data, 5);
    ck_assert_int_eq(graph_test_edge(G, 3, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 4, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 3).data, 9);
    ck_assert_int_eq(graph_test_edge(G, 4, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 5, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 6).data, 7);
    ck_assert_int_eq(graph_test_edge(G, 5, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 6, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 7).data, 10);
    ck_assert_int_eq(graph_test_edge(G, 6, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 7, 0).data, 13);
    ck_assert_int_eq(graph_test_edge(G, 7, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 8).data, 11);
    ck_assert_int_eq(graph_test_edge(G, 7, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 8, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 9, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 8).data, 12);
    ck_assert_int_eq(graph_test_edge(G, 9, 9).found, false);

    teardown();
}
END_TEST

START_TEST(test_graph_undirected_ok) {
    graph_t g;

    G = &g;
    DIRECTED = !GRAPH_DIRECTED;

    setup();

    ck_assert_int_eq(graph_test_edge(G, 0, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 1).data, 4);
    ck_assert_int_eq(graph_test_edge(G, 0, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 7).data, 13);
    ck_assert_int_eq(graph_test_edge(G, 0, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 0, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 1, 0).data, 4);
    ck_assert_int_eq(graph_test_edge(G, 1, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 2).data, 2);
    ck_assert_int_eq(graph_test_edge(G, 1, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 1, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 2, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 1).data, 2);
    ck_assert_int_eq(graph_test_edge(G, 2, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 4).data, 8);
    ck_assert_int_eq(graph_test_edge(G, 2, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 2, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 3, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 4).data, 9);
    ck_assert_int_eq(graph_test_edge(G, 3, 5).data, 5);
    ck_assert_int_eq(graph_test_edge(G, 3, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 3, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 4, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 2).data, 8);
    ck_assert_int_eq(graph_test_edge(G, 4, 3).data, 9);
    ck_assert_int_eq(graph_test_edge(G, 4, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 4, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 5, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 3).data, 5);
    ck_assert_int_eq(graph_test_edge(G, 5, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 6).data, 7);
    ck_assert_int_eq(graph_test_edge(G, 5, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 5, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 6, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 5).data, 7);
    ck_assert_int_eq(graph_test_edge(G, 6, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 7).data, 10);
    ck_assert_int_eq(graph_test_edge(G, 6, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 6, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 7, 0).data, 13);
    ck_assert_int_eq(graph_test_edge(G, 7, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 6).data, 10);
    ck_assert_int_eq(graph_test_edge(G, 7, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 7, 8).data, 11);
    ck_assert_int_eq(graph_test_edge(G, 7, 9).found, false);

    ck_assert_int_eq(graph_test_edge(G, 8, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 7).data, 11);
    ck_assert_int_eq(graph_test_edge(G, 8, 8).found, false);
    ck_assert_int_eq(graph_test_edge(G, 8, 9).data, 12);

    ck_assert_int_eq(graph_test_edge(G, 9, 0).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 1).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 2).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 3).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 4).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 5).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 6).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 7).found, false);
    ck_assert_int_eq(graph_test_edge(G, 9, 8).data, 12);
    ck_assert_int_eq(graph_test_edge(G, 9, 9).found, false);

    teardown();
}
END_TEST

static
bool check_bfs(const graph_t const *g, graph_vertex_idx_t v,
               vector_t *parent, vector_t *distance,
               bfs_dfs_checker_t *checker) {
    graph_vertex_idx_t idx;

    fprintf(
        stderr,
        "Enqueue %llu\n",
        (long long unsigned int)v
    );
    *(graph_vertex_idx_t *)(list_append(&checker->series)->data) = v;

    if (*(graph_vertex_idx_t *)(list_begin(&checker->series)->data) != v) {
        ck_assert_int_gt(
            set_count(
                (set_t *)
                vector_get(&checker->neighbours,
                           *(graph_vertex_idx_t *)
                           (list_begin(&checker->series)->data)),
                      v),
                      0
        );

        fprintf(
            stderr,
            "  Neighbour %llu of %llu (%llu)\n",
                (long long unsigned int)v,
                (long long unsigned int)(*(graph_vertex_idx_t *)
                                         (list_begin(&checker->series)->data)),
                (long long unsigned int)set_count(
                    (set_t *)vector_get(&checker->neighbours,
                                        *(graph_vertex_idx_t *)
                                        (list_begin(&checker->series)->data)),
                    v
                )
        );

        if (1 == set_size((set_t *)vector_get(&checker->neighbours,
                          *(graph_vertex_idx_t *)
                          (list_begin(&checker->series)->data)))) {
            fprintf(
                stderr,
                "   Dequeue %llu\n",
                (long long unsigned int)
                *(graph_vertex_idx_t *)(list_begin(&checker->series)->data)
            );

            list_remove_and_advance(&checker->series,
                                    list_begin(&checker->series));

        }   /* if (1 == set_size((set_t *)vector_get(&checker->neighbours, */
    }   /* if (*(graph_vertex_idx_t *)(list_begin(&checker->queue)->data) != v) */

    for (idx = 0; idx < v; ++idx)
        set_remove((set_t *)vector_get(&checker->neighbours, idx), v);

    for (; idx < g->vertices_number; ++idx)
        set_remove((set_t *)vector_get(&checker->neighbours, idx), v);

    while (list_begin(&checker->series) &&
           0 == set_size((set_t *)vector_get(&checker->neighbours,
                         *(graph_vertex_idx_t *)
                         (list_begin(&checker->series)->data)))) {
        fprintf(
            stderr,
            "   Dequeue %llu\n",
            (long long unsigned int)
            *(graph_vertex_idx_t *)(list_begin(&checker->series)->data)
        );

        list_remove_and_advance(&checker->series, list_begin(&checker->series));
    }   /* while (list_begin(&checker->queue) && */

    return true;
}

START_TEST(test_graph_bfs_ok) {
    static const graph_vertex_idx_t INITIAL = 0;

    graph_t g;
    bfs_dfs_checker_t bfs_checker;
    vector_t parent, distance;
    graph_vertex_idx_t v;
    list_t *adj_list;
    list_element_t *adj_v;

    setup_algorithms(&g);

    vector_init(&parent, sizeof(graph_vertex_idx_t), g.vertices_number);
    vector_init(&distance, sizeof(graph_vertex_idx_t), g.vertices_number);

    vector_init(&bfs_checker.neighbours, sizeof(set_t), g.vertices_number);
    for (v = 0; v < g.vertices_number; ++v) {
        set_init((set_t *)vector_get(&bfs_checker.neighbours, v));

        adj_list = vector_get(&g.adjacency_list, v);
        for (adj_v = list_begin(adj_list); adj_v;
             adj_v = list_next(adj_list, adj_v))
            set_add_single(
                (set_t *)vector_get(&bfs_checker.neighbours, v),
                *(graph_vertex_idx_t *)(adj_v->data)
            );
    }

    list_init(&bfs_checker.series, true, sizeof(graph_vertex_idx_t));

    graph_bfs(&g, INITIAL, &parent, &distance,
              (graph_vertex_runner_t)check_bfs, &bfs_checker);

    ck_assert_int_eq(list_size(&bfs_checker.series), 0);

    for (v = 0; v < g.vertices_number; ++v)
        ck_assert_int_eq(
            set_size((set_t *)vector_get(&bfs_checker.neighbours, v)), 0);

    for (v = 0; v < g.vertices_number; ++v)
        set_purge((set_t *)vector_get(&bfs_checker.neighbours, v));

    vector_deinit(&bfs_checker.neighbours);
    list_purge(&bfs_checker.series);

    vector_deinit(&distance);
    vector_deinit(&parent);

    teardown_algorithms(&g);
}
END_TEST

static
bool check_dfs(const graph_t const *g, graph_vertex_idx_t v,
               vector_t *parent, vector_t *distance,
               bfs_dfs_checker_t *checker) {
    graph_vertex_idx_t idx;
    list_element_t *last_el;

    fprintf(
        stderr,
        "Check %llu\n",
        (long long unsigned int)v
    );

    last_el = list_end(&checker->series);

    if (last_el &&
        v != *(graph_vertex_idx_t *)(last_el->data)) {
        ck_assert_int_gt(
            set_count(
                (set_t *)
                vector_get(&checker->neighbours,
                           *(graph_vertex_idx_t *)
                           (list_end(&checker->series)->data)),
                v
            ),
            0
        );

        fprintf(
            stderr,
            "  Neighbour %llu of %llu (%llu)\n",
                (long long unsigned int)v,
                (long long unsigned int)(*(graph_vertex_idx_t *)
                                         (list_end(&checker->series)->data)),
                (long long unsigned int)set_count(
                    (set_t *)vector_get(&checker->neighbours,
                                        *(graph_vertex_idx_t *)
                                        (list_end(&checker->series)->data)),
                    v
                )
        );
    }

    for (idx = 0; idx < v; ++idx)
        set_remove((set_t *)vector_get(&checker->neighbours, idx), v);

    for (; idx < g->vertices_number; ++idx)
        set_remove((set_t *)vector_get(&checker->neighbours, idx), v);

    while (list_end(&checker->series) &&
           0 == set_size((set_t *)vector_get(&checker->neighbours,
                         *(graph_vertex_idx_t *)
                         (list_end(&checker->series)->data)))) {
        fprintf(
            stderr,
            "   Dequeue %llu\n",
            (long long unsigned int)
            *(graph_vertex_idx_t *)(list_end(&checker->series)->data)
        );

        list_remove_and_advance(&checker->series, list_end(&checker->series));
    }

    fprintf(
        stderr,
        " Enqueue %llu\n",
        (long long unsigned int)v
    );
    *(graph_vertex_idx_t *)(list_append(&checker->series)->data) = v;

    if (0 == set_size((set_t *)vector_get(&checker->neighbours,
                      *(graph_vertex_idx_t *)
                      (list_end(&checker->series)->data)))) {
        fprintf(
            stderr,
            "   Dequeue %llu\n",
            (long long unsigned int)
            *(graph_vertex_idx_t *)(list_end(&checker->series)->data)
        );

        list_remove_and_advance(&checker->series, list_end(&checker->series));
    }

    return true;
}

START_TEST(test_graph_dfs_ok) {
    static const graph_vertex_idx_t INITIAL = 0;

    graph_t g;
    bfs_dfs_checker_t dfs_checker;
    vector_t parent, distance;
    graph_vertex_idx_t v;
    list_t *adj_list;
    list_element_t *adj_v;

    setup_algorithms(&g);

    vector_init(&parent, sizeof(graph_vertex_idx_t), g.vertices_number);
    vector_init(&distance, sizeof(graph_vertex_idx_t), g.vertices_number);

    vector_init(&dfs_checker.neighbours, sizeof(set_t), g.vertices_number);
    for (v = 0; v < g.vertices_number; ++v) {
        set_init((set_t *)vector_get(&dfs_checker.neighbours, v));

        adj_list = vector_get(&g.adjacency_list, v);
        for (adj_v = list_begin(adj_list); adj_v;
             adj_v = list_next(adj_list, adj_v))
            set_add_single(
                (set_t *)vector_get(&dfs_checker.neighbours, v),
                *(graph_vertex_idx_t *)(adj_v->data)
            );
    }

    list_init(&dfs_checker.series, true, sizeof(graph_vertex_idx_t));

    graph_dfs(&g, INITIAL, &parent, &distance,
              (graph_vertex_runner_t)check_dfs, &dfs_checker);

    ck_assert_int_eq(list_size(&dfs_checker.series), 0);

    for (v = 0; v < g.vertices_number; ++v)
        ck_assert_int_eq(
            set_size((set_t *)vector_get(&dfs_checker.neighbours, v)), 0);

    for (v = 0; v < g.vertices_number; ++v)
        set_purge((set_t *)vector_get(&dfs_checker.neighbours, v));

    vector_deinit(&dfs_checker.neighbours);
    list_purge(&dfs_checker.series);

    vector_deinit(&distance);
    vector_deinit(&parent);

    teardown_algorithms(&g);
}
END_TEST

START_TEST(test_graph_random_path_ok) {
    static const graph_vertex_idx_t FROM    = 0,
                                    TO      = 7;
    graph_t g;
    graph_drg_ctx_t ctx;
    list_t path;
    list_element_t *path_el1, *path_el2;

    setup_algorithms(&g);

    graph_init_default_random_generator(&ctx);

    list_init(&path, true, sizeof(graph_vertex_idx_t));

    graph_random_path(
        &g, FROM, TO,
        &path,
        (uint64_t (*)(void *))graph_default_random_generator,
        &ctx
    );

    ck_assert_int_gt(list_size(&path), 1);

    path_el1 = list_begin(&path);

    ck_assert_ptr_ne(path_el1, NULL);

    ck_assert_int_eq(*(graph_vertex_idx_t *)(path_el1->data), FROM);

    /* check if path from -> to is valid */
    for (path_el2 = list_next(&path, path_el1); path_el2;
         path_el1 = path_el2, path_el2 = list_next(&path, path_el1))
        ck_assert_int_eq(
            true,
            graph_test_edge(
                &g,
                *(graph_vertex_idx_t *)(path_el1->data),
                *(graph_vertex_idx_t *)(path_el2->data)
            ).found
        );

    ck_assert_int_eq(*(graph_vertex_idx_t *)(path_el1->data), TO);

    list_purge(&path);

    teardown_algorithms(&g);
}
END_TEST

START_TEST(test_graph_untie_path_ok) {
    graph_t g;
    list_t path;
    list_element_t *le_path;

    setup_algorithms(&g);

    list_init(&path, true, sizeof(graph_vertex_idx_t));
    *(graph_vertex_idx_t *)list_append(&path)->data = 0;
    *(graph_vertex_idx_t *)list_append(&path)->data = 3;
    *(graph_vertex_idx_t *)list_append(&path)->data = 6;
    *(graph_vertex_idx_t *)list_append(&path)->data = 5;
    *(graph_vertex_idx_t *)list_append(&path)->data = 4;
    *(graph_vertex_idx_t *)list_append(&path)->data = 3;
    *(graph_vertex_idx_t *)list_append(&path)->data = 9;
    *(graph_vertex_idx_t *)list_append(&path)->data = 8;
    *(graph_vertex_idx_t *)list_append(&path)->data = 2;
    *(graph_vertex_idx_t *)list_append(&path)->data = 9;

    graph_untie_path(&g, &path);

    ck_assert_int_eq(list_size(&path), 3);

    le_path = list_begin(&path);
    ck_assert_int_eq(*(graph_vertex_idx_t *)le_path->data, 0);

    le_path = list_next(&path, le_path);
    ck_assert_int_eq(*(graph_vertex_idx_t *)le_path->data, 3);

    le_path = list_next(&path, le_path);
    ck_assert_int_eq(*(graph_vertex_idx_t *)le_path->data, 9);

    list_purge(&path);

    teardown_algorithms(&g);
}
END_TEST

Suite *graph_suite(void) {
    Suite *s = suite_create("graph");
    TCase *tc = tcase_create("directed multi");

    tcase_add_test(tc, test_graph_directed_ok);

    suite_add_tcase(s, tc);

    tc = tcase_create("undirected multi");

    tcase_add_test(tc, test_graph_undirected_ok);

    suite_add_tcase(s, tc);

    tc = tcase_create("algorithms");

    tcase_add_test(tc, test_graph_bfs_ok);
    tcase_add_test(tc, test_graph_dfs_ok);
    tcase_add_loop_test(tc, test_graph_random_path_ok, 0, 1000);
    tcase_add_test(tc, test_graph_untie_path_ok);

    suite_add_tcase(s, tc);

    return s;
}
