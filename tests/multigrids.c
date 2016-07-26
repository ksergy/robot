#include "multigrids.h"
#include "multigrid/multigrid.h"

static const grid_level_t MAX_LEVEL = 4;
static const division_scheme_t ds = {
    .v = {
        [DS_X_AXIS] = 3,
        [DS_Y_AXIS] = 2
    }
};

#define DIGIT(n)    n
#define V1(n)      DIGIT(n)
#define V2(n)      V1(n),V1(n)
#define V3(n)      V2(n),V1(n)
#define V4(n)      V3(n),V1(n)
#define V5(n)      V4(n),V1(n)
#define V6(n)      V5(n),V1(n)
#define V7(n)      V6(n),V1(n)
#define V8(n)      V7(n),V1(n)
#define V9(n)      V8(n),V1(n)
#define V10(n)     V9(n),V1(n)
#define V11(n)     V10(n),V1(n)
#define V12(n)     V11(n),V1(n)
#define V13(n)     V12(n),V1(n)
#define V14(n)     V13(n),V1(n)
#define V15(n)     V14(n),V1(n)
#define V16(n)     V15(n),V1(n)
#define V17(n)     V16(n),V1(n)
#define V18(n)     V17(n),V1(n)
#define V19(n)     V18(n),V1(n)
#define V20(n)     V19(n),V1(n)
#define V21(n)     V20(n),V1(n)
#define V22(n)     V21(n),V1(n)
#define V23(n)     V22(n),V1(n)
#define V24(n)     V23(n),V1(n)
#define V25(n)     V24(n),V1(n)
#define V26(n)     V25(n),V1(n)
#define V27(n)     V26(n),V1(n)
#define V28(n)     V27(n),V1(n)
#define V29(n)     V28(n),V1(n)
#define V30(n)     V29(n),V1(n)

#define Z16 V16(0)

