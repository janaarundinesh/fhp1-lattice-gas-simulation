#include "fhp1.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    Fhp1SiteState state = FHP1_STATE_EMPTY;

    assert(state == 0);

    state |= FHP1_DIRECTION_BIT(FHP1_EAST);
    state |= FHP1_DIRECTION_BIT(FHP1_NORTH_WEST);

    assert(fhp1_has_particle(state, FHP1_EAST));
    assert(fhp1_has_particle(state, FHP1_NORTH_WEST));
    assert(!fhp1_has_particle(state, FHP1_WEST));

    assert(!fhp1_has_particle(state, -1));
    assert(!fhp1_has_particle(state, FHP1_NUM_DIRECTIONS));

    assert((state & FHP1_STATE_ALL_DIRECTIONS) == state);

    printf("All lattice state tests passed.\n");
    return 0;
}