#include "fhp1.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    Fhp1Grid grid = {0};

    assert(fhp1_grid_init(&grid, 4, 3) == 0);
    assert(grid.width == 4);
    assert(grid.height == 3);
    assert(grid.cells != NULL);

    /* All cells should start empty. */
    for (int y = 0; y < grid.height; ++y) {
        for (int x = 0; x < grid.width; ++x) {
            Fhp1SiteState *cell = fhp1_grid_at(&grid, x, y);
            assert(cell != NULL);
            assert(*cell == FHP1_STATE_EMPTY);
        }
    }

    /* Verify indexing and independent cell storage. */
    Fhp1SiteState *cell_a = fhp1_grid_at(&grid, 1, 1);
    Fhp1SiteState *cell_b = fhp1_grid_at(&grid, 2, 1);

    assert(cell_a != NULL);
    assert(cell_b != NULL);

    *cell_a = FHP1_DIRECTION_BIT(FHP1_EAST);

    assert(fhp1_has_particle(*cell_a, FHP1_EAST));
    assert(*cell_b == FHP1_STATE_EMPTY);

    /* Verify const access. */
    const Fhp1Grid *const_grid = &grid;
    const Fhp1SiteState *const_cell =
        fhp1_grid_at_const(const_grid, 1, 1);

    assert(const_cell != NULL);
    assert(fhp1_has_particle(*const_cell, FHP1_EAST));

    /* Verify bounds handling. */
    assert(fhp1_grid_at(&grid, -1, 0) == NULL);
    assert(fhp1_grid_at(&grid, 4, 0) == NULL);
    assert(fhp1_grid_at(&grid, 0, -1) == NULL);
    assert(fhp1_grid_at(&grid, 0, 3) == NULL);
    assert(fhp1_grid_at(NULL, 0, 0) == NULL);

    /* Invalid initialization must fail. */
    Fhp1Grid invalid = {0};
    assert(fhp1_grid_init(&invalid, 0, 3) == -1);
    assert(fhp1_grid_init(&invalid, 3, -1) == -1);

    fhp1_grid_destroy(&grid);

    assert(grid.cells == NULL);
    assert(grid.width == 0);
    assert(grid.height == 0);

    /* Destroying an empty grid should be safe. */
    fhp1_grid_destroy(&grid);
    fhp1_grid_destroy(NULL);

    puts("All lattice grid tests passed.");
    return 0;
}