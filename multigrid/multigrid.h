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

# define COORD(name, a)     (a).v[DS_##name##_AXIS]
# define X(a)               COORD(X, a)
# define Y(a)               COORD(Y, a)
# define X_PTR(ptr)         X(*(division_scheme_t *)ptr)
# define Y_PTR(ptr)         Y(*(division_scheme_t *)ptr)

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
    set_t neighbors;    /* set of ids */
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
