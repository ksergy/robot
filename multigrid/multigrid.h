#ifndef _MULTIGRID_H_
# define _MULTIGRID_H_

/** \file multigrid.h
 * Multigrid library.
 */

# include "lib/containers.h"
# include "lib/set.h"
# include "lib/avl-tree.h"

# include <stdint.h>
# include <stdbool.h>

typedef enum division_scheme_axes {
    DS_X_AXIS = 0,
    DS_Y_AXIS = 1,
    DS_AXES_NUMBER
} division_scheme_axes_t;

# define DS_X_AXIS DS_X_AXIS
# define DS_Y_AXIS DS_Y_AXIS
# define DS_AXES_NUMBER DS_AXES_NUMBER

struct grid;
typedef struct grid grid_t;

struct multigrid;
typedef struct multigrid multigrid_t;

struct picture;
typedef struct picture picture_t;

typedef struct division_scheme {
    unsigned int v[DS_AXES_NUMBER];
} division_scheme_t;

# define COORD_NAME(name)   DS_##name##_AXIS
# define COORD(name, a)     a.v[COORD_NAME(name)]
# define X(a)               COORD(X, a)
# define Y(a)               COORD(Y, a)
# define X_PTR(ptr)         X((*(division_scheme_t *)ptr))
# define Y_PTR(ptr)         Y((*(division_scheme_t *)ptr))

typedef division_scheme_t picture_dimensions_t;

typedef uint16_t grid_value_t;
typedef uint64_t grid_level_t;
typedef uint64_t grid_id_t;

typedef int64_t sgrid_level_t;

struct picture {
    picture_dimensions_t dim;
    const grid_value_t *p; /* pic(x, y) = p[x * dim[DS_Y_AXIS] + y],
                            * x = 0 .. dim[DS_X_AXIS]
                            * y = 0 .. dim[DS_Y_AXIS]
                            */
};

typedef enum grid_edge {
    GE_N        = 0x00,
    GE_NE       = 0x01,
    GE_E        = 0x02,
    GE_SE       = 0x03,

    GE_HALF     = 0x04,

    GE_S        = GE_HALF + 0x00,
    GE_SW       = GE_HALF + 0x01,
    GE_W        = GE_HALF + 0x02,
    GE_NW       = GE_HALF + 0x03,

    /* constants */
    GE_MIN      = GE_N,
    GE_MAX      = GE_NW,
    GE_COUNT    = GE_HALF * 2
} grid_edge_t;

# define grid_edge_inverse_val(e)           e##_INVERSE
# define GE_N_INVERSE                       GE_S
# define GE_S_INVERSE                       GE_N
# define GE_W_INVERSE                       GE_E
# define GE_E_INVERSE                       GE_W

# define grid_edge_inverse(e)               ((e + GE_HALF) & GE_MAX)
# define grid_edge_pos_coord(e, pos)        e##_COORD(pos)
# define grid_edge_other_coord(e, pos)      e##_OTHER_COORD(pos)

# define GE_N_COORD                         X
# define GE_S_COORD                         X
# define GE_W_COORD                         Y
# define GE_E_COORD                         Y

# define GE_N_OTHER_COORD                   Y
# define GE_S_OTHER_COORD                   Y
# define GE_W_OTHER_COORD                   X
# define GE_E_OTHER_COORD                   X

# define grid_edge_coord_val(e, pos)        e##_COORD_VAL(pos)
# define GE_N_COORD_VAL(pos)                0
# define GE_S_COORD_VAL(pos)                Y(pos)
# define GE_E_COORD_VAL(pos)                X(pos)
# define GE_W_COORD_VAL(pos)                0

# define grid_edge_other_coord_val(e, pos)  e##_OTHER_COORD_VAL(pos)
# define GE_N_OTHER_COORD_VAL(pos)          Y(pos)
# define GE_S_OTHER_COORD_VAL(pos)          0
# define GE_E_OTHER_COORD_VAL(pos)          X(pos)
# define GE_W_OTHER_COORD_VAL(pos)          0

# define grid_corner_pos(e, pos, ds)            \
do {                                            \
    division_scheme_t tmp = e##_CORNER_POS(ds); \
    pos = tmp;                                  \
} while (0)
# define GE_NE_CORNER_POS(ds)           \
{                                       \
    .v = {                              \
        [COORD_NAME(X)] = X(ds) - 1,    \
        [COORD_NAME(Y)] = 0             \
    }                                   \
}
# define GE_SE_CORNER_POS(ds)           \
{                                       \
    .v = {                              \
        [COORD_NAME(X)] = X(ds) - 1,    \
        [COORD_NAME(Y)] = Y(ds) - 1     \
    }                                   \
}
# define GE_NW_CORNER_POS(ds)           \
{                                       \
    .v = {                              \
        [COORD_NAME(X)] = 0,            \
        [COORD_NAME(Y)] = 0             \
    }                                   \
}
# define GE_SW_CORNER_POS(ds)           \
{                                       \
    .v = {                              \
        [COORD_NAME(X)] = 0,            \
        [COORD_NAME(Y)] = Y(ds) - 1     \
    }                                   \
}



struct grid {
    multigrid_t *host;

    grid_value_t v;

    grid_id_t id;
    grid_t *parent;
    division_scheme_t position;

    picture_dimensions_t from;
    picture_dimensions_t size;

    grid_level_t level;
    bool should_grid;
    bool grided;
    set_t neighbors[GE_COUNT];  /* set of ids */
};

struct multigrid {
    picture_t pic;
    division_scheme_t ds;
    vector_t level_capacity;
    grid_level_t max_level;
    avl_tree_t grids; /* grids by id */
    grid_t *id_0;
};

/** Initialize multigrid structure.
 * \param pic picture description
 * \param ds division scheme
 * \param max_level maximum level. The first undivided grids level.
 */
void multigrid_init(multigrid_t *mg,
                    picture_t pic,
                    division_scheme_t ds,
                    grid_level_t max_level);
void multigrid_purge(multigrid_t *mg);
void multigrid_grid(multigrid_t *mg);

/** Convert grid id to its path
 * \param mg multigrid descriptor
 * \param id grid's id
 * \return \c list_t list with inplace allocation of \c scheme_division_t.
 *         Each element shows grid position inside it's parent grid.
 *         The first element shows position at level 1.
 *         Empty list is returned for zero id.
 */
list_t multigrid_id_to_path(multigrid_t *mg, grid_id_t id);

/** Reverse for \c multigrid_id_to_path
 */
grid_id_t multigrid_path_to_id(multigrid_t *mg, const list_t *path);

#endif /* _MULTIGRID_H_ */
