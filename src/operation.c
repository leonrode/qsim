#include "operation.h"
#include <stdlib.h>
#include <stdio.h>

void free_operation(operation_t* operation) {
    free(operation->qubit_indices);
    free_gate(operation->gate);
    free(operation->gate);
}

void copy_operation(operation_t* src, operation_t* dest) {
    dest->gate = malloc(sizeof(gate_t));
    copy_gate(src->gate, dest->gate);
    dest->qubit_indices = calloc(src->n_qubit_indices, sizeof(int));
    for (int i = 0; i < src->n_qubit_indices; i++) {
        dest->qubit_indices[i] = src->qubit_indices[i];
    }
    dest->n_qubit_indices = src->n_qubit_indices;
}