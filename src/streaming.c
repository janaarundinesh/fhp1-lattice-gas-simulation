#include "fhp1.h"

#include <stddef.h>
#include <string.h>

int fhp1_grid_stream(
    const Fhp1Grid *source,
    Fhp1Grid *destination
)
{
    if (source == NULL || destination == NULL ||
        source->cells == NULL || destination->cells == NULL ||
        source->width <= 0 || source->height <= 0 ||
        source->width != destination->width ||
        source->height != destination->height ||
        source->cells == destination->cells) {
        return -1;
    }

    const size_t width = (size_t)source->width;
    const size_t height = (size_t)source->height;
    const size_t cell_count = width * height;

    /* Reject invalid occupancy bits before modifying the destination. */
    for (size_t i = 0; i < cell_count; ++i) {
        if ((source->cells[i] &
             (Fhp1SiteState)~FHP1_STATE_ALL_DIRECTIONS) != 0) {
            return -1;
        }
    }

    memset(destination->cells, 0,
           cell_count * sizeof(Fhp1SiteState));

    for (int y = 0; y < source->height; ++y) {
        for (int x = 0; x < source->width; ++x) {
            const Fhp1SiteState state =
                source->cells[(size_t)y * width + (size_t)x];

            for (int direction = 0;
                 direction < FHP1_NUM_DIRECTIONS;
                 ++direction) {

                if (!fhp1_has_particle(state, direction)) {
                    continue;
                }

                int next_x =
                    (x + fhp1_dq[direction]) % source->width;
                int next_y =
                    (y + fhp1_dr[direction]) % source->height;

                if (next_x < 0) {
                    next_x += source->width;
                }
                if (next_y < 0) {
                    next_y += source->height;
                }

                destination->cells[
                    (size_t)next_y * width + (size_t)next_x
                ] |= FHP1_DIRECTION_BIT(direction);
            }
        }
    }

    return 0;
}