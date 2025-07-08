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
    build_controlled_single_qubit_gate(&X_gate,ctrl_index, target_index, &CX_gate);

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

void rx(qc_t* qc, int qubit_index, double theta) {
    operation_t op;
    op.gate = &RX_gate;
    new_gate(&RX_gate, 2, "RX");
    build_rx_gate(&RX_gate, theta);
    print_gate(&RX_gate);
    op.qubit_indices = malloc(sizeof(int) * 1);
    op.qubit_indices[0] = qubit_index;
    op.n_qubit_indices = 1;
    add_operation(qc, &op);
}

void ry(qc_t* qc, int qubit_index, double theta) {
    operation_t op;
    op.gate = &RY_gate;
    new_gate(&RY_gate, 2, "RY");
    build_ry_gate(&RY_gate, theta);
    op.qubit_indices = malloc(sizeof(int) * 1);
    op.qubit_indices[0] = qubit_index;
    op.n_qubit_indices = 1;
    add_operation(qc, &op);
}

void rz(qc_t* qc, int qubit_index, double theta) {
    operation_t op;
    op.gate = &RZ_gate;
    new_gate(&RZ_gate, 2, "RZ");
    build_rz_gate(&RZ_gate, theta);
    op.qubit_indices = malloc(sizeof(int) * 1);
    op.qubit_indices[0] = qubit_index;
    op.n_qubit_indices = 1;
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

    for (int i = 0; i < qc->n_operations; i++) {

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

        // e.g. if operation is X[1] then we take I* X * I ... until we have 2^n sized matrix 
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
            for (int j = 0; j < (1 << (i + 1)); j++) {
                free(products[i][j]);
            }
            free(products[i]);
        }
        free(products);
    }

    _remove_global_phase(qc);

}

void print_qc(qc_t* qc) {
    // for each qubit we iterate through the operations
    for (int i = 0; i < qc->n_qubits; i++) {
        printf("|0>-");
        for (int j = 0; j < qc->n_operations; j++) {
            operation_t* operation = &qc->operations[j];
            if (operation->qubit_indices[0] == i) {
                if (operation->gate->name[0] == 'C') {
                    printf("•---");
                } else {
                    printf("%s---", operation->gate->name);
                }
            } else if (operation->n_qubit_indices == 2 && operation->qubit_indices[1] == i) {
                printf("%c---", operation->gate->name[1]);
            } else {
                printf("----");
            }
        }
        printf("\n\n");
    }
}

void _remove_global_phase(qc_t* qc) {
    // we need to find the first non-zero amplitude
    for (int i = 0; i < qc->n_amplitudes; i++) {
        if (qc->amps[i].r != 0) {

            double phase = qc->amps[i].theta;
            // we need to remove the global phase
            for (int j = 0; j < qc->n_amplitudes; j++) {
                qc->amps[j].theta -= phase;
            }
            break;
        }
    }
}