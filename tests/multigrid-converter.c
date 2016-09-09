#include "multigrid-converter.h"
#include "multigrid-data.h"
#include "multigrid-converter/multigrid-converter.h"
#include "lib/avl-tree.h"

#include <stdlib.h>

struct T {
    graph_vertex_idx_t from;
    graph_vertex_idx_t to;
};

void check_node(avl_tree_node_t *atn, multigrid_t *mg, multigrid_graph_t *mg_g) {
    struct T *t = atn->data;

    avl_tree_node_t *atn_from, *atn_to;
    graph_vertex_idx_t from, to;

    atn_from = avl_tree_get(
        &mg_g->grid_to_graph_vertex,
        (avl_tree_key_t)multigrid_get_grid(mg, t->from)
    );

    atn_to = avl_tree_get(
        &mg_g->grid_to_graph_vertex,
        (avl_tree_key_t)multigrid_get_grid(mg, t->to)
    );

    from = *(graph_vertex_idx_t *)atn_from->data;
    to = *(graph_vertex_idx_t *)atn_to->data;

    graph_edge_found_t gef = graph_remove_edge(&mg_g->graph, from, to);

    ck_assert_int_eq(gef.found, true);

    free(gef.data);

    if (atn->left)
        check_node(atn->left, mg, mg_g);

    if (atn->right)
        check_node(atn->right, mg, mg_g);
}

