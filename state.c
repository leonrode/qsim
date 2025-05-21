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

