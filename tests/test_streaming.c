#include "fhp1.h"

#include <assert.h>
#include <stdio.h>

static int particle_count(const Fhp1Grid *grid)
{
    int count = 0;

    for (int y = 0; y < grid->height; ++y) {
        for (int x = 0; x < grid->width; ++x) {
            Fhp1SiteState state =
                *fhp1_grid_at_const(grid, x, y);

            for (int d = 0; d < FHP1_NUM_DIRECTIONS; ++d) {
                count += fhp1_has_particle(state, d);
            }
        }
    }

    return count;
}

int main(void)
{
    Fhp1Grid source = {0};
    Fhp1Grid destination = {0};

    assert(fhp1_grid_init(&source, 5, 4) == 0);
    assert(fhp1_grid_init(&destination, 5, 4) == 0);

    /* All six directions from one site. */
    *fhp1_grid_at(&source, 2, 1) = FHP1_STATE_ALL_DIRECTIONS;

    assert(fhp1_grid_stream(&source, &destination) == 0);
    assert(particle_count(&source) == 6);
    assert(particle_count(&destination) == 6);

    for (int d = 0; d < FHP1_NUM_DIRECTIONS; ++d) {
        int x = (2 + fhp1_dq[d]) % source.width;
        int y = (1 + fhp1_dr[d]) % source.height;

        if (x < 0) x += source.width;
        if (y < 0) y += source.height;

        assert(fhp1_has_particle(
            *fhp1_grid_at(&destination, x, y), d));
    }

    /* Source must remain unchanged. */
    assert(*fhp1_grid_at(&source, 2, 1) ==
           FHP1_STATE_ALL_DIRECTIONS);

    /* Start an independent periodic-boundary test. */
    for (int y = 0; y < source.height; ++y) {
        for (int x = 0; x < source.width; ++x) {
            *fhp1_grid_at(&source, x, y) = FHP1_STATE_EMPTY;
        }
    }

    /* East particle wraps from the right edge to the left. */
    *fhp1_grid_at(&source, 4, 1) =
        FHP1_DIRECTION_BIT(FHP1_EAST);

    assert(fhp1_grid_stream(&source, &destination) == 0);
    assert(particle_count(&destination) == 1);
    assert(fhp1_has_particle(
        *fhp1_grid_at(&destination, 0, 1), FHP1_EAST));

    /* Clear the source grid before the next independent scenario. */
    for (int y = 0; y < source.height; ++y) {
        for (int x = 0; x < source.width; ++x) {
            *fhp1_grid_at(&source, x, y) = FHP1_STATE_EMPTY;
        }
    }

    *fhp1_grid_at(&source, 0, 0) =
        FHP1_DIRECTION_BIT(FHP1_SOUTH_WEST);

    assert(fhp1_grid_stream(&source, &destination) == 0);
    assert(particle_count(&destination) == 1);
    assert(fhp1_has_particle(
        *fhp1_grid_at(&destination, 0, 3), FHP1_SOUTH_WEST));

    /* Invalid calls must fail. */
    assert(fhp1_grid_stream(NULL, &destination) == -1);
    assert(fhp1_grid_stream(&source, NULL) == -1);
    assert(fhp1_grid_stream(&source, &source) == -1);

    fhp1_grid_destroy(&source);
    fhp1_grid_destroy(&destination);

    puts("All streaming tests passed.");
    return 0;
}