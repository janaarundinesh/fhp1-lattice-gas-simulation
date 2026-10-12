#include "fhp1.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

int fhp1_grid_init(Fhp1Grid *grid, int width, int height)
{
    if (grid == NULL || width <= 0 || height <= 0) {
        return -1;
    }

    grid->width = 0;
    grid->height = 0;
    grid->cells = NULL;

    if ((size_t)width > SIZE_MAX / (size_t)height) {
        return -1;
    }

    size_t cell_count = (size_t)width * (size_t)height;

    if (cell_count > SIZE_MAX / sizeof(Fhp1SiteState)) {
        return -1;
    }

    Fhp1SiteState *cells = calloc(cell_count, sizeof(Fhp1SiteState));

    if (cells == NULL) {
        return -1;
    }

    grid->width = width;
    grid->height = height;
    grid->cells = cells;

    return 0;
}

void fhp1_grid_destroy(Fhp1Grid *grid)
{
    if (grid == NULL) {
        return;
    }

    free(grid->cells);

    grid->width = 0;
    grid->height = 0;
    grid->cells = NULL;
}

Fhp1SiteState *fhp1_grid_at(Fhp1Grid *grid, int x, int y)
{
    if (grid == NULL || grid->cells == NULL ||
        x < 0 || x >= grid->width ||
        y < 0 || y >= grid->height) {
        return NULL;
    }

    return &grid->cells[(size_t)y * (size_t)grid->width + (size_t)x];
}

const Fhp1SiteState *fhp1_grid_at_const(
    const Fhp1Grid *grid,
    int x,
    int y
)
{
    if (grid == NULL || grid->cells == NULL ||
        x < 0 || x >= grid->width ||
        y < 0 || y >= grid->height) {
        return NULL;
    }

    return &grid->cells[(size_t)y * (size_t)grid->width + (size_t)x];
}