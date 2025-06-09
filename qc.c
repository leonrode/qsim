#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "qc.h"

void init_qc(qc_t* qc, int n_qubits) {
    qc->n_qubits = n_qubits;
    qc->n_operations = 0;
    qc->operations = malloc(MAX_OPERATIONS * sizeof(operation_t));
    qc->n_amplitudes = pow(2, n_qubits);
    qc->amps = malloc(qc->n_amplitudes * sizeof(polar_t));
    // initialize |00...0> to 1 and all other amplitudes to 0
    qc->amps[0] = (polar_t) {1, 0};
    for (int i = 1; i < qc->n_amplitudes; i++) {
        qc->amps[i] = (polar_t) {0, 0};
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

void print_qc_operations(qc_t* qc) {
    for (int i = 0; i < qc->n_operations; i++) {
        operation_t* operation = &qc->operations[i];
        printf("Applying gate %s to qubit %d\n", operation->gate->name, operation->qubit_index);
    }
}

void print_qc_amplitudes(qc_t* qc) {
    for (int i = 0; i < qc->n_amplitudes; i++) {
        printf("|%d> = %f exp(%f i)\n", i, qc->amps[i].r, qc->amps[i].theta);
    }
}
