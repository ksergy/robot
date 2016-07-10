#include "multigrid.h"

#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define WITH_NEIGHBORHOOD       true
#define WITHOUT_NEIGHBORHOOD    false

#define RECURSIVE               true
#define NOT_RECURSIVE           false

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
void grid_grid(grid_t *g, bool with_negighborhood);

static
void grid_setup_neighborhood_children_relations(grid_t *g,
                                                bool recursive);

static inline
bool grid_check_neighbourhood(grid_t *g, grid_t *neighbour, enum edge edge);

static inline
grid_id_t grid_child_id(grid_t *g, grid_id_t idx);

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
grid_t *multigrid_get_grid(multigrid_t *host, grid_id_t id);

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
    unsigned int x, y;
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

    pv.v /= Y(pp[PP_SIZE]) * X(pp[PP_SIZE]);

    return pv;
}

void picture_divide(const picture_dimensions_t pic[PP_MAX],
                    const division_scheme_t *ds,
                    const division_scheme_t *pos,
                    picture_dimensions_t ret[PP_MAX]) {
    picture_dimensions_t offset;
    picture_dimensions_t size;
    size_t idx;

    assert(ds && pos);

    for (idx = 0; idx < DS_AXES_NUMBER; ++idx) {
        size.v[idx] = pic[PP_SIZE].v[idx] / ds->v[idx];
        offset.v[idx] = pic[PP_FROM].v[idx] + size.v[idx] * pos->v[idx];
    }
}

void grid_grid(grid_t *g, bool with_negighborhood) {
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

    /* TODO: rethink! */
    if (l < g->host->max_level) {
        g->grided = true;
        for (id_offset = 0, X(pos) = 0;
             X(pos) < X(host->ds);
             ++X(pos)) {
            for (Y(pos) = 0; Y(pos) < Y(host->ds);
                 /*id_offset += lcap,*/ ++Y(pos)) {
                picture_divide(pic, &host->ds, &pos, child_pic);
                child_pv = picture_value(&host->pic, child_pic);

                grid_id_t id = grid_child_id(
                    g, division_scheme_idx(&g->host->ds, &pos)
                );

                child = multigrid_add_grid(host, id);
                grid_init(host, child, id, l, g, &pos, child_pic, child_pv);

                grid_grid(child, with_negighborhood);
            }
        }
    }

    if (with_negighborhood)
        grid_setup_neighborhood_children_relations(g, RECURSIVE);
}

grid_id_t grid_child_id(grid_t *g, grid_id_t idx) {
    /* JUST FOR REFERENCE (?) */
    grid_level_t l = g->level + 1;
    grid_id_t id = g->id + 1;
    grid_id_t lcap = *(grid_id_t *)vector_get(&g->host->level_capacity, l);
    id += idx * lcap;

    return id;
}

