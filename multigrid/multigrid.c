#include "multigrid.h"

#include <stdlib.h>
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
               const picture_dimensions_t pic[PP_MAX],
               bool has_value, pic_val_t v);

static
void grid_grid(grid_t *g, bool recursive, bool with_picture);

static
void grid_setup_children_neighbourhood(grid_t *g);

static
void grid_setup_children_neighbourhood_recursive(grid_t *g);

static
void grid_set_neighbourhood(grid_t *from, grid_t *to, grid_edge_t e);

static
grid_id_t division_scheme_mul(const division_scheme_t *ds);

static inline
grid_id_t division_scheme_idx(const division_scheme_t *ds,
                              const division_scheme_t *pos);

static
division_scheme_t division_scheme_idx_reverse(const division_scheme_t *ds,
                                              grid_id_t idx);

static
grid_t *multigrid_add_grid(multigrid_t *host, grid_id_t id);

static
grid_t *multigrid_get_grid_(multigrid_t *host, grid_id_t id);

static
pic_val_t picture_value(const picture_t *pic,
                        const picture_dimensions_t pp[PP_MAX]);

static
void picture_divide(const picture_dimensions_t pic[PP_MAX],
                    const division_scheme_t *ds,
                    const division_scheme_t *pos,
                    picture_dimensions_t ret[PP_MAX]);

static
void multilayer_multigrid_merge_multigrids(multilayer_multigrid_t *mmg);

static
void multilayer_multigrid_merge_multigrids_id(multilayer_multigrid_t *mmg,
                                              grid_id_t id);

/************************** functions definitions **************************/
void grid_init(multigrid_t *host,
               grid_t *g, grid_id_t id, grid_level_t level,
               grid_t *parent, const division_scheme_t *pos,
               const picture_dimensions_t pic[PP_MAX],
               bool has_value, pic_val_t v) {
    g->host = host;
    g->id = id;
    g->has_value = has_value;
    g->v = v.v;

    g->parent = parent;
    memcpy(&g->position, pos, sizeof(g->position));

    memcpy(&g->from, &pic[PP_FROM], sizeof(g->from));
    memcpy(&g->size, &pic[PP_SIZE], sizeof(g->size));

    g->level = level;

    g->should_grid = v.should_grid;
    g->grided = false;

    grid_edge_t e;
    for (e = GE_MIN; e <= GE_MAX; ++e)
        set_init(&g->neighbors[e]);
}

grid_id_t division_scheme_mul(const division_scheme_t *ds) {
    grid_id_t r = 1;
    unsigned int idx;

    for (idx = 0; idx < DS_AXES_NUMBER; ++idx)
        r *= ds->v[idx];

    return r;
}

grid_id_t division_scheme_idx(const division_scheme_t *ds,
                              const division_scheme_t *pos) {
    return X_PTR(ds) * Y_PTR(pos) + X_PTR(pos);
}

division_scheme_t division_scheme_idx_reverse(const division_scheme_t *ds,
                                              grid_id_t idx) {
    division_scheme_t pos;

    X(pos) = idx % X_PTR(ds);
    Y(pos) = idx / X_PTR(ds);

    return pos;
}

pic_val_t picture_value(const picture_t *pic,
                        const picture_dimensions_t pp[PP_MAX]) {
    pic_val_t pv;
    dimension_t x, y;
    grid_value_t original;

    x = X(pp[PP_FROM]);
    y = Y(pp[PP_FROM]);

    original = pic->p[x * Y(pic->dim) + y];

    for (pv.v = 0, pv.should_grid = false;
         !pv.should_grid &&
          x < X(pp[PP_FROM]) + X(pp[PP_SIZE]);
         ++x)
        for (y = Y(pp[PP_FROM]);
             y < Y(pp[PP_FROM]) + Y(pp[PP_SIZE]);
             ++y) {
            pv.v += pic->p[x * Y(pic->dim) + y];
            if (original ^ pic->p[x * Y(pic->dim) + y])
                pv.should_grid = true;
        }

    for (; x < X(pp[PP_FROM]) + X(pp[PP_SIZE]);
         ++x)
        for (y = Y(pp[PP_FROM]);
             y < Y(pp[PP_FROM]) + Y(pp[PP_SIZE]);
             ++y)
            pv.v += pic->p[x * Y(pic->dim) + y];

    pv.v /= division_scheme_mul(&pp[PP_SIZE]);

    return pv;
}

