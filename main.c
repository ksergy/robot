#include "multigrid/multigrid.h"

#include <stdio.h>

static const grid_level_t MAX_LEVEL = 4;
static const division_scheme_t ds = {
    .v = {
        [DS_X_AXIS] = 3,
        [DS_Y_AXIS] = 2
    }
};

#define Z2 0,0
#define Z4 Z2,Z2
#define Z8 Z4,Z4
#define Z16 Z8,Z8

const grid_value_t PIC_VAL[] = {
    /* pic(x, y) = p[x * dim[DS_Y_AXIS] + y],
     * x = 0 .. dim[DS_X_AXIS]
     * y = 0 .. dim[DS_Y_AXIS]
     */
/* y =   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  */
/*  0 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/*  1 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/*  2 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/*  3 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/*  4 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/*  5 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/*  6 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/*  7 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/*  8 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/*  9 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 10 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 11 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 12 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 13 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 14 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 15 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 16 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 17 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 18 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 19 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 20 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 21 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 22 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 23 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 24 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 25 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 26 */ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
/* 27 */ Z16,
/* 28 */ Z16,
/* 29 */ Z16,
/* 30 */ Z16,
/* 31 */ Z16,
/* 32 */ Z16,
/* 33 */ Z16,
/* 34 */ Z16,
/* 35 */ Z16,
/* 36 */ Z16,
/* 37 */ Z16,
/* 38 */ Z16,
/* 39 */ Z16,
/* 40 */ Z16,
/* 41 */ Z16,
/* 42 */ Z16,
/* 43 */ Z16,
/* 44 */ Z16,
/* 45 */ Z16,
/* 46 */ Z16,
/* 47 */ Z16,
/* 48 */ Z16,
/* 49 */ Z16,
/* 50 */ Z16,
/* 51 */ Z16,
/* 52 */ Z16,
/* 53 */ Z16,
/* 54 */ Z16,
/* 55 */ Z16,
/* 56 */ Z16,
/* 57 */ Z16,
/* 58 */ Z16,
/* 59 */ Z16,
/* 60 */ Z16,
/* 61 */ Z16,
/* 62 */ Z16,
/* 63 */ Z16,
/* 64 */ Z16,
/* 65 */ Z16,
/* 66 */ Z16,
/* 67 */ Z16,
/* 68 */ Z16,
/* 69 */ Z16,
/* 70 */ Z16,
/* 71 */ Z16,
/* 72 */ Z16,
/* 73 */ Z16,
/* 74 */ Z16,
/* 75 */ Z16,
/* 76 */ Z16,
/* 77 */ Z16,
/* 78 */ Z16,
/* 79 */ Z16,
/* 80 */ Z16
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
        printf("[%4llu] grided: %d (%llu): (%4llu, %4llu) + (%4llu, %4llu)\n",
               (long long unsigned int)g->id, (int)g->grided,
               (long long unsigned int)g->v,
               (unsigned long long)X(g->from), (unsigned long long)Y(g->from),
               (unsigned long long)X(g->size), (unsigned long long)Y(g->size));

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