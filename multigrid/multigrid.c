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
               grid_t *g, grid_id_t id, grid_level_t level,
               grid_t *parent, const division_scheme_t *pos,
               const picture_dimensions_t pic[PP_MAX], pic_val_t v);

static
void grid_grid(grid_t *g);

static
void grid_setup_neighborhood_children_relations(grid_t *g,
                                                bool recursive);

static
grid_id_t division_scheme_mul(const division_scheme_t *ds);

static
grid_t *multigrid_add_grid(multigrid_t *host, grid_id_t id);

static
pic_val_t picture_value(const picture_t *pic,
                        const picture_dimensions_t pp[PP_MAX]);

static
void picture_divide(const picture_dimensions_t pic[PP_MAX],
                    const division_scheme_t *ds,
                    const division_scheme_t *pos,
                    picture_dimensions_t ret[PP_MAX]);

/************************** functions definitions **************************/
void grid_init(multigrid_t *host,
               grid_t *g, grid_id_t id, grid_level_t level,
               grid_t *parent, const division_scheme_t *pos,
               const picture_dimensions_t pic[PP_MAX], pic_val_t v) {
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

void grid_grid(grid_t *g) {
    grid_id_t id_segment = g->id + 1;
    grid_level_t l = g->level + 1;
    multigrid_t *host = g->host;
    grid_id_t lcap = *(grid_id_t *)vector_get(&host->level_capacity, l);
    picture_dimensions_t pic[PP_MAX] = {
        [PP_FROM] = g->from,
        [PP_SIZE] = g->size
    };

    grid_id_t id_offset;
    picture_dimensions_t child_pic[PP_MAX];
    division_scheme_t pos;
    grid_t *child;
    pic_val_t child_pv;

    if (!g->should_grid)
        return;

    g->grided = true;
    for (id_offset = 0, pos.v[DS_X_AXIS] = 0;
         pos.v[DS_X_AXIS] < host->ds.v[DS_X_AXIS];
         ++pos.v[DS_X_AXIS]) {
        for (pos.v[DS_Y_AXIS] = 0; pos.v[DS_Y_AXIS] < host->ds.v[DS_Y_AXIS];
             id_offset += lcap, ++pos.v[DS_Y_AXIS]) {
            picture_divide(pic, &host->ds, &pos, child_pic);
            child_pv = picture_value(&host->pic, child_pic);

            grid_id_t id = id_segment + id_offset;

            child = multigrid_add_grid(host, id);
            grid_init(host, child, id, l, g, &pos, child_pic, child_pv);

            grid_grid(child);
        }
    }

    /* TODO make children relations setup selectable with a flag
     * for ceratain situations
     */
    grid_setup_neighborhood_children_relations(g, true);
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

    grid_init(mg, grid_0, 0, 0, NULL, &pos_0, pp, pic_value);

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

    grid_grid(mg->id_0);
}

list_t grid_id_to_path(multigrid_t *mg, grid_id_t id) {
    /* TODO */
}

