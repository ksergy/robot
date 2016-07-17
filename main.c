#include "multigrid/multigrid.h"

#include <stdio.h>

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

const grid_value_t PIC_VAL[] = {
    /* pic(x, y) = p[x * dim[DS_Y_AXIS] + y],
     * x = 0 .. dim[DS_X_AXIS]
     * y = 0 .. dim[DS_Y_AXIS]
     */
/* y =   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  */
/*  0 */ V4(1),         V4(4),          V8(9),
/*  1 */ V4(1),         V4(4),          V8(9),
/*  2 */ V4(1),         V4(4),          V8(9),
/*  3 */ V4(1),         V4(4),          V8(9),
/*  4 */ V4(1),         V4(4),          V8(9),
/*  5 */ V4(1),         V4(4),          V8(9),
/*  6 */ V4(1),         V4(4),          V8(9),
/*  7 */ V4(1),         V4(4),          V8(9),
/*  8 */ V4(1),         V4(4),          V8(9),
/*  9 */ V4(2),         V4(5),          V8(9),
/* 10 */ V4(2),         V4(5),          V8(9),
/* 11 */ V4(2),         V4(5),          V8(9),
/* 12 */ V4(2),         V4(5),          V8(9),
/* 13 */ V4(2),         V4(5),          V8(9),
/* 14 */ V4(2),         V4(5),          V8(9),
/* 15 */ V4(2),         V4(5),          V8(9),
/* 16 */ V4(2),         V4(5),          V8(9),
/* 17 */ V4(2),         V4(5),          V8(9),
/* 18 */ V4(3),         V4(6),          V8(9),
/* 19 */ V4(3),         V4(6),          V8(9),
/* 20 */ V4(3),         V4(6),          V8(9),
/* 21 */ V4(3),         V4(6),          V8(9),
/* 22 */ V4(3),         V4(6),          V8(9),
/* 23 */ V4(3),         V4(6),          V8(9),
/* 24 */ V4(3),         V4(6),          V8(9),
/* 25 */ V4(3),         V4(6),          V8(9),
/* 26 */ V4(3),         V4(6),          V8(9),
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
/* 54 */ V8(8),                         V8(21),
/* 55 */ V8(8),                         V8(21),
/* 56 */ V8(8),                         V8(21),
/* 57 */ V8(8),                         V8(21),
/* 58 */ V8(8),                         V8(21),
/* 59 */ V8(8),                         V8(21),
/* 60 */ V8(8),                         V8(21),
/* 61 */ V8(8),                         V8(21),
/* 62 */ V8(8),                         V8(21),
/* 63 */ V8(8),                         V8(21),
/* 64 */ V8(8),                         V8(21),
/* 65 */ V8(8),                         V8(21),
/* 66 */ V8(8),                         V8(21),
/* 67 */ V8(8),                         V8(21),
/* 68 */ V8(8),                         V8(21),
/* 69 */ V8(8),                         V8(21),
/* 70 */ V8(8),                         V8(21),
/* 71 */ V8(8),                         V8(21),
/* 72 */ V8(8),                         V8(21),
/* 73 */ V8(8),                         V8(21),
/* 74 */ V8(8),                         V8(21),
/* 75 */ V8(8),                         V8(21),
/* 76 */ V8(8),                         V8(21),
/* 77 */ V8(8),                         V8(21),
/* 78 */ V8(8),                         V8(21),
/* 79 */ V8(8),                         V8(21),
/* 80 */ V8(8),                         V8(21)
};

static const picture_t PIC = {
    .dim =  {
        .v = {
            [DS_X_AXIS] = 81,
            [DS_Y_AXIS] = 16
        }
    },
    .p = PIC_VAL
};

/**********************************
 * Use case:
 * - allocate, create/fill picture
 * - allocate multigrid
 * - init multigrid
 * - multigrid_grid
 **********************************/

int main(void) {
    multigrid_t mg;
    grid_id_t id;
    division_scheme_t pos;
    const grid_t *g;
    list_t id_q;
    list_element_t *id_el;

    multigrid_init(
        &mg, GRID_WITH_PICTURE, !MULTIGRID_PICTURE_DIM_VERIFIED,
        &PIC, &ds, MAX_LEVEL
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
        printf("[%5llu : %2llu] grided: %d / %d "
               "(%3llu): "
               "(%4llu, %4llu) + (%4llu, %4llu) ... (%4llu, %4llu)\n",
               (long long unsigned int)g->id,
               (long long unsigned int)g->level,
               (int)g->grided, (int)g->should_grid,
               (long long unsigned int)g->v,
               (unsigned long long)X(g->from), (unsigned long long)Y(g->from),
               (unsigned long long)X(g->size), (unsigned long long)Y(g->size),
               (unsigned long long)(X(g->from) + X(g->size)),
               (unsigned long long)(Y(g->from) + Y(g->size))
              );

        if (g->grided)
            for (X(pos) = 0; X(pos) < X(ds); ++X(pos))
                for (Y(pos) = 0; Y(pos) < Y(ds); ++Y(pos))
                    list_append(&id_q)->data =
                        (void *)grid_child_id_pos(g, &pos);
    };

    /* ... TODO CHECK ... */

    multigrid_purge(&mg);

    return 0;
}