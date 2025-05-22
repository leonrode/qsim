#include <string.h>
#include <stdio.h>
#include "state.h"
#include "utils.h"
void init_state(state_t* state, polar_t amplitudes[2]) {
    memcpy(state->amplitudes, amplitudes, 2 * sizeof(polar_t));
}
void print_state(state_t* state) {
    printf("alpha: ");
    print_polar(state->amplitudes[0]);
    printf("beta: ");
    print_polar(state->amplitudes[1]);
}


bool_t states_equal(state_t* a, state_t* b) {
    // deeper than just a memcmp
    if (a == NULL || b == NULL) return FALSE;

    printf("%d %d\n", polar_equal(a->amplitudes[0], b->amplitudes[0]), polar_equal(a->amplitudes[1], b->amplitudes[1]));
    // comparing amplitudes
    return polar_equal(a->amplitudes[0], b->amplitudes[0]) && polar_equal(a->amplitudes[1], b->amplitudes[1]);
}
 