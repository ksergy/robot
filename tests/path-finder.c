#include "path-finder.h"
#include "graph-data.h"

#include "path-finder/path-finder.h"

#include <check.h>

static
bool lists_equal(list_t *a, list_t *b) {
    list_element_t *el_a, *el_b;

    if (list_size(a) != list_size(b))
        return false;

    for (el_a = list_begin(a), el_b = list_begin(b);
         el_a && el_b;
         el_a = list_next(a, el_a), el_b = list_next(b, el_b))
        if (*(graph_vertex_idx_t *)(el_a->data) != *(graph_vertex_idx_t *)(el_b->data))
            return false;

    return !(el_a || el_b);
}

START_TEST(test_bfs_ok) {
    static const graph_vertex_idx_t FROM    = 0,
                                    TO      = 7;
    graph_t g;
    path_finder_t pf;
    bool ret;
    list_t path_etalon;
    list_t path;

    list_init(&path_etalon, true, sizeof(graph_vertex_idx_t));
    *(graph_vertex_idx_t *)(list_append(&path_etalon)->data) = FROM;
    *(graph_vertex_idx_t *)(list_append(&path_etalon)->data) = 9;
    *(graph_vertex_idx_t *)(list_append(&path_etalon)->data) = 8;
    *(graph_vertex_idx_t *)(list_append(&path_etalon)->data) = TO;

    setup_algorithms(&g);
    path_finder_init(&pf, pf_bfs, &g, NULL, NULL);

    ret = path_finder_find_path(&pf, FROM, TO, &path);

    ck_assert_int_eq(ret, true);

    ret = lists_equal(&path_etalon, &path);
    ck_assert_int_eq(ret, true);

    list_purge(&path);
    path_finder_deinit(&pf);
    teardown_algorithms(&g);
    list_purge(&path_etalon);
}
END_TEST

START_TEST(test_dfs_ok) {
    static const graph_vertex_idx_t FROM    = 0,
                                    TO      = 7;
    graph_t g;
    path_finder_t pf;
    bool ret;
    list_t path_etalon;
    list_t path;

    list_init(&path_etalon, true, sizeof(graph_vertex_idx_t));
    *(graph_vertex_idx_t *)(list_append(&path_etalon)->data) = FROM;
    *(graph_vertex_idx_t *)(list_append(&path_etalon)->data) = 3;
    *(graph_vertex_idx_t *)(list_append(&path_etalon)->data) = 1;
    *(graph_vertex_idx_t *)(list_append(&path_etalon)->data) = 2;
    *(graph_vertex_idx_t *)(list_append(&path_etalon)->data) = 8;
    *(graph_vertex_idx_t *)(list_append(&path_etalon)->data) = TO;

    setup_algorithms(&g);
    path_finder_init(&pf, pf_dfs, &g, NULL, NULL);

    ret = path_finder_find_path(&pf, FROM, TO, &path);

    ck_assert_int_eq(ret, true);

    ret = lists_equal(&path_etalon, &path);
    ck_assert_int_eq(ret, true);

    list_purge(&path);
    path_finder_deinit(&pf);
    teardown_algorithms(&g);
    list_purge(&path_etalon);
}
END_TEST

Suite *path_finder_suite(void) {
    Suite *s = suite_create("path finder");
    TCase *tc = tcase_create("bfs");

    tcase_add_test(tc, test_bfs_ok);

    suite_add_tcase(s, tc);

    tc = tcase_create("dfs");

    tcase_add_test(tc, test_dfs_ok);

    suite_add_tcase(s, tc);

    return s;
}
