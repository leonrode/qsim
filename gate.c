#include "gate.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Initialize gate struct with n dimensions i.e. n x n matrix
 */
void new_gate(gate_t* gate, int n, char* name) {
    gate->ndim = n;
    gate->elements = malloc(n * sizeof(complex_t*));
    gate->name = name;
    for (int i = 0; i < n; i++) {
        gate->elements[i] = malloc(n * sizeof(complex_t));
    }
}

void print_gate(gate_t* gate) {
    printf("Gate: %s\n", gate->name);
    for (int r = 0; r < gate->ndim; r++) {
        polar_t* row = gate->elements[r];

        for (int c = 0; c < gate->ndim; c++) {
            printf("(%f)exp([%f]i) ", (row + c)->r, (row + c)->theta);
        }
        printf("\n");
    }
}


void fill_with_zeros(gate_t* gate) {
    for (int i = 0; i < gate->ndim; i++) {
        for (int j = 0; j < gate->ndim; j++) {
            (gate->elements[i][j]) = (polar_t) {0, 0};
        }
    }
}