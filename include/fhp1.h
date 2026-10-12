#ifndef FHP1_H
#define FHP1_H
#include <stdint.h>

/*
 * FHP-1 lattice gas simulation
 *
 * The lattice uses integer axial coordinates (q, r).
 * The six directions are ordered counterclockwise.
 */

#define FHP1_NUM_DIRECTIONS 6

typedef enum {
    FHP1_EAST = 0,
    FHP1_NORTH_EAST,
    FHP1_NORTH_WEST,
    FHP1_WEST,
    FHP1_SOUTH_WEST,
    FHP1_SOUTH_EAST
} Fhp1Direction;

typedef uint8_t Fhp1SiteState;

#define FHP1_STATE_EMPTY ((Fhp1SiteState)0)

#define FHP1_DIRECTION_BIT(direction) \
    ((Fhp1SiteState)(1u << (direction)))

#define FHP1_STATE_ALL_DIRECTIONS ((Fhp1SiteState)0x3Fu)

static inline int fhp1_has_particle(
    Fhp1SiteState state,
    int direction
) {
    if (direction < 0 || direction >= FHP1_NUM_DIRECTIONS) {
        return 0;
    }

    return (state & FHP1_DIRECTION_BIT(direction)) != 0;
}

/*
 * Integer lattice-coordinate offsets for each direction.
 *
 * A neighbor is located at:
 *   q_next = q + dq[direction]
 *   r_next = r + dr[direction]
 */
extern const int fhp1_dq[FHP1_NUM_DIRECTIONS];
extern const int fhp1_dr[FHP1_NUM_DIRECTIONS];

/*
 * Return the opposite direction.
 * Return -1 if the direction is invalid.
 */
int fhp1_opposite_direction(int direction);

typedef struct {
    int width;
    int height;
    Fhp1SiteState *cells;
} Fhp1Grid;

int fhp1_grid_init(Fhp1Grid *grid, int width, int height);
void fhp1_grid_destroy(Fhp1Grid *grid);

Fhp1SiteState *fhp1_grid_at(
    Fhp1Grid *grid,
    int x,
    int y
);

const Fhp1SiteState *fhp1_grid_at_const(
    const Fhp1Grid *grid,
    int x,
    int y
);

#endif