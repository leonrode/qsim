#include "gate.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Initialize gate struct with n dimensions
 */
void new_gate(gate_t* gate, int n) {
    gate->ndim = n;
    gate->elements = malloc(n * sizeof(complex_t*));
    
    for (int i = 0; i < n; i++) {
        gate->elements[i] = malloc(n * sizeof(complex_t));
    }
}

void print_gate(gate_t* gate) {

    for (int r = 0; r < gate->ndim; r++) {
        polar_t* row = gate->elements[r];

        for (int c = 0; c < gate->ndim; c++) {
            printf("(%f)exp([%f]i) ", (row + c)->r, (row + c)->theta);
        }
        printf("\n");
    }
}

void apply_gate_to_state(gate_t* gate, state_t* state) {

    polar_t** e = gate->elements;
    polar_t* a = state->amplitudes;
    polar_t a_prior = {a[0].r, a[0].theta};
    // add(mult(e[0][0], a[0]), mult(e[0][1], a[1]))
    a[0] = polar_add(polar_mult(e[0][0], a[0]), polar_mult(e[0][1], a[1]));
    // add(mult(e[1][0], a[0]), mult(e[1][1], a[1]))
    a[1] = polar_add(polar_mult(e[1][0], a_prior), polar_mult(e[1][1], a[1]));
}