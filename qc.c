#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gate.h"
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

// void x(qc_t* qc, int qubit_index) {
//     operation_t op;
//     op.qubit_indices = (int*) calloc(1, qc->n_qubits * sizeof(int));
//     op.qubit_indices[qubit_index] = 1;
//     op.gate = &X_gate;
//     add_operation(qc, &op);
// }

// void h(qc_t* qc, int qubit_index) {
//     operation_t op;
//     op.qubit_indices = (int*) calloc(1, qc->n_qubits * sizeof(int));
//     op.qubit_indices[qubit_index] = 1;
//     op.gate = &H_gate;
//     add_operation(qc, &op);
// }

// void cx(qc_t* qc, int ctrl_index, int target_index) {
//     operation_t op;
//     op.qubit_indices = (int*) calloc(1, qc->n_qubits * sizeof(int)); // calloc to init 0
//     op.qubit_indices[ctrl_index] = -1;
//     op.qubit_indices[target_index] = 1;

//     // we have to build this one out on the fly
//     gate_t* CX_gate = malloc(sizeof(gate_t));
//     new_gate(CX_gate, qc->n_amplitudes, "CX");

//     // init with zeros.
//     fill_with_zeros(CX_gate);

//     for (int i = 0; i < qc->n_amplitudes; i++) { // row major order
//         // we have to check if the ctrl_index'th bit in i is set
//         if (i >> ctrl_index & 1) {
//             // then we flip the target_index'th bit in i
//             int flipped_i = i ^ (1 << target_index);
//             (CX_gate->elements[i][flipped_i]) = (polar_t) {1, 0};
//         } else {
//             (CX_gate->elements[i][i]) = (polar_t) {1, 0};
//         }
//     }

//     op.gate = CX_gate;
//     add_operation(qc, &op);
// }

void print_qc_operations(qc_t* qc) {
    for (int i = 0; i < qc->n_operations; i++) {
        operation_t* operation = &qc->operations[i];
        printf("Applying gate %s to qubits: ", operation->gate->name);
        for (int j = 0; j < operation->n_qubit_indices; j++) {
            printf("%d ", operation->qubit_indices[j]);
        }
        printf("\n");
    }
}

void print_qc_amplitudes(qc_t* qc) {
    for (int i = 0; i < qc->n_amplitudes; i++) {
        printf("|%d> = %f exp(%f i)\n", i, qc->amps[i].r, qc->amps[i].theta);
    }
}

void run_qc(qc_t* qc) {
    // we analyze the layers of the qc
    // in this first iteration we simply 
    // kronecker product the gates with appropraite I matrices
    // until they are the size of the qc

    // init product representations of size
    // 4, 8, 16, ... until 2^n qubits
    polar_t*** products = malloc(qc->n_qubits * sizeof(polar_t**));
    for (int i = 2; i <= qc->n_qubits; i++) {
        products[i] = malloc((1 << i) * sizeof(polar_t*));
        // init the product matrices
        for (int j = 0; j < (1 << i); j++) {
            products[i][j] = malloc((1 << i) * sizeof(polar_t));
            for (int k = 0; k < (1 << i); k++) {
                products[i][j][k] = (polar_t) {0, 0};
            }
        }
    }

    // now we iterate through the operations
    for (int i = 0; i < qc->n_operations; i++) {
        operation_t* operation = &qc->operations[i];
        // e.g. if operation is X[1] then we take
        // I * X * I ... until we have 2^n sized matrix
        
    }


}