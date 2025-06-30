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

    // now we iterate through the operations
    for (int i = 0; i < qc->n_operations; i++) {

        /*
         * products[0] is 2x2
         * products[1] is 4x4
         * products[2] is 8x8
         * ...
         * products[n-1] is 2^n x 2^n
         */
        polar_t*** products = malloc((qc->n_qubits - 1) * sizeof(polar_t**));
        for (int i = 1; i <= qc->n_qubits; i++) {
            products[i - 1] = malloc((1 << i) * sizeof(polar_t*));
            // init the product matrices
            for (int j = 0; j < (1 << i); j++) {
                products[i - 1][j] = malloc((1 << i) * sizeof(polar_t));
                for (int k = 0; k < (1 << i); k++) {
                    products[i - 1][j][k] = (polar_t) {0, 0};
                }
            }
        }
        printf("built products\n");
        operation_t* operation = &qc->operations[i];
        printf("operation: %s with dimension %d\n", operation->gate->name, operation->gate->ndim);
        // e.g. if operation is X[1] then we take
        // I* X * I ... until we have 2^n sized matrix 
        // we need the first qubit index to know how many I's to multiply 
        int first_qubit_index = operation->qubit_indices[0];
        int product_index = 0;

        if (first_qubit_index == 0) {
            // copy the gate into the proper product 
            // whose index is given by log(gate dimension)

            printf("%d %d\n", operation->gate->ndim, (int) log2(operation->gate->ndim));

            copy_matrix(operation->gate->elements, products[(int) log2(operation->gate->ndim)], operation->gate->ndim, operation->gate->ndim);
            product_index += (int) log2(operation->gate->ndim) + 1;
        } else if (first_qubit_index == 1) {
            // copy the identity into the first product 
            copy_matrix(I_gate.elements, products[0], 2, 2);
            product_index++;

            // now we kronecker the gate with the identity
            kronecker_product(products[0], operation->gate->elements, products[product_index], 2, 2, operation->gate->ndim, operation->gate->ndim);
            product_index += (int) log2(operation->gate->ndim * 2);
        } else {
            
        }

        
        

        




    }


}