START_TEST(test_multigrid_converter_ok) {
#define NEIGHBOURS(a,b)                                                         \
do {                                                                            \
    bool inserted = false;                                                      \
    struct T *e = avl_tree_add_or_get(                                          \
        &TST,                                                                   \
        graph_edge_idx(*(grid_id_t *)vector_get(&mg.level_capacity, 0) + 1,     \
                       (a), (b)), &inserted)->data;                             \
    e->from = (a); e->to = (b);                                                 \
} while(0)

    multigrid_t mg;
    multigrid_graph_t *mg_g;
    avl_tree_t TST;

    multigrid_init(
        &mg, GRID_WITH_PICTURE, !MULTIGRID_PICTURE_DIM_VERIFIED,
        &PIC1, &ds, MAX_LEVEL
    );
    multigrid_grid(&mg);

    avl_tree_init(&TST, true, sizeof(struct T));
    {
    NEIGHBOURS(2, 45);
    NEIGHBOURS(2, 131);
    NEIGHBOURS(2, 174);

    NEIGHBOURS(45, 88);
    NEIGHBOURS(45, 217);
    NEIGHBOURS(45, 174);
    NEIGHBOURS(45, 131);

    NEIGHBOURS(88, 260);
    NEIGHBOURS(88, 217);
    NEIGHBOURS(88, 174);

    NEIGHBOURS(260, 519);
    NEIGHBOURS(260, 1038);
    NEIGHBOURS(260, 1081);
    NEIGHBOURS(260, 1125);
    NEIGHBOURS(260, 1132);
    NEIGHBOURS(260, 1139);
    NEIGHBOURS(260, 1297);
    NEIGHBOURS(260, 865);
    NEIGHBOURS(260, 217);

    NEIGHBOURS(519, 1139);
    NEIGHBOURS(519, 1297);
    NEIGHBOURS(519, 1340);
    NEIGHBOURS(519, 1383);

    NEIGHBOURS(131, 174);
    NEIGHBOURS(131, 823);
    NEIGHBOURS(131, 779);

    NEIGHBOURS(174, 217);
    NEIGHBOURS(174, 865);
    NEIGHBOURS(174, 823);
    NEIGHBOURS(174, 830);
    NEIGHBOURS(174, 837);
    NEIGHBOURS(174, 779);

    NEIGHBOURS(217, 1038);
    NEIGHBOURS(217, 865);
    NEIGHBOURS(217, 837);

    NEIGHBOURS(779, 823);
    NEIGHBOURS(779, 844);
    NEIGHBOURS(779, 908);
    NEIGHBOURS(779, 951);

    NEIGHBOURS(823, 830);
    NEIGHBOURS(823, 844);
    NEIGHBOURS(823, 851);

    NEIGHBOURS(830, 837);
    NEIGHBOURS(830, 844);
    NEIGHBOURS(830, 851);
    NEIGHBOURS(830, 858);

    NEIGHBOURS(837, 865);
    NEIGHBOURS(837, 851);
    NEIGHBOURS(837, 858);

    NEIGHBOURS(865, 1038);
    NEIGHBOURS(865, 1167);
    NEIGHBOURS(865, 994);
    NEIGHBOURS(865, 951);

    NEIGHBOURS(1038, 1081);
    NEIGHBOURS(1038, 1210);
    NEIGHBOURS(1038, 1167);
    NEIGHBOURS(1038, 994);

    NEIGHBOURS(1081, 1125);
    NEIGHBOURS(1081, 1146);
    NEIGHBOURS(1081, 1253);
    NEIGHBOURS(1081, 1210);
    NEIGHBOURS(1081, 1167);

    NEIGHBOURS(1125, 1132);
    NEIGHBOURS(1125, 1153);
    NEIGHBOURS(1125, 1146);

    NEIGHBOURS(1132, 1139);
    NEIGHBOURS(1132, 1160);
    NEIGHBOURS(1132, 1153);
    NEIGHBOURS(1132, 1146);

    NEIGHBOURS(1139, 1153);
    NEIGHBOURS(1139, 1160);
    NEIGHBOURS(1139, 1297);

    NEIGHBOURS(1146, 1153);
    NEIGHBOURS(1146, 1253);
    NEIGHBOURS(1146, 1210);

    NEIGHBOURS(1153, 1160);

    NEIGHBOURS(1297, 1160);
    NEIGHBOURS(1297, 1340);
    NEIGHBOURS(1297, 1469);
    NEIGHBOURS(1297, 1426);
    NEIGHBOURS(1297, 1253);

    NEIGHBOURS(1340, 1383);
    NEIGHBOURS(1340, 1513);
    NEIGHBOURS(1340, 1469);
    NEIGHBOURS(1340, 1426);

    NEIGHBOURS(1383, 1513);
    NEIGHBOURS(1383, 1520);
    NEIGHBOURS(1383, 1527);
    NEIGHBOURS(1383, 1469);

    NEIGHBOURS(844, 851);
    NEIGHBOURS(844, 951);
    NEIGHBOURS(844, 908);

    NEIGHBOURS(851, 858);
    NEIGHBOURS(851, 951);

    NEIGHBOURS(858, 865);
    NEIGHBOURS(858, 994);
    NEIGHBOURS(858, 951);

    NEIGHBOURS(908, 951);
    NEIGHBOURS(951, 994);
    NEIGHBOURS(994, 1167);
    NEIGHBOURS(1160, 1426);
    NEIGHBOURS(1167, 1210);
    NEIGHBOURS(1210, 1253);
    NEIGHBOURS(1253, 1426);
    NEIGHBOURS(1253, 1153);
    NEIGHBOURS(1253, 1160);
    NEIGHBOURS(1426, 1469);
    NEIGHBOURS(1469, 1513);
    NEIGHBOURS(1469, 1534);

    NEIGHBOURS(1513, 1520);
    NEIGHBOURS(1513, 1541);
    NEIGHBOURS(1513, 1534);

    NEIGHBOURS(1520, 1527);
    NEIGHBOURS(1520, 1548);
    NEIGHBOURS(1520, 1541);
    NEIGHBOURS(1520, 1534);

    NEIGHBOURS(1527, 1548);
    NEIGHBOURS(1527, 1541);

    NEIGHBOURS(1534, 1541);

    NEIGHBOURS(1541, 1548);
    }

    mg_g = multigrid_to_graph(&mg);

    check_node(TST.root, &mg, mg_g);

    ck_assert_int_eq(mg_g->graph.edges_number, 0);

    multigrid_purge_graph(mg_g);

    avl_tree_purge(&TST);

    multigrid_purge(&mg);
}
END_TEST

Suite *multigrid_converter_suite(void) {
    Suite *s = suite_create("multigrid to graph converter");
    TCase *tc = tcase_create("multigrid to graph converter");

    tcase_add_test(tc, test_multigrid_converter_ok);
    suite_add_tcase(s, tc);

    return s;
}
