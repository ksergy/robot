#include "graph.h"
#include "graph/graph.h"

graph_t *G = NULL;
bool DIRECTED;

void setup(void) {
    graph_edge_count_t i;
    graph_vertex_idx_t from, to;
    graph_edge_idx_t __attribute__((unused)) e;

    graph_init(G, 10, DIRECTED);

    for (i = 0, from = 0, to = 1; i < 4; ++i)
        e = graph_add_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 1, to = 2; i < 3; ++i)
        e = graph_add_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 2, to = 1; i < 2; ++i)
        e = graph_add_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 2, to = 4; i < 8; ++i)
        e = graph_add_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 4, to = 3; i < 9; ++i)
        e = graph_add_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 3, to = 5; i < 5; ++i)
        e = graph_add_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 5, to = 6; i < 7; ++i)
        e = graph_add_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 6, to = 7; i < 10; ++i)
        e = graph_add_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 7, to = 0; i < 13; ++i)
        e = graph_add_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 7, to = 8; i < 11; ++i)
        e = graph_add_edge(G, from, to, (void *) (i + 1));

    for (i = 0, from = 9, to = 8; i < 12; ++i)
        e = graph_add_edge(G, from, to, (void *) (i + 1));
}

void teardown(void) {
    graph_deinit(G);
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

Suite *graph_suite(void) {
    Suite *s = suite_create("graph");
    TCase *tc = tcase_create("directed multi");

    tcase_add_test(tc, test_graph_directed_ok);

    suite_add_tcase(s, tc);

    tc = tcase_create("undirected multi");

    tcase_add_test(tc, test_graph_undirected_ok);

    suite_add_tcase(s, tc);

    return s;
}
