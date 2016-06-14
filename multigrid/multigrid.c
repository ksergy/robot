#include "multigrid.h"

#include <string.h>
#include <stdbool.h>
#include <assert.h>

/************************** types **************************/
enum pic_pos {
    PP_FROM = 0,
    PP_SIZE = 1,
    PP_MAX
};

typedef struct pic_val {
    grid_value_t v;
    bool should_grid;
} pic_val_t;

/************************** functions declarations **************************/
static
void grid_init(multigrid_t *host,
               grid_t *g, grid_id_t id, pic_val_t v,
               grid_t *parent, const division_scheme_t *pos,
               const picture_dimensions_t pic[PP_MAX], grid_level_t level);

static
pic_val_t picture_value(const picture_t *pic,
                        const picture_dimensions_t pp[PP_MAX]);

static
grid_id_t division_scheme_mul(const division_scheme_t *ds);

static
void grid_grid(multigrid_t *host, grid_t *g);

/************************** functions definitions **************************/
void grid_init(multigrid_t *host,
               grid_t *g, grid_id_t id, pic_val_t v,
               grid_t *parent, const division_scheme_t *pos,
               const picture_dimensions_t pic[PP_MAX], grid_level_t level) {
    g->host = host;
    g->id = id;
    g->v = v.v;

    g->parent = parent;
    memcpy(&g->position, pos, sizeof(g->position));

    memcpy(&g->from, &pic[PP_FROM], sizeof(g->from));
    memcpy(&g->size, &pic[PP_SIZE], sizeof(g->size));

    g->level = level;

    g->should_grid = v.should_grid;
    g->grided = false;

    set_init(&g->neighbors);
}

grid_id_t division_scheme_mul(const division_scheme_t *ds) {
    grid_id_t r = 1;
    unsigned int idx;

    for (idx = 0; idx < DS_AXES_NUMBER; ++idx)
        r *= ds->v[idx];

    return r;
}

pic_val_t picture_value(const picture_t *pic,
                        const picture_dimensions_t pp[PP_MAX]) {
    pic_val_t pv;
    unsigned int x, y;
    grid_value_t original;

    x = pp[PP_FROM].v[DS_X_AXIS];
    y = pp[PP_FROM].v[DS_Y_AXIS];

    original = pic->p[x * pic->dim.v[DS_Y_AXIS] + y];

    for (pv.v = 0, pv.should_grid = false;
         !pv.should_grid &&
          x < pp[PP_FROM].v[DS_X_AXIS] + pp[PP_SIZE].v[DS_X_AXIS];
         ++x)
        for (y = pp[PP_FROM].v[DS_Y_AXIS];
             y < pp[PP_FROM].v[DS_Y_AXIS] + pp[PP_SIZE].v[DS_Y_AXIS];
             ++y) {
            pv.v += pic->p[x * pic->dim.v[DS_Y_AXIS] + y];
            if (original ^ pic->p[x * pic->dim.v[DS_Y_AXIS] + y])
                pv.should_grid = true;
        }

    for (; x < pp[PP_FROM].v[DS_X_AXIS] + pp[PP_SIZE].v[DS_X_AXIS];
         ++x)
        for (y = pp[PP_FROM].v[DS_Y_AXIS];
             y < pp[PP_FROM].v[DS_Y_AXIS] + pp[PP_SIZE].v[DS_Y_AXIS];
             ++y)
            pv.v += pic->p[x * pic->dim.v[DS_Y_AXIS] + y];

    pv.v /= pp[PP_SIZE].v[DS_Y_AXIS] * pp[PP_SIZE].v[DS_X_AXIS];

    return pv;
}

void grid_grid(multigrid_t *host, grid_t *g) {
    if (!g->should_grid)
        return;
#error "Not implemented"
    /* TODO */
}

/************************** API **************************/

void multigrid_init(multigrid_t *mg,
                    picture_t pic,
                    division_scheme_t ds,
                    grid_level_t max_level) {
    avl_tree_node_t *atn_0;
    grid_t *grid_0;
    pic_val_t pic_value;
    picture_dimensions_t pp[PP_MAX];
    division_scheme_t pos_0;
    sgrid_level_t level;
    grid_id_t wh;
    grid_id_t wh_pow_l;
    grid_id_t level_cap;
    unsigned int dim;

    assert(mg);

    for (dim = 0; dim < DS_AXES_NUMBER; ++dim)
        assert(0 == (pic.dim.v[dim] % ds.v[dim]));

    memcpy(&mg->pic, &pic, sizeof(mg->pic));
    memcpy(&mg->ds, &ds, sizeof(mg->ds));

    mg->id_0 = NULL;
    mg->max_level = max_level;

    /* set up level id offset */
    vector_init(&mg->level_capacity, sizeof(grid_id_t), mg->max_level + 1);

    wh = division_scheme_mul(&mg->ds);
    for (level = mg->max_level, wh_pow_l = 1, level_cap = 1;
         level >= 0;
         --level, wh_pow_l *= wh, level_cap += wh_pow_l)
        *(grid_id_t *)vector_get(&mg->level_capacity, level) = level_cap;

    /* allocate and initialize ID-0 grid */
    avl_tree_init(&mg->grids, true, sizeof(grid_t));

    atn_0 = avl_tree_add(&mg->grids, 0);

    assert(atn_0 != NULL);

    grid_0 = atn_0->data;

    memset(&pos_0, 0, sizeof(pos_0));

    memset(&pp[PP_FROM], 0, sizeof(pp[PP_FROM]));
    memcpy(&pp[PP_SIZE], &pic.dim, sizeof(pp[PP_SIZE]));

    pic_value = picture_value(&pic, pp);

    grid_init(mg, grid_0, 0, pic_value, NULL, &pos_0, pp, 0);

    mg->id_0 = grid_0;
}

void multigrid_purge(multigrid_t *mg) {
    assert(mg);

    mg->id_0 = 0;
    mg->max_level = 0;
    avl_tree_purge(&mg->grids);
}

void multigrid_grid(multigrid_t *mg) {
    assert(mg);

    grid_grid(mg, mg->id_0);
}

list_t grid_id_to_path(multigrid_t *mg, grid_id_t id) {
    /* TODO */
}