void picture_divide(const picture_dimensions_t pic[PP_MAX],
                    const division_scheme_t *ds,
                    const division_scheme_t *pos,
                    picture_dimensions_t ret[PP_MAX]) {
    size_t idx;

    assert(ds && pos);

    for (idx = 0; idx < DS_AXES_NUMBER; ++idx) {
        ret[PP_SIZE].v[idx] = pic[PP_SIZE].v[idx] / ds->v[idx];
        ret[PP_FROM].v[idx] = pic[PP_FROM].v[idx] + ret[PP_SIZE].v[idx] * pos->v[idx];
    }
}

void grid_grid(grid_t *g, bool recursive, bool with_picture) {
    grid_level_t l = g->level + 1;
    multigrid_t *host = g->host;
    picture_dimensions_t pic[PP_MAX] = {
        [PP_FROM] = g->from,
        [PP_SIZE] = g->size
    };

    picture_dimensions_t child_pic[PP_MAX];
    division_scheme_t pos;
    grid_t *child;
    pic_val_t child_pv;

    if (!g->should_grid)
        return;

    if (l >= g->host->max_level)
        return;

    g->grided = true;
    for (X(pos) = 0; X(pos) < X_PTR(host->ds); ++X(pos)) {
        for (Y(pos) = 0; Y(pos) < Y_PTR(host->ds); ++Y(pos)) {
            picture_divide(pic, host->ds, &pos, child_pic);
            child_pv = with_picture
                        ? picture_value(host->pic, child_pic)
                        : (pic_val_t){ .v = 0, .should_grid = false /* recursive ? */ };

            grid_id_t id = grid_child_id(
                g, division_scheme_idx(g->host->ds, &pos)
            );

            child = multigrid_add_grid(host, id);
            grid_init(host, child, id, l, g, &pos,
                      child_pic, with_picture, child_pv);

            if (recursive)
                grid_grid(child, GRID_RECURSIVE, with_picture);
        }   /* for (Y(pos) = 0; Y(pos) < Y(host->ds); ++Y(pos)) */
    }   /* for (X(pos) = 0; X(pos) < X(host->ds); ++X(pos)) */
}

void grid_set_neighbourhood(grid_t *from, grid_t *to, grid_edge_t e) {
    assert(from && to);

    set_add_single(&from->neighbors[e], to->id);
    set_add_single(&to->neighbors[grid_edge_inverse(e)], from->id);
}

void grid_setup_children_neighbourhood_recursive(grid_t *g) {
    list_t q;
    list_element_t *qe;
    multigrid_t *host = g->host;
    const division_scheme_t *ds = host->ds;
    grid_id_t idx;
    grid_id_t m = division_scheme_mul(ds);

    list_init(&q, false, sizeof(grid_t *));

    qe = list_append(&q);
    qe->data = g;

    while (list_size(&q)) {
        qe = list_begin(&q);
        g = (grid_t *)qe->data;

        list_remove_and_advance(&q, qe);

        if (!g->grided)
            continue;

        grid_setup_children_neighbourhood(g);

        for (idx = 0; idx < m; ++idx) {
            grid_id_t child_id = grid_child_id(g, idx);
            qe = list_append(&q);
            qe->data = multigrid_get_grid_(host, child_id);
        }   /* for (idx = 0; idx < m; ++idx) */
    }   /* while (list_size(&q)) */
}

