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