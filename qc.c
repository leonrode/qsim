#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "qc.h"

void init_qc(qc_t* qc, int n_qubits) {
    qc->n_qubits = n_qubits;
    qc->n_operations = 0;
    qc->operations = malloc(MAX_OPERATIONS * sizeof(operation_t));
    qc->states = malloc(n_qubits * sizeof(state_t));
    for (int i = 0; i < n_qubits; i++) {
        init_state(&qc->states[i], (polar_t[]) { {1, 0}, {0, 0}}); // init states to zero (polar 1e^0 and 0e^0);
    }
}

void add_operation(qc_t* qc, operation_t* operation) {
    qc->operations[qc->n_operations] = *operation;
    qc->n_operations++;
}

void x(qc_t* qc, int qubit_index) {
    operation_t op;
    op.qubit_index = qubit_index;
    op.gate = &X_gate;
    add_operation(qc, &op);
}

void h(qc_t* qc, int qubit_index) {
    operation_t op;
    op.qubit_index = qubit_index;
    op.gate = &H_gate;
    add_operation(qc, &op);
}



void run_qc(qc_t* qc) {
    for (int i = 0; i < qc->n_operations; i++) {
        operation_t* operation = &qc->operations[i];
        printf("Applying gate to qubit %d\n", operation->qubit_index);
        apply_gate_to_state(operation->gate, &qc->states[operation->qubit_index]);
    }
}

void print_qc_states(qc_t* qc) {
    for (int i = 0; i < qc->n_qubits; i++) {
        print_state(&qc->states[i]);
    }
}