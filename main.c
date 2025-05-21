#include <stdio.h>
#include "gate.h"
#include "state.h"
int main(int argc, char** argv) {

    gate_t gate;
    new_gate(&gate, 2);

    (gate.elements[0][0]) = (polar_t) {1, 0};
    (gate.elements[1][1]) = (polar_t) {1, 0};

    print_gate(&gate);

    state_t state;
    polar_t amplitudes[] = {{2, 2}, {2, 3}};
    init_state(&state, amplitudes);
    print_state(&state);

    return 0;
}