void grid_setup_children_neighbourhood(grid_t *g) {
    multigrid_t *host = g->host;
    division_scheme_t child_pos, neigh_pos;
    const division_scheme_t *ds = host->ds;
    grid_id_t neigh_idx, child_idx;
    grid_id_t child_id, neigh_id;
    grid_t *child, *neigh;
    grid_edge_t e, rev_e;
    set_iterator_t neigh_it;

    /* ATP:
     * g is grided. g has some neighbours. g's childred are not grided.
     * g's children do not have any neighbours set
     */

#define init_child                                              \
do {                                                            \
    child_idx = division_scheme_idx(ds, &child_pos);            \
    child_id = grid_child_id(g, child_idx);                     \
    child = multigrid_get_grid_(host, child_id);                \
} while(0)

#define neighbours(edge)                                        \
do {                                                            \
    e = edge;                                                   \
    neigh_idx = division_scheme_idx(ds, &neigh_pos);            \
    neigh_id = grid_child_id(g, neigh_idx);                     \
    neigh = multigrid_get_grid_(host, neigh_id);                \
    grid_set_neighbourhood(child, neigh, e);                    \
} while(0)

    /* 1. set up neighbourhood inside division scheme */
    for (Y(child_pos) = 0; Y(child_pos) < Y_PTR(ds) - 1; ++ Y(child_pos)) {
        X(child_pos) = Y(child_pos) = 0;

        init_child;

        neigh_pos = child_pos;

        ++ X(neigh_pos);
        neighbours(GE_E);

        ++ Y(neigh_pos);
        neighbours(GE_SE);

        -- X(neigh_pos);
        neighbours(GE_S);

        for (X(child_pos) = 1; X(child_pos) < X_PTR(ds) - 1; ++ X(child_pos)) {
            init_child;

            neigh_pos = child_pos;

            ++ X(neigh_pos);
            neighbours(GE_E);

            ++ Y(neigh_pos);
            neighbours(GE_SE);

            -- X(neigh_pos);
            neighbours(GE_S);

            -- X(neigh_pos);
            neighbours(GE_SW);
        }   /* for (X(pos) = 1; X(pos) < X(ds) - 1; ++ X(pos)) */

        init_child;

        neigh_pos = child_pos;

        ++Y(neigh_pos);
        e = GE_S;
        neighbours(GE_S);
    }   /* for (Y(child_pos) = 0; Y(child_pos) < Y(ds) - 1; ++ Y(child_pos)) */

    for (X(child_pos) = 0; X(child_pos) < X_PTR(ds) - 1; ++ X(child_pos)) {
        init_child;

        neigh_pos = child_pos;

        ++ X(neigh_pos);
        e = GE_E;
        neighbours(GE_E);
    }   /* for (X(child_pos) = 0; X(child_pos) < X(ds) - 1; ++ X(child_pos)) */
#undef neighbours

    /* 2. inherit g's neighbours by children and set up neighbourhood
     * (only for outer edge children)
     */
#define neighbours(edge)                            \
do {                                                \
    e = edge;                                       \
    grid_set_neighbourhood(child, neigh, e);        \
} while(0)

#define delta_pos(edge)                             \
abs( /* coord for inversed edge is the same */      \
    grid_edge_fixed_coord(edge, child->position)    \
    - grid_edge_fixed_coord(edge, neigh->position)  \
)

#define neighbours_sub(edge)                                            \
do {                                                                    \
    division_scheme_t sub_pos;                                          \
    grid_id_t sub_idx, sub_id;                                          \
    grid_t *sub;                                                        \
    grid_t *tmp;                                                        \
                                                                        \
    list_t q;                                                           \
    list_element_t *qe, *new_qe;                                        \
    list_init(&q, false, 0);                                            \
                                                                        \
    qe = list_append(&q);                                               \
    qe->data = neigh;                                                   \
                                                                        \
    grid_edge_other_coord(edge, sub_pos) =                              \
        grid_edge_other_coord_val(edge, (*ds));                         \
                                                                        \
    while (list_size(&q)) {                                             \
        qe = list_begin(&q);                                            \
        tmp = qe->data;                                                 \
                                                                        \
        list_remove_and_advance(&q, qe);                                \
                                                                        \
        if (!tmp->grided)                                               \
            continue;                                                   \
                                                                        \
        for (grid_edge_fixed_coord(edge, sub_pos) = 0;                  \
             grid_edge_fixed_coord(edge, sub_pos) <                     \
                 grid_edge_fixed_coord(edge, (*ds));                    \
             ++grid_edge_fixed_coord(edge, sub_pos)) {                  \
            sub_idx = division_scheme_idx(ds, &sub_pos);                \
            sub_id = grid_child_id(tmp, sub_idx);                       \
            sub = multigrid_get_grid_(host, sub_id);                    \
                                                                        \
            grid_set_neighbourhood(child, sub, edge);                   \
                                                                        \
            new_qe = list_append(&q);                                   \
            new_qe->data = sub;                                         \
             }   /* for (grid_edge_fixed_coord(edge, sub_pos) = 0; */   \
    }   /* while (list_size(&q)) */                                     \
} while(0)

#define EDGE(edge)                                                          \
do {                                                                        \
    X(child_pos) = Y(child_pos) = 0;                                        \
                                                                            \
    grid_edge_fixed_coord(edge, child_pos) =                                \
        grid_edge_fixed_coord_val(edge, (*ds));                             \
                                                                            \
    for (grid_edge_other_coord(edge, child_pos) = 0;                        \
         grid_edge_other_coord(edge, child_pos) <                           \
             grid_edge_other_coord(edge, (*ds));                            \
         ++ grid_edge_other_coord(edge, child_pos)) {                       \
        init_child;                                                         \
                                                                            \
        for (neigh_it = set_begin(&g->neighbors[edge]);                     \
             neigh_it.it;                                                   \
             neigh_it = set_next(&g->neighbors[edge], neigh_it.it)) {       \
            neigh_id = neigh_it.k;                                          \
            neigh = multigrid_get_grid_(host, neigh_id);                    \
                                                                            \
            if (neigh->level > child->level)                                \
                continue;                                                   \
                                                                            \
            if (neigh->level < child->level)                                \
                neighbours(edge);                                           \
            else /* if (neigh->level) == child->level) */                   \
                if (delta_pos(edge) <= 1)                                   \
                    neighbours_sub(edge);                                   \
        }   /* for (neigh_it = set_begin(&g->neighbors[edge]); */           \
    }   /* for (grid_edge_other_coord) */                                   \
} while(0)

#define EDGE_CORNER(edge)                                               \
do {                                                                    \
    rev_e = grid_edge_inverse(edge);                                    \
    grid_corner_pos(edge, child_pos, (*ds));                            \
                                                                        \
    init_child;                                                         \
                                                                        \
    for (neigh_it = set_begin(&g->neighbors[rev_e]);                    \
         neigh_it.it;                                                   \
         neigh_it = set_next(&g->neighbors[rev_e], neigh_it.it)) {      \
        neigh_id = neigh_it.k;                                          \
        neigh = multigrid_get_grid_(host, neigh_id);                    \
        neighbours(edge);                                               \
    }                                                                   \
} while(0)

    EDGE(GE_N);
    EDGE_CORNER(GE_NE);
    EDGE(GE_E);
    EDGE_CORNER(GE_SE);
    EDGE(GE_S);
    EDGE_CORNER(GE_SW);
    EDGE(GE_W);
    EDGE_CORNER(GE_NW);

#undef EDGE_CORNER
#undef EDGE
#undef neighbours_sub
#undef delta_pos
#undef neighbours
#undef init_child
}

grid_t *multigrid_add_grid(multigrid_t *host, grid_id_t id) {
    avl_tree_node_t *atn;
    bool inserted = false;

    assert(host);

    atn = avl_tree_add_or_get(&host->grids, id, &inserted);

    assert((atn != NULL) && inserted);

    return atn->data;
}

grid_t *multigrid_get_grid_(multigrid_t *host, grid_id_t id) {
    avl_tree_node_t *atn;

    assert(host);

    atn = avl_tree_get(&host->grids, id);

    return atn ? atn->data : NULL;
}

void multilayer_multigrid_merge_multigrids(multilayer_multigrid_t *mmg) {
    grid_t *g_0 = multigrid_get_grid_(&mmg->mg, 0);

    multilayer_multigrid_merge_multigrids_id(mmg, 0);

    grid_setup_children_neighbourhood_recursive(g_0);
}

void multilayer_multigrid_merge_multigrids_id(multilayer_multigrid_t *mmg,
                                              grid_id_t id) {
    multigrid_t *mg;
    grid_t *mg_g;
    division_scheme_t g_child_pos;
    grid_t *g = multigrid_get_grid_(&mmg->mg, id);

    for (mg = vector_begin(&mmg->multigrids); mg;
         mg = vector_next(&mmg->multigrids, mg)) {
        mg_g = multigrid_get_grid_(mg, id);

        g->should_grid |= mg_g->grided;
    }

    grid_grid(g, !GRID_RECURSIVE, !GRID_WITH_PICTURE);

    for (X(g_child_pos) = 0; X(g_child_pos) < X_PTR(mmg->ds); ++X(g_child_pos))
        for (Y(g_child_pos) = 0; Y(g_child_pos) < Y_PTR(mmg->ds); ++Y(g_child_pos))
            multilayer_multigrid_merge_multigrids_id(
                mmg,
                grid_child_id(g, division_scheme_idx(mmg->ds, &g_child_pos))
            );
}

/************************** API **************************/
/********* multigrid *********/
void multigrid_init(multigrid_t *mg,
                    bool with_picture,
                    bool pic_dim_verified,
                    const picture_t *pic,
                    const division_scheme_t *ds,
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

    if (!pic_dim_verified)
        for (dim = 0; dim < DS_AXES_NUMBER; ++dim)
            assert(0 == (pic->dim.v[dim] % ds->v[dim]));

    mg->has_picture = with_picture;
    mg->pic = pic;
    mg->ds = ds;

    mg->id_0 = NULL;
    mg->max_level = max_level;

    /* set up level id offset */
    vector_init(&mg->level_capacity, sizeof(grid_id_t), mg->max_level + 1);

    wh = division_scheme_mul(mg->ds);
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
    memcpy(&pp[PP_SIZE], &pic->dim, sizeof(pp[PP_SIZE]));

    pic_value = with_picture
                 ? picture_value(pic, pp)
                 : (pic_val_t){ .v = 0, .should_grid = false };

    grid_init(mg, grid_0, 0, 0, NULL, &pos_0, pp, with_picture, pic_value);

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

    grid_grid(mg->id_0, GRID_RECURSIVE, mg->has_picture);
    grid_setup_children_neighbourhood_recursive(mg->id_0);
}

list_t multigrid_id_to_path(const multigrid_t *mg, grid_id_t id) {
    /* Path creation rules:
     * x,y>x,y>x,y
     *
     * x,y --- current level grid position in division scheme
     * >   --- level delimiter
     */

    grid_level_t l = 0;
    grid_id_t idx;
    list_t path;
    list_element_t *el;
    grid_id_t lcap;

    assert(mg);

    list_init(&path, true, sizeof(division_scheme_t));

    while (id > 0) {
        --id;
        ++l;

        lcap = *(grid_id_t *)vector_get((vector_t *)&mg->level_capacity, l);
        idx = id / lcap;

        el = list_append(&path);
        *(division_scheme_t *)el->data = division_scheme_idx_reverse(
            mg->ds,
            idx
        );

        id -= idx * lcap;
    }

    return path;
}

grid_id_t multigrid_path_to_id(const multigrid_t *mg, const list_t *path) {
    grid_id_t id = 0;
    grid_level_t l = 0;
    division_scheme_t *pos;
    const list_element_t *next;
    grid_id_t lcap;

    assert(mg);

    if (!path)
        return 0;

    for (next = list_begin((list_t *)path); next;
         next = list_next((list_t *)path, (list_element_t *)next)) {
        ++l;
        ++id;
        pos = next->data;

        lcap = *(grid_id_t *)vector_get((vector_t *)&mg->level_capacity, l);
        id += division_scheme_idx(mg->ds, pos) * lcap;
    }

    return id;
}

const grid_t *multigrid_get_grid(const multigrid_t *mg, grid_id_t id) {
    assert(mg);

    return multigrid_get_grid_((multigrid_t *)mg, id);
}