const grid_value_t PIC1_VAL[] = {
    /* pic(x, y) = p[x * dim[DS_Y_AXIS] + y],
     * x = 0 .. dim[DS_X_AXIS]
     * y = 0 .. dim[DS_Y_AXIS]
     */
/* y =   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  */
/*  0 */ V4(1),         V4(4),          V4(32),         V4(35),
/*  1 */ V4(1),         V4(4),          V4(32),         V4(35),
/*  2 */ V4(1),         V4(4),          V4(32),         V4(35),
/*  3 */ V4(1),         V4(4),          V4(32),         V4(35),
/*  4 */ V4(1),         V4(4),          V4(32),         V4(35),
/*  5 */ V4(1),         V4(4),          V4(32),         V4(35),
/*  6 */ V4(1),         V4(4),          V4(32),         V4(35),
/*  7 */ V4(1),         V4(4),          V4(32),         V4(35),
/*  8 */ V4(1),         V4(4),          V4(32),         V4(35),
/*  9 */ V4(2),         V4(5),          V2(38), V2(41), V4(36),
/* 10 */ V4(2),         V4(5),          V2(38), V2(41), V4(36),
/* 11 */ V4(2),         V4(5),          V2(38), V2(41), V4(36),
/* 12 */ V4(2),         V4(5),          V2(39), V2(42), V4(36),
/* 13 */ V4(2),         V4(5),          V2(39), V2(42), V4(36),
/* 14 */ V4(2),         V4(5),          V2(39), V2(42), V4(36),
/* 15 */ V4(2),         V4(5),          V2(40), V2(43), V4(36),
/* 16 */ V4(2),         V4(5),          V2(40), V2(43), V4(36),
/* 17 */ V4(2),         V4(5),          V2(40), V2(43), V4(36),
/* 18 */ V4(3),         V4(6),          V4(34),         V4(37),
/* 19 */ V4(3),         V4(6),          V4(34),         V4(37),
/* 20 */ V4(3),         V4(6),          V4(34),         V4(37),
/* 21 */ V4(3),         V4(6),          V4(34),         V4(37),
/* 22 */ V4(3),         V4(6),          V4(34),         V4(37),
/* 23 */ V4(3),         V4(6),          V4(34),         V4(37),
/* 24 */ V4(3),         V4(6),          V4(34),         V4(37),
/* 25 */ V4(3),         V4(6),          V4(34),         V4(37),
/* 26 */ V4(3),         V4(6),          V4(34),         V4(37),
/* 27 */ V8(7),                         V4(10),         V4(18),
/* 28 */ V8(7),                         V4(10),         V4(18),
/* 29 */ V8(7),                         V4(10),         V4(18),
/* 30 */ V8(7),                         V4(10),         V4(18),
/* 31 */ V8(7),                         V4(10),         V4(18),
/* 32 */ V8(7),                         V4(10),         V4(18),
/* 33 */ V8(7),                         V4(10),         V4(18),
/* 34 */ V8(7),                         V4(10),         V4(18),
/* 35 */ V8(7),                         V4(10),         V4(18),
/* 36 */ V8(7),                         V4(11),         V4(19),
/* 37 */ V8(7),                         V4(11),         V4(19),
/* 38 */ V8(7),                         V4(11),         V4(19),
/* 39 */ V8(7),                         V4(11),         V4(19),
/* 40 */ V8(7),                         V4(11),         V4(19),
/* 41 */ V8(7),                         V4(11),         V4(19),
/* 42 */ V8(7),                         V4(11),         V4(19),
/* 43 */ V8(7),                         V4(11),         V4(19),
/* 44 */ V8(7),                         V4(11),         V4(19),
/* 45 */ V8(7),                         V2(12), V2(15), V4(20),
/* 46 */ V8(7),                         V2(12), V2(15), V4(20),
/* 47 */ V8(7),                         V2(12), V2(15), V4(20),
/* 48 */ V8(7),                         V2(13), V2(16), V4(20),
/* 49 */ V8(7),                         V2(13), V2(16), V4(20),
/* 50 */ V8(7),                         V2(13), V2(16), V4(20),
/* 51 */ V8(7),                         V2(14), V2(17), V4(20),
/* 52 */ V8(7),                         V2(14), V2(17), V4(20),
/* 53 */ V8(7),                         V2(14), V2(17), V4(20),
/* 54 */ V8(8),                         V4(21),         V4(24),
/* 55 */ V8(8),                         V4(21),         V4(24),
/* 56 */ V8(8),                         V4(21),         V4(24),
/* 57 */ V8(8),                         V4(21),         V4(24),
/* 58 */ V8(8),                         V4(21),         V4(24),
/* 59 */ V8(8),                         V4(21),         V4(24),
/* 60 */ V8(8),                         V4(21),         V4(24),
/* 61 */ V8(8),                         V4(21),         V4(24),
/* 62 */ V8(8),                         V4(21),         V4(24),
/* 63 */ V8(8),                         V4(22),         V4(25),
/* 64 */ V8(8),                         V4(22),         V4(25),
/* 65 */ V8(8),                         V4(22),         V4(25),
/* 66 */ V8(8),                         V4(22),         V4(25),
/* 67 */ V8(8),                         V4(22),         V4(25),
/* 68 */ V8(8),                         V4(22),         V4(25),
/* 69 */ V8(8),                         V4(22),         V4(25),
/* 70 */ V8(8),                         V4(22),         V4(25),
/* 71 */ V8(8),                         V4(22),         V4(25),
/* 72 */ V8(8),                         V4(23),         V2(26), V2(29),
/* 73 */ V8(8),                         V4(23),         V2(26), V2(29),
/* 74 */ V8(8),                         V4(23),         V2(26), V2(29),
/* 75 */ V8(8),                         V4(23),         V2(27), V2(30),
/* 76 */ V8(8),                         V4(23),         V2(27), V2(30),
/* 77 */ V8(8),                         V4(23),         V2(27), V2(30),
/* 78 */ V8(8),                         V4(23),         V2(28), V2(31),
/* 79 */ V8(8),                         V4(23),         V2(28), V2(31),
/* 80 */ V8(8),                         V4(23),         V2(28), V2(31)
};

