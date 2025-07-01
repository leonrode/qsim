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

void x(qc_t* qc, int qubit_index) {
    operation_t op;
    op.qubit_indices = (int*) calloc(1, qc->n_qubits * sizeof(int));
    op.qubit_indices[0] = qubit_index;
    op.n_qubit_indices = 1;
    op.gate = &X_gate;
    add_operation(qc, &op);
}

void h(qc_t* qc, int qubit_index) {
    operation_t op;
    op.qubit_indices = (int*) calloc(1, qc->n_qubits * sizeof(int));
    op.qubit_indices[0] = qubit_index;
    op.n_qubit_indices = 1;
    op.gate = &H_gate;
    add_operation(qc, &op);
}

void cx(qc_t* qc, int ctrl_index, int target_index) {

    int swapped = 0;
    if (ctrl_index > target_index) {
        // we add swap gate
        swap(qc, target_index, ctrl_index);
        swapped = 1;
        int t = ctrl_index;
        ctrl_index = target_index;
        target_index = t;
    }


    new_gate(&CX_gate, 1 << (target_index - ctrl_index + 1), "CX");
    build_controlled_single_qubit_gate(&X_gate, 2, 2, ctrl_index, target_index, &CX_gate);

    operation_t CX_op;
    CX_op.gate = &CX_gate;
    CX_op.qubit_indices = malloc(sizeof(int) * 2);
    CX_op.qubit_indices[0] = ctrl_index;
    CX_op.qubit_indices[1] = target_index;
    CX_op.n_qubit_indices = 2;
    add_operation(qc, &CX_op);

    if (swapped) {
        swap(qc, ctrl_index, target_index);
    }
}

// requires qubit_2 > qubit_1
void swap(qc_t* qc, int qubit_1, int qubit_2) {

    new_gate(&SWAP_gate, 1 << (qubit_2 - qubit_1 + 1), "SWAP");
    
    for (int i = 0; i < SWAP_gate.ndim; i++) {
        for (int j = 0; j < SWAP_gate.ndim; j++) {
            SWAP_gate.elements[i][j] = (polar_t) {0, 0};
        }
    }

    build_swap_gate(&SWAP_gate, qubit_1, qubit_2);

    operation_t op;
    op.gate = &SWAP_gate;
    op.qubit_indices = malloc(sizeof(int) * 2);
    op.qubit_indices[0] = qubit_1;
    op.qubit_indices[1] = qubit_2;
    op.n_qubit_indices = 2;
    add_operation(qc, &op);
}

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
        polar_t*** products = malloc((qc->n_qubits) * sizeof(polar_t**));
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
        operation_t* operation = &qc->operations[i];


        // e.g. if operation is X[1] then we take
        // I* X * I ... until we have 2^n sized matrix 
        // we need the first qubit index to know how many I's to multiply
        int first_qubit_index = operation->qubit_indices[0];

        int product_index = 0;

        if (first_qubit_index != 0) {
            // copy I in to product[0]
            copy_matrix(I_gate.elements, products[0], 2, 2);

            for (int j = 1; j < first_qubit_index; j++) {
                kronecker_product(products[j - 1], I_gate.elements, products[j], 1 << (j), 1 << (j), 2, 2);
            }

            product_index = first_qubit_index;
            // product stored in products[product_index - 1]
            // it has size 2^(product_index) * 2^(product_index)
            // the gate has size operation->gate->ndim * operation->gate->ndim
            
            // thus the final size is 2^product_index * operation->gate->ndim
            // thus the final product is stored in products[2^product_index * operation->gate->ndim - 1]

            kronecker_product(products[product_index - 1], operation->gate->elements, products[(int) log2((1 << (product_index)) * operation->gate->ndim - 1)], 1 << (product_index), 1 << (product_index), operation->gate->ndim, operation->gate->ndim);
            product_index = (int) log2((1 << (product_index)) * operation->gate->ndim - 1) + 1;

        } else {
            copy_matrix(operation->gate->elements, products[(int) log2(operation->gate->ndim) - 1], operation->gate->ndim, operation->gate->ndim);
            product_index = (int) log2(operation->gate->ndim);
        }

        // now for the remaining qubits we kronecker the product at products[product_index - 1] with the identity

        for (int j = product_index; j < qc->n_qubits; j++) {
            kronecker_product(products[j - 1], I_gate.elements, products[j], 1 << (j), 1 << (j), 2, 2);
        }


        // build output amps
        polar_t* output_amps = malloc(qc->n_amplitudes * sizeof(polar_t));
        for (int i = 0; i < qc->n_amplitudes; i++) {
            output_amps[i] = (polar_t) {0, 0};
        }

        // now we matrix multiply the product with the amplitudes
        matrix_vector_mult(products[qc->n_qubits - 1], qc->amps, output_amps, qc->n_amplitudes);

        // copy output amps back to qc->amps
        for (int i = 0; i < qc->n_amplitudes; i++) {
            qc->amps[i] = output_amps[i];
        }

        free(output_amps);
        for (int i = 0; i < qc->n_qubits; i++) {
            for (int j = 0; j < (1 << i); j++) {
                free(products[i][j]);
            }
            free(products[i]);
        }
        free(products);
    }


}