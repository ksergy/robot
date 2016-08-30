#include "multigrids.h"
#include "multigrid-data.h"
#include "multigrid/multigrid.h"

START_TEST(test_pure_multigrid) {
    multigrid_t mg;
    grid_id_t id;
    division_scheme_t pos;
    const grid_t *g;
    list_t id_q;
    list_element_t *id_el;
    avl_tree_t T;
    avl_tree_node_t *atn;
    set_t *Tneigh;
    int direction;

#define GET_NEIGHBOUR(d)                \
do {                                    \
    Tneigh = &((set_t *)atn->data)[d];  \
} while(0)

#define GET_NEIGHBOUR_INIT(d)           \
do {                                    \
    Tneigh = &((set_t *)atn->data)[d];  \
    set_init(Tneigh);                   \
} while(0)

#define NEIGHBOUR(k) \
do {                                    \
    set_add_single(Tneigh, k);          \
} while(0)

    avl_tree_init(&T, true, GE_COUNT * sizeof(*Tneigh));

    /* Level - 0 */
    {
    atn = avl_tree_add(&T, 0);

    GET_NEIGHBOUR_INIT(GE_N);
    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    GET_NEIGHBOUR_INIT(GE_NW);
    }

    /* Level - 1 */
    {
    atn = avl_tree_add(&T, 1);

    GET_NEIGHBOUR_INIT(GE_N);
    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1037);
    NEIGHBOUR(1038);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(778);
    NEIGHBOUR(779);
    NEIGHBOUR(822);
    NEIGHBOUR(865);
    NEIGHBOUR(823);
    NEIGHBOUR(830);
    NEIGHBOUR(837);

    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 260);

    GET_NEIGHBOUR_INIT(GE_N);
    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(519);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1296);
    NEIGHBOUR(1297);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1037);
    NEIGHBOUR(1038);
    NEIGHBOUR(1081);
    NEIGHBOUR(1124);
    NEIGHBOUR(1125);
    NEIGHBOUR(1132);
    NEIGHBOUR(1139);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(778);
    NEIGHBOUR(865);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1);
    NEIGHBOUR(88);
    NEIGHBOUR(217);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 519);

    GET_NEIGHBOUR_INIT(GE_N);
    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1296);
    NEIGHBOUR(1297);
    NEIGHBOUR(1340);
    NEIGHBOUR(1383);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(1037);
    NEIGHBOUR(1124);
    NEIGHBOUR(1139);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 778);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1);
    NEIGHBOUR(131);
    NEIGHBOUR(174);
    NEIGHBOUR(217);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1037);
    NEIGHBOUR(1038);
    NEIGHBOUR(1167);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 1037);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(519);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1296);
    NEIGHBOUR(1297);
    NEIGHBOUR(1426);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(778);
    NEIGHBOUR(865);
    NEIGHBOUR(994);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(1);
    NEIGHBOUR(217);

    atn = avl_tree_add(&T, 1296);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(519);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1037);
    NEIGHBOUR(1124);
    NEIGHBOUR(1139);
    NEIGHBOUR(1160);
    NEIGHBOUR(1253);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(260);
    }

    /* Level - 2 */
    {
    atn = avl_tree_add(&T, 2);

    GET_NEIGHBOUR_INIT(GE_N);
    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(45);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(174);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(131);

    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 45);

    GET_NEIGHBOUR_INIT(GE_N);
    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(88);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(217);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(174);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(131);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(2);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 88);

    GET_NEIGHBOUR_INIT(GE_N);
    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(217);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(174);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(45);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 131);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(2);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(45);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(174);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(822);
    NEIGHBOUR(823);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(778);
    NEIGHBOUR(779);

    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 174);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(45);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(88);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(217);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(865);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(778);
    NEIGHBOUR(822);
    NEIGHBOUR(823);
    NEIGHBOUR(830);
    NEIGHBOUR(837);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(779);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(131);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(2);

    atn = avl_tree_add(&T, 217);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(88);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1037);
    NEIGHBOUR(1038);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(778);
    NEIGHBOUR(865);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(822);
    NEIGHBOUR(837);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(174);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(45);

    atn = avl_tree_add(&T, 779);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1);
    NEIGHBOUR(131);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(174);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(822);
    NEIGHBOUR(823);
    NEIGHBOUR(844);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(951);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(908);

    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 822);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1);
    NEIGHBOUR(174);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(217);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(865);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(994);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(951);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(908);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(779);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(131);

    atn = avl_tree_add(&T, 865);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1);
    NEIGHBOUR(217);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1037);
    NEIGHBOUR(1038);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1167);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(994);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(951);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(822);
    NEIGHBOUR(837);
    NEIGHBOUR(858);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(174);

    atn = avl_tree_add(&T, 908);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(779);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(822);
    NEIGHBOUR(844);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(951);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 951);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(822);
    NEIGHBOUR(844);
    NEIGHBOUR(851);
    NEIGHBOUR(858);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(865);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(994);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(908);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(779);

    atn = avl_tree_add(&T, 994);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(865);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(1038);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1037);
    NEIGHBOUR(1167);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(951);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(822);
    NEIGHBOUR(858);

    atn = avl_tree_add(&T, 1038);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_NE);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1081);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1210);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1167);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(994);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(778);
    NEIGHBOUR(865);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(1);
    NEIGHBOUR(217);

    atn = avl_tree_add(&T, 1081);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_NE);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1124);
    NEIGHBOUR(1125);
    NEIGHBOUR(1146);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1253);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1210);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(1167);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1038);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 1124);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(519);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1296);
    NEIGHBOUR(1297);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1426);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1253);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(1210);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1081);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 1167);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1038);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(1081);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1210);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(778);
    NEIGHBOUR(994);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(865);

    atn = avl_tree_add(&T, 1210);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1081);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(1124);
    NEIGHBOUR(1146);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1253);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1167);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(1038);

    atn = avl_tree_add(&T, 1253);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1124);
    NEIGHBOUR(1146);
    NEIGHBOUR(1153);
    NEIGHBOUR(1160);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(1297);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1296);
    NEIGHBOUR(1426);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1210);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(1081);

    atn = avl_tree_add(&T, 1297);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(519);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1340);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1469);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1426);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(1253);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1037);
    NEIGHBOUR(1124);
    NEIGHBOUR(1139);
    NEIGHBOUR(1160);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(260);

    atn = avl_tree_add(&T, 1340);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(519);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1383);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1512);
    NEIGHBOUR(1513);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1469);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(1426);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1297);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 1383);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(519);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1512);
    NEIGHBOUR(1513);
    NEIGHBOUR(1520);
    NEIGHBOUR(1527);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(1469);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1340);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 1426);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1297);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(1340);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1469);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1037);
    NEIGHBOUR(1253);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(1124);
    NEIGHBOUR(1160);

    atn = avl_tree_add(&T, 1469);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1340);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(1383);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1512);
    NEIGHBOUR(1513);
    NEIGHBOUR(1534);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1426);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(1297);

    atn = avl_tree_add(&T, 1512);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1383);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1469);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(1340);
    }

    /* Level - 3 */
    {
    atn = avl_tree_add(&T, 823);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1);
    NEIGHBOUR(174);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(830);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(851);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(844);

    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(779);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(131);

    atn = avl_tree_add(&T, 830);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1);
    NEIGHBOUR(174);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(837);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(858);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(851);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(844);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(823);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 837);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1);
    NEIGHBOUR(174);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(217);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(865);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(858);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(851);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(830);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 844);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(823);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(830);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(851);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(951);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(908);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(779);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 851);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(830);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(837);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(858);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(951);

    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(844);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(823);

    atn = avl_tree_add(&T, 858);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(837);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(865);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(994);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(951);

    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(851);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(830);

    atn = avl_tree_add(&T, 1125);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1132);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1153);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1146);

    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1081);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 1132);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1139);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1160);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1153);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(1146);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1125);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 1139);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(260);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(519);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1296);
    NEIGHBOUR(1297);

    GET_NEIGHBOUR_INIT(GE_SE);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1160);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(1153);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1132);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 1146);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1125);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(1132);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1153);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1253);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(1210);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1081);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 1153);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1132);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(1139);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1160);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1253);

    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1146);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(1125);

    atn = avl_tree_add(&T, 1160);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1139);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1296);
    NEIGHBOUR(1297);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1426);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1253);

    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1153);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(1132);

    atn = avl_tree_add(&T, 1513);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1383);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1520);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1541);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1534);

    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1469);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(1340);

    atn = avl_tree_add(&T, 1520);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1383);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1527);

    GET_NEIGHBOUR_INIT(GE_SE);
    NEIGHBOUR(1548);

    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1541);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(1534);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1513);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 1527);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1383);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    NEIGHBOUR(1548);

    GET_NEIGHBOUR_INIT(GE_SW);
    NEIGHBOUR(1541);

    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1520);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 1534);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1513);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(1520);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1541);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1469);

    GET_NEIGHBOUR_INIT(GE_NW);

    atn = avl_tree_add(&T, 1541);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1520);

    GET_NEIGHBOUR_INIT(GE_NE);
    NEIGHBOUR(1527);

    GET_NEIGHBOUR_INIT(GE_E);
    NEIGHBOUR(1548);

    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1534);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(1513);

    atn = avl_tree_add(&T, 1548);

    GET_NEIGHBOUR_INIT(GE_N);
    NEIGHBOUR(1527);

    GET_NEIGHBOUR_INIT(GE_NE);
    GET_NEIGHBOUR_INIT(GE_E);
    GET_NEIGHBOUR_INIT(GE_SE);
    GET_NEIGHBOUR_INIT(GE_S);
    GET_NEIGHBOUR_INIT(GE_SW);
    GET_NEIGHBOUR_INIT(GE_W);
    NEIGHBOUR(1541);

    GET_NEIGHBOUR_INIT(GE_NW);
    NEIGHBOUR(1520);
    }

    multigrid_init(
        &mg, GRID_WITH_PICTURE, !MULTIGRID_PICTURE_DIM_VERIFIED,
        &PIC1, &ds, MAX_LEVEL
    );

    multigrid_grid(&mg);

    list_init(&id_q, false, 0);
    id_el = list_append(&id_q);
    id_el->data = (void *)0;

    while (list_size(&id_q)) {
        id_el = list_begin(&id_q);
        id = (grid_id_t)id_el->data;
        list_remove_and_advance(&id_q, id_el);

        g = multigrid_get_grid(&mg, id);

        atn = avl_tree_get(&T, g->id);
        ck_assert_ptr_ne(atn, NULL);

        for (direction = GE_MIN; direction < GE_MIN + GE_COUNT; ++direction) {
            GET_NEIGHBOUR(direction);

            const set_t *g_neigh = grid_get_neighbours(g, direction);
            ck_assert_int_eq(set_size(Tneigh), set_size((set_t *)g_neigh));

            set_iterator_t it;

            for (it = set_begin((set_t *)g_neigh); it.it;
                 it = set_next((set_t *)g_neigh, it.it)) {
                ck_assert_int_ne(set_count(Tneigh, it.k), 0);
                set_remove(Tneigh, it.k);
            }

            ck_assert_int_eq(set_size(Tneigh), 0);

            set_purge(Tneigh);
        }

        if (g->grided)
            for (X(pos) = 0; X(pos) < X(ds); ++X(pos))
                for (Y(pos) = 0; Y(pos) < Y(ds); ++Y(pos))
                    list_append(&id_q)->data =
                        (void *)grid_child_id_pos(g, &pos);

        avl_tree_remove(&T, g->id);
    };

    ck_assert_int_eq(T.count, 0);

    avl_tree_purge(&T);
    multigrid_purge(&mg);
    list_purge(&id_q);
}
END_TEST

START_TEST(test_multilayer_multigrid) {
    multilayer_multigrid_t mmg;
    list_t pic_list;
    const multigrid_t *mg;
    grid_id_t id;
    division_scheme_t pos;
    const grid_t *g;
    list_t id_q;
    list_element_t *id_el;
    set_t TST;

    set_init(&TST);

    /* Level - 0 */
    set_add_single(&TST, 0);

    /* Level - 1 */
    set_add_single(&TST, 1);
    set_add_single(&TST, 260);
    set_add_single(&TST, 519);
    set_add_single(&TST, 778);
    set_add_single(&TST, 1037);
    set_add_single(&TST, 1296);

    /* Level - 2 */
    set_add_single(&TST, 2);
    set_add_single(&TST, 45);
    set_add_single(&TST, 88);
    set_add_single(&TST, 131);
    set_add_single(&TST, 174);
    set_add_single(&TST, 217);

    set_add_single(&TST, 520);
    set_add_single(&TST, 563);
    set_add_single(&TST, 606);
    set_add_single(&TST, 649);
    set_add_single(&TST, 692);
    set_add_single(&TST, 735);

    set_add_single(&TST, 779);
    set_add_single(&TST, 822);
    set_add_single(&TST, 865);
    set_add_single(&TST, 908);
    set_add_single(&TST, 951);
    set_add_single(&TST, 994);

    set_add_single(&TST, 1038);
    set_add_single(&TST, 1081);
    set_add_single(&TST, 1124);
    set_add_single(&TST, 1167);
    set_add_single(&TST, 1210);
    set_add_single(&TST, 1253);

    set_add_single(&TST, 1297);
    set_add_single(&TST, 1340);
    set_add_single(&TST, 1383);
    set_add_single(&TST, 1426);
    set_add_single(&TST, 1469);
    set_add_single(&TST, 1512);

    /* Level 3 */
    set_add_single(&TST, 175);
    set_add_single(&TST, 182);
    set_add_single(&TST, 189);
    set_add_single(&TST, 196);
    set_add_single(&TST, 203);
    set_add_single(&TST, 210);

    set_add_single(&TST, 823);
    set_add_single(&TST, 830);
    set_add_single(&TST, 837);
    set_add_single(&TST, 844);
    set_add_single(&TST, 851);
    set_add_single(&TST, 858);

    set_add_single(&TST, 1125);
    set_add_single(&TST, 1132);
    set_add_single(&TST, 1139);
    set_add_single(&TST, 1146);
    set_add_single(&TST, 1153);
    set_add_single(&TST, 1160);

    set_add_single(&TST, 1513);
    set_add_single(&TST, 1520);
    set_add_single(&TST, 1527);
    set_add_single(&TST, 1534);
    set_add_single(&TST, 1541);
    set_add_single(&TST, 1548);

    list_init(&pic_list, false, 0);

    list_append(&pic_list)->data = (void *)&PIC1;
    list_append(&pic_list)->data = (void *)&PIC2;

    multilayer_multigrid_init(
        &mmg,
        &pic_list,
        &ds,
        MAX_LEVEL
    );

    multilayer_multigrid_grid(&mmg);

    mg = multilayer_multigrid_get_mg(&mmg);

    list_init(&id_q, false, 0);
    id_el = list_append(&id_q);
    id_el->data = (void *)0;

    while (list_size(&id_q)) {
        id_el = list_begin(&id_q);
        id = (grid_id_t)id_el->data;
        list_remove_and_advance(&id_q, id_el);

        g = multigrid_get_grid(mg, id);

        ck_assert_int_ne(set_count(&TST, g->id), 0);

        set_remove(&TST, g->id);

        if (g->grided)
            for (X(pos) = 0; X(pos) < X(ds); ++X(pos))
                for (Y(pos) = 0; Y(pos) < Y(ds); ++Y(pos))
                    list_append(&id_q)->data =
                        (void *)grid_child_id_pos(g, &pos);
    };

    ck_assert_int_eq(set_size(&TST), 0);

    set_purge(&TST);

    multilayer_multigrid_purge(&mmg);
    list_purge(&id_q);
    list_purge(&pic_list);
}
END_TEST

Suite *multigrid_suite(void) {
    Suite *s = suite_create("multigrid");
    TCase *tc = tcase_create("multigrid");

    tcase_add_test(tc, test_pure_multigrid);
    tcase_add_test(tc, test_multilayer_multigrid);

    suite_add_tcase(s, tc);

    return s;
}
