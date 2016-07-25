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
