#include <stdio.h>
#include "gate.h"
#include "state.h"
#include "math.h"
int main(int argc, char** argv) {

    gate_t I_gate;
    new_gate(&I_gate, 2);
    (I_gate.elements[0][0]) = (polar_t) {1, 0};
    (I_gate.elements[1][1]) = (polar_t) {1, 0};
    
    gate_t X_gate;
    new_gate(&X_gate, 2);
    (X_gate.elements[0][1]) = (polar_t) {1, 0};
    (X_gate.elements[1][0]) = (polar_t) {1, 0};

    gate_t H_gate;
    new_gate(&H_gate, 2);
    (H_gate.elements[0][0]) = (polar_t) {1/sqrt(2), 0};
    (H_gate.elements[0][1]) = (polar_t) {1/sqrt(2), 0};
    (H_gate.elements[1][0]) = (polar_t) {1/sqrt(2), 0};
    (H_gate.elements[1][1]) = (polar_t) {-1/sqrt(2), 0};

    state_t state;
    polar_t amplitudes[] = {{1, 0}, {0, 0}};
    init_state(&state, amplitudes);
    print_state(&state);
    
    apply_gate_to_state(&I_gate, &state);
    print_state(&state);

    apply_gate_to_state(&X_gate, &state);
    print_state(&state);

    apply_gate_to_state(&H_gate, &state);
    print_state(&state);


    return 0;
}
