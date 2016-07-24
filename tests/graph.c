#include "graph.h"
#include "graph/graph.h"

graph_t *G = NULL;
bool DIRECTED;

void setup(void) {
    graph_edge_count_t i;
    graph_vertex_idx_t from, to;
    graph_edge_idx_t e;

    graph_init(G, 10, DIRECTED);

    for (i = 0, from = 0, to = 1; i < 4; ++i)
        e = graph_add_edge(G, from, to);

    for (i = 0, from = 1, to = 2; i < 3; ++i)
        e = graph_add_edge(G, from, to);

    for (i = 0, from = 2, to = 1; i < 2; ++i)
        e = graph_add_edge(G, from, to);

    for (i = 0, from = 2, to = 4; i < 8; ++i)
        e = graph_add_edge(G, from, to);

    for (i = 0, from = 4, to = 3; i < 9; ++i)
        e = graph_add_edge(G, from, to);

    for (i = 0, from = 3, to = 5; i < 5; ++i)
        e = graph_add_edge(G, from, to);

    for (i = 0, from = 5, to = 6; i < 7; ++i)
        e = graph_add_edge(G, from, to);

    for (i = 0, from = 6, to = 7; i < 10; ++i)
        e = graph_add_edge(G, from, to);

    for (i = 0, from = 7, to = 0; i < 13; ++i)
        e = graph_add_edge(G, from, to);

    for (i = 0, from = 7, to = 8; i < 11; ++i)
        e = graph_add_edge(G, from, to);

    for (i = 0, from = 9, to = 8; i < 12; ++i)
        e = graph_add_edge(G, from, to);
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

    ck_assert_int_eq(graph_test_edge(G, 0, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 1), 4);
    ck_assert_int_eq(graph_test_edge(G, 0, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 1, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 2), 3);
    ck_assert_int_eq(graph_test_edge(G, 1, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 2, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 1), 2);
    ck_assert_int_eq(graph_test_edge(G, 2, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 4), 8);
    ck_assert_int_eq(graph_test_edge(G, 2, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 3, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 5), 5);
    ck_assert_int_eq(graph_test_edge(G, 3, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 4, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 3), 9);
    ck_assert_int_eq(graph_test_edge(G, 4, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 5, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 6), 7);
    ck_assert_int_eq(graph_test_edge(G, 5, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 6, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 7), 10);
    ck_assert_int_eq(graph_test_edge(G, 6, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 7, 0), 13);
    ck_assert_int_eq(graph_test_edge(G, 7, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 8), 11);
    ck_assert_int_eq(graph_test_edge(G, 7, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 8, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 9, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 8), 12);
    ck_assert_int_eq(graph_test_edge(G, 9, 9), 0);

    teardown();
}
END_TEST

START_TEST(test_graph_undirected_ok) {
    graph_t g;

    G = &g;
    DIRECTED = !GRAPH_DIRECTED;

    setup();

    ck_assert_int_eq(graph_test_edge(G, 0, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 1), 4);
    ck_assert_int_eq(graph_test_edge(G, 0, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 7), 13);
    ck_assert_int_eq(graph_test_edge(G, 0, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 0, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 1, 0), 4);
    ck_assert_int_eq(graph_test_edge(G, 1, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 2), 5);
    ck_assert_int_eq(graph_test_edge(G, 1, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 1, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 2, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 1), 5);
    ck_assert_int_eq(graph_test_edge(G, 2, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 4), 8);
    ck_assert_int_eq(graph_test_edge(G, 2, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 2, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 3, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 4), 9);
    ck_assert_int_eq(graph_test_edge(G, 3, 5), 5);
    ck_assert_int_eq(graph_test_edge(G, 3, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 3, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 4, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 2), 8);
    ck_assert_int_eq(graph_test_edge(G, 4, 3), 9);
    ck_assert_int_eq(graph_test_edge(G, 4, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 4, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 5, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 3), 5);
    ck_assert_int_eq(graph_test_edge(G, 5, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 6), 7);
    ck_assert_int_eq(graph_test_edge(G, 5, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 5, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 6, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 5), 7);
    ck_assert_int_eq(graph_test_edge(G, 6, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 7), 10);
    ck_assert_int_eq(graph_test_edge(G, 6, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 6, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 7, 0), 13);
    ck_assert_int_eq(graph_test_edge(G, 7, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 6), 10);
    ck_assert_int_eq(graph_test_edge(G, 7, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 7, 8), 11);
    ck_assert_int_eq(graph_test_edge(G, 7, 9), 0);

    ck_assert_int_eq(graph_test_edge(G, 8, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 7), 11);
    ck_assert_int_eq(graph_test_edge(G, 8, 8), 0);
    ck_assert_int_eq(graph_test_edge(G, 8, 9), 12);

    ck_assert_int_eq(graph_test_edge(G, 9, 0), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 1), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 2), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 3), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 4), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 5), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 6), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 7), 0);
    ck_assert_int_eq(graph_test_edge(G, 9, 8), 12);
    ck_assert_int_eq(graph_test_edge(G, 9, 9), 0);

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