/********* grid *********/
bool grid_grided(const grid_t *g) {
    return g ? g->grided : false;
}

const set_t *grid_get_neighbours(const grid_t *g, grid_edge_t e) {
    return g && e < GE_COUNT ? &g->neighbors[e] : NULL;
}

grid_id_t grid_child_id_pos(const grid_t *g, const division_scheme_t *pos) {
    assert(g);

    return grid_child_id(g, division_scheme_idx(g->host->ds, pos));
}

grid_id_t grid_child_id(const grid_t *g, grid_id_t idx) {
    /* JUST FOR REFERENCE (?) */
    grid_level_t l = g->level + 1;
    grid_id_t id = g->id + 1;
    grid_id_t lcap = *(grid_id_t *)vector_get(&g->host->level_capacity, l);
    id += idx * lcap;

    return id;
}

/********* multilayer multigrid *********/
void multilayer_multigrid_init(multilayer_multigrid_t *mmg,
                               const list_t *pictures,
                               const division_scheme_t *ds,
                               grid_level_t max_level) {
    const list_element_t *pic_el;
    const picture_t *pic;
    const picture_dimensions_t *dim;
    unsigned int dim_idx;
    multigrid_t *mg;
    bool same_size = true;

    assert(mmg && pictures && ds && list_size(pictures));

    pic_el = list_begin((list_t *)pictures);
    pic = (const picture_t *)pic_el->data;
    dim = &pic->dim;

    for (dim_idx = 0; dim_idx < DS_AXES_NUMBER; ++dim)
        assert(0 == (dim->v[dim_idx] % ds->v[dim_idx]));

    for (pic_el = list_next((list_t *)pictures, (list_element_t *)pic_el);
         pic_el && same_size;
         pic_el = list_next((list_t *)pictures, (list_element_t *)pic_el)) {
        pic = pic_el->data;

        for (dim_idx = 0; dim_idx < DS_AXES_NUMBER; ++dim)
            same_size = (dim->v[dim_idx] == pic->dim.v[dim_idx]);
    }

    assert(same_size);

    mmg->pictures = pictures;
    mmg->ds = ds;
    mmg->max_level = max_level;

    vector_init(&mmg->multigrids,
                sizeof(multigrid_t), list_size(mmg->pictures));

    for (mg = vector_begin(&mmg->multigrids),
         pic_el = list_begin((list_t *)mmg->pictures);
         pic_el;
         mg = vector_next(&mmg->multigrids, mg),
         pic_el = list_next((list_t *)mmg->pictures, (list_element_t *)pic_el))
        multigrid_init(
            mg, GRID_WITH_PICTURE, MULTIGRID_PICTURE_DIM_VERIFIED,
            pic_el->data, mmg->ds, mmg->max_level
        );

    memcpy(&mmg->pic.dim, dim, sizeof(mmg->pic.dim));
    mmg->pic.p = NULL;
    multigrid_init(
        &mmg->mg, !GRID_WITH_PICTURE, MULTIGRID_PICTURE_DIM_VERIFIED,
        &mmg->pic, mmg->ds, mmg->max_level
    );
}

void multilayer_multigrid_grid(multilayer_multigrid_t *mmg) {
    multigrid_t *mg;

    assert(mmg);

    for (mg = vector_begin(&mmg->multigrids); mg;
         mg = vector_next(&mmg->multigrids, mg))
        multigrid_grid(mg);

    multilayer_multigrid_merge_multigrids(mmg);
}

const multigrid_t *multilayer_multigrid_get_mg(const multilayer_multigrid_t *mmg) {
    return mmg ? &mmg->mg : NULL;
}

void multilayer_multigrid_purge(multilayer_multigrid_t *mmg) {
    multigrid_t *mg;

    if (!mmg)
        return;

    for (mg = vector_begin(&mmg->multigrids); mg;
         mg = vector_next(&mmg->multigrids, mg))
        multigrid_purge(mg);

    vector_deinit(&mmg->multigrids);

    multigrid_purge(&mmg->mg);
}