const grid_value_t PIC2_VAL[] = {
    /* pic(x, y) = p[x * dim[DS_Y_AXIS] + y],
     * x = 0 .. dim[DS_X_AXIS]
     * y = 0 .. dim[DS_Y_AXIS]
     */
/* y =   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  */
/*  0 */ V4(1),         V4(4),          V8(19),
/*  1 */ V4(1),         V4(4),          V8(19),
/*  2 */ V4(1),         V4(4),          V8(19),
/*  3 */ V4(1),         V4(4),          V8(19),
/*  4 */ V4(1),         V4(4),          V8(19),
/*  5 */ V4(1),         V4(4),          V8(19),
/*  6 */ V4(1),         V4(4),          V8(19),
/*  7 */ V4(1),         V4(4),          V8(19),
/*  8 */ V4(1),         V4(4),          V8(19),
/*  9 */ V4(2),         V2(5),  V2(8),  V8(19),
/* 10 */ V4(2),         V2(5),  V2(8),  V8(19),
/* 11 */ V4(2),         V2(5),  V2(8),  V8(19),
/* 12 */ V4(2),         V2(6),  V2(9),  V8(19),
/* 13 */ V4(2),         V2(6),  V2(9),  V8(19),
/* 14 */ V4(2),         V2(6),  V2(9),  V8(19),
/* 15 */ V4(2),         V2(7),  V2(10), V8(19),
/* 16 */ V4(2),         V2(7),  V2(10), V8(19),
/* 17 */ V4(2),         V2(7),  V2(10), V8(19),
/* 18 */ V4(3),         V4(11),         V8(19),
/* 19 */ V4(3),         V4(11),         V8(19),
/* 20 */ V4(3),         V4(11),         V8(19),
/* 21 */ V4(3),         V4(11),         V8(19),
/* 22 */ V4(3),         V4(11),         V8(19),
/* 23 */ V4(3),         V4(11),         V8(19),
/* 24 */ V4(3),         V4(11),         V8(19),
/* 25 */ V4(3),         V4(11),         V8(19),
/* 26 */ V4(3),         V4(11),         V8(19),
/* 27 */ V8(12),                        V8(20),
/* 28 */ V8(12),                        V8(20),
/* 29 */ V8(12),                        V8(20),
/* 30 */ V8(12),                        V8(20),
/* 31 */ V8(12),                        V8(20),
/* 32 */ V8(12),                        V8(20),
/* 33 */ V8(12),                        V8(20),
/* 34 */ V8(12),                        V8(20),
/* 35 */ V8(12),                        V8(20),
/* 36 */ V8(12),                        V8(20),
/* 37 */ V8(12),                        V8(20),
/* 38 */ V8(12),                        V8(20),
/* 39 */ V8(12),                        V8(20),
/* 40 */ V8(12),                        V8(20),
/* 41 */ V8(12),                        V8(20),
/* 42 */ V8(12),                        V8(20),
/* 43 */ V8(12),                        V8(20),
/* 44 */ V8(12),                        V8(20),
/* 45 */ V8(12),                        V8(20),
/* 46 */ V8(12),                        V8(20),
/* 47 */ V8(12),                        V8(20),
/* 48 */ V8(12),                        V8(20),
/* 49 */ V8(12),                        V8(20),
/* 50 */ V8(12),                        V8(20),
/* 51 */ V8(12),                        V8(20),
/* 52 */ V8(12),                        V8(20),
/* 53 */ V8(12),                        V8(20),
/* 54 */ V4(13),        V4(16),         V8(21),
/* 55 */ V4(13),        V4(16),         V8(21),
/* 56 */ V4(13),        V4(16),         V8(21),
/* 57 */ V4(13),        V4(16),         V8(21),
/* 58 */ V4(13),        V4(16),         V8(21),
/* 59 */ V4(13),        V4(16),         V8(21),
/* 60 */ V4(13),        V4(16),         V8(21),
/* 61 */ V4(13),        V4(16),         V8(21),
/* 62 */ V4(13),        V4(16),         V8(21),
/* 63 */ V4(14),        V4(17),         V8(21),
/* 64 */ V4(14),        V4(17),         V8(21),
/* 65 */ V4(14),        V4(17),         V8(21),
/* 66 */ V4(14),        V4(17),         V8(21),
/* 67 */ V4(14),        V4(17),         V8(21),
/* 68 */ V4(14),        V4(17),         V8(21),
/* 69 */ V4(14),        V4(17),         V8(21),
/* 70 */ V4(14),        V4(17),         V8(21),
/* 71 */ V4(14),        V4(17),         V8(21),
/* 72 */ V4(15),        V4(18),         V8(21),
/* 73 */ V4(15),        V4(18),         V8(21),
/* 74 */ V4(15),        V4(18),         V8(21),
/* 75 */ V4(15),        V4(18),         V8(21),
/* 76 */ V4(15),        V4(18),         V8(21),
/* 77 */ V4(15),        V4(18),         V8(21),
/* 78 */ V4(15),        V4(18),         V8(21),
/* 79 */ V4(15),        V4(18),         V8(21),
/* 80 */ V4(15),        V4(18),         V8(21)
};

static const picture_t PIC1 = {
    .dim =  {
        .v = {
            [DS_X_AXIS] = 81,
            [DS_Y_AXIS] = 16
        }
    },
    .p = PIC1_VAL
};

static const picture_t PIC2 = {
    .dim =  {
        .v = {
            [DS_X_AXIS] = 81,
            [DS_Y_AXIS] = 16
        }
    },
    .p = PIC2_VAL
};

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