void grid_setup_neighborhood_children_relations(grid_t *g, bool recursive) {
    assert(g);

    multigrid_t *host = g->host;
    division_scheme_t child_pos, neigh_pos, *ds = &host->ds;
    grid_id_t neigh_idx, child_idx;
    grid_id_t child_id, neigh_id;
    grid_t *child, *neigh;
    grid_edge_t e, rev_e;
    set_iterator_t neigh_it;

    if (!g->grided)
        return;

    /* ATP:
     * g is grided. g has some neighbours. g's childred are not grided.
     * g's children do not have any neighbours set
     */

#define init_child                                              \
do {                                                            \
    child_idx = division_scheme_idx(ds, &child_pos);            \
    child_id = grid_child_id(g, child_idx);                     \
    child = multigrid_get_grid(host, child_id);                 \
} while(0)

#define neighbours(edge)                                        \
do {                                                            \
    e = edge;                                                   \
    neigh_idx = division_scheme_idx(ds, &neigh_pos);            \
    neigh_id = grid_child_id(g, neigh_idx);                     \
    neigh = multigrid_get_grid(host, neigh_id);                 \
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
    grid_edge_pos_coord(edge, child->position)      \
    - grid_edge_pos_coord(edge, neigh->position)    \
)

#define neighbours_sub(edge)                                        \
do {                                                                \
    division_scheme_t sub_pos;                                      \
    grid_id_t sub_idx, sub_id;                                      \
    grid_t *sub;                                                    \
    grid_t *tmp;                                                    \
                                                                    \
    list_t q;                                                       \
    list_element_t *qe, *new_qe;                                    \
    list_init(&q, false, 0);                                        \
                                                                    \
    qe = list_append(&q);                                           \
    qe->data = neigh;                                               \
                                                                    \
    grid_edge_other_coord(edge, sub_pos) =                          \
        grid_edge_other_coord_val(edge, (*ds)) - 1;                 \
                                                                    \
    while (list_size(&q)) {                                         \
        qe = list_begin(&q);                                        \
        tmp = qe->data;                                             \
                                                                    \
        list_remove_and_advance(&q, qe);                            \
                                                                    \
        if (!tmp->grided)                                           \
            continue;                                               \
                                                                    \
        for (grid_edge_pos_coord(edge, sub_pos) = 0;                \
             grid_edge_pos_coord(edge, sub_pos) <                   \
                 grid_edge_pos_coord(edge, (*ds));                  \
             ++grid_edge_pos_coord(edge, sub_pos)) {                \
            sub_idx = division_scheme_idx(ds, &sub_pos);            \
            sub_id = grid_child_id(tmp, sub_idx);                   \
            sub = multigrid_get_grid(host, sub_id);                 \
                                                                    \
            grid_set_neighbourhood(child, sub, edge);               \
                                                                    \
            new_qe = list_append(&q);                               \
            new_qe->data = sub;                                     \
             }   /* for (grid_edge_pos_coord(edge, sub_pos) = 0; */ \
    }   /* while (list_size(&q)) */                                 \
} while(0)

#define EDGE(edge)                                                          \
do {                                                                        \
    X(child_pos) = Y(child_pos) = 0;                                        \
                                                                            \
    grid_edge_pos_coord(edge, child_pos) =                                  \
        grid_edge_coord_val(edge, (*ds)) - 1;                               \
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
            neigh = multigrid_get_grid(host, neigh_id);                     \
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
        neigh = multigrid_get_grid(host, neigh_id);                     \
        neighbours(edge);                                               \
    }                                                                   \
} while(0);

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

    grid_grid(mg->id_0, WITH_NEIGHBORHOOD);
}

list_t multigrid_id_to_path(multigrid_t *mg, grid_id_t id) {
    /* Path creation rules:
     * x,y>x,y>x,y
     *
     * x,y --- current level grid position in division scheme
     * >   --- level delimiter
     */

    grid_level_t l = 0;
    grid_id_t idx;
    division_scheme_t pos;
    list_t path;
    list_element_t *el;
    grid_id_t lcap;

    assert(mg);

    list_init(&path, true, sizeof(division_scheme_t));

    while (id > 0) {
        --id;
        ++l;

        lcap = *(grid_id_t *)vector_get(&mg->level_capacity, l);
        idx = id / lcap;

        el = list_append(&path);
        *(division_scheme_t *)el->data = division_scheme_idx_reverse(
            &mg->ds,
            idx
        );

        id -= idx * lcap;
    }

    return path;
}

grid_id_t multigrid_path_to_id(multigrid_t *mg, const list_t *path) {
    grid_id_t id = 0;
    grid_level_t l = 0;
    division_scheme_t *pos;
    list_element_t *next;
    grid_id_t lcap;

    assert(mg);

    if (!path)
        return 0;

    for (next = list_begin(path); next; next = list_next(path, next)) {
        ++l;
        ++id;
        pos = next->data;

        lcap = *(grid_id_t *)vector_get(&mg->level_capacity, l);
        id += division_scheme_idx(&mg->ds, pos) * lcap;
    }

    return id;
}
