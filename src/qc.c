#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gate.h"
#include "utils.h"
#include "qc.h"
#include "layer.h"

void init_qc(qc_t* qc, int n_qubits) {
    qc->n_qubits = n_qubits;
    qc->n_layers = 0;
    qc->layers = malloc(MAX_LAYERS * sizeof(layer_t));

    qc->n_amplitudes = pow(2, n_qubits);
    qc->probabilities = calloc(qc->n_qubits, sizeof(int));
    qc->amps = malloc(qc->n_amplitudes * sizeof(polar_t));
    // initialize |00...0> to 1 and all other amplitudes to 0
    qc->amps[0] = (polar_t) {1, 0};
    for (int i = 1; i < qc->n_amplitudes; i++) {
        qc->amps[i] = (polar_t) {0, 0};
    }
}

void free_qc(qc_t* qc) {
    free(qc->amps);
    for (int i = 0; i < qc->n_layers; i++) {
        free_layer(qc->layers[i]);
    }
    free(qc->probabilities);
    free(qc->layers);
    free(qc);
}



void add_operation(qc_t* qc, operation_t* operation) {
    if (qc->n_layers == 0) {
        // create a new layer and add the operation to it
        layer_t* new_layer = malloc(sizeof(layer_t));
        init_layer(new_layer, qc->n_qubits);
        new_layer->operations[0] = operation;
        new_layer->n_operations = 1;
        // add qubits operated on by the operation to the layer
        for (int i = 0; i < operation->n_qubit_indices; i++) {
            new_layer->qubits[operation->qubit_indices[i]] = 1;
        }

        qc->layers[qc->n_layers] = new_layer;
        qc->n_layers++;
    } else {
        int layer_fit_index = -1;

        int start_index = 0;

        // we find the last full layer and then search from there

        for (int i = qc->n_layers - 1; i >= 0; i--) {
            // all qubits in used
            if (layer_full(qc->layers[i])) {
                start_index = i;
                break;
            }
        }

        for (int i = start_index; i < qc->n_layers; i++) {
            if (operation_layer_overlap(operation, qc->layers[i]) == 0) {
                layer_fit_index = i;
                break;
            }
        }

        if (layer_fit_index == -1) {
            // create a new layer and add the operation to it
            layer_t* new_layer = malloc(sizeof(layer_t));
            init_layer(new_layer, qc->n_qubits);
            new_layer->operations[0] = operation;
            new_layer->n_operations = 1;
            // add qubits operated on by the operation to the layer
            for (int i = 0; i < operation->n_qubit_indices; i++) {
                new_layer->qubits[operation->qubit_indices[i]] = 1;
            }
    
            qc->layers[qc->n_layers] = new_layer;
            qc->n_layers++;
        } else {
            // add the operation to the layer
            qc->layers[layer_fit_index]->operations[qc->layers[layer_fit_index]->n_operations] = operation;
            qc->layers[layer_fit_index]->n_operations++;

            // add qubits operated on by the operation to the layer
            for (int i = 0; i < operation->n_qubit_indices; i++) {
                qc->layers[layer_fit_index]->qubits[operation->qubit_indices[i]] = 1;
            }
        }
    }
}


void x(qc_t* qc, int qubit_index) {
    operation_t* op = calloc(1, sizeof(operation_t));
    op->qubit_indices = (int*) calloc(1, qc->n_qubits * sizeof(int));
    op->qubit_indices[0] = qubit_index;
    op->n_qubit_indices = 1;
    op->gate = &X_gate;
    add_operation(qc, op);
}

void h(qc_t* qc, int qubit_index) {
    operation_t* op = calloc(1, sizeof(operation_t));
    op->qubit_indices = (int*) calloc(1, qc->n_qubits * sizeof(int));
    op->qubit_indices[0] = qubit_index;
    op->n_qubit_indices = 1;
    op->gate = &H_gate;
    add_operation(qc, op);
}

void z(qc_t* qc, int qubit_index) {
    operation_t* op = calloc(1, sizeof(operation_t));
    op->qubit_indices = (int*) calloc(1, qc->n_qubits * sizeof(int));
    op->qubit_indices[0] = qubit_index;
    op->n_qubit_indices = 1;
    op->gate = &Z_gate;
    add_operation(qc, op);
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

    operation_t* CX_op = calloc(1, sizeof(operation_t));
    CX_op->gate = &CX_gate;
    CX_op->qubit_indices = malloc(sizeof(int) * (target_index - ctrl_index + 1));
    for (int i = ctrl_index; i <= target_index; i++) {
        CX_op->qubit_indices[i - ctrl_index] = i;
    }
    CX_op->n_qubit_indices = (target_index - ctrl_index + 1);
    add_operation(qc, CX_op);

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

    operation_t* op = calloc(1, sizeof(operation_t));
    op->gate = &SWAP_gate;
    op->qubit_indices = malloc(sizeof(int) * (qubit_2 - qubit_1 + 1));
    for (int i = qubit_1; i <= qubit_2; i++) {
        op->qubit_indices[i - qubit_1] = i;
    }
    op->n_qubit_indices = (qubit_2 - qubit_1 + 1);
    add_operation(qc, op);
}

void rx(qc_t* qc, int qubit_index, double theta) {
    operation_t* op = calloc(1, sizeof(operation_t));
    op->gate = &RX_gate;
    new_gate(&RX_gate, 2, "RX");
    build_rx_gate(&RX_gate, theta);
    print_gate(&RX_gate);
    op->qubit_indices = malloc(sizeof(int) * 1);
    op->qubit_indices[0] = qubit_index;
    op->n_qubit_indices = 1;
    add_operation(qc, op);
}

void ry(qc_t* qc, int qubit_index, double theta) {
    operation_t* op = calloc(1, sizeof(operation_t));
    op->gate = &RY_gate;
    new_gate(&RY_gate, 2, "RY");
    build_ry_gate(&RY_gate, theta);
    op->qubit_indices = malloc(sizeof(int) * 1);
    op->qubit_indices[0] = qubit_index;
    op->n_qubit_indices = 1;
    add_operation(qc, op);
}

void rz(qc_t* qc, int qubit_index, double theta) {
    operation_t* op = calloc(1, sizeof(operation_t));
    op->gate = &RZ_gate;
    new_gate(&RZ_gate, 2, "RZ");
    build_rz_gate(&RZ_gate, theta);
    op->qubit_indices = malloc(sizeof(int) * 1);
    op->qubit_indices[0] = qubit_index;
    op->n_qubit_indices = 1;
    add_operation(qc, op);
}

void print_qc_amplitudes(qc_t* qc) {
    for (int i = 0; i < qc->n_amplitudes; i++) {
        printf("|%d> = %f exp(%f i)\n", i, qc->amps[i].r, qc->amps[i].theta);
    }
}

void run_qc(qc_t* qc) {

    // iterate through the layers and apply the operations in order
    for (int i = 0; i < qc->n_layers; i++) {
        // for each qubit in the layer we multiply either the operation that exists 
        // or the identiy matrix 

        int qubit_index = 0;

        polar_t** product = NULL;

        int product_dim = 1; // product is currently product_dim * product_dim

        // printf("Layer %d:\n", i);

        while (qubit_index < qc->n_qubits) {
            if (qc->layers[i]->qubits[qubit_index] == 1) {

                // find the operation that operates on qubit_index
                operation_t* operation = NULL;
                for (int j = 0; j < qc->layers[i]->n_operations; j++) {
                    for (int k = 0; k < qc->layers[i]->operations[j]->n_qubit_indices; k++) {
                        if (qc->layers[i]->operations[j]->qubit_indices[k] == qubit_index) {
                            operation = qc->layers[i]->operations[j];
                            break;
                        }
                    }
                }
                // printf("Operation: %s on qubit %d\n", operation->gate->name, qubit_index);
                if (operation != NULL) { // should never be null
                    // if product is null, we set product to the operation matrix
                    if (product == NULL) {
                        // product = operation->gate->elements;
                        product = malloc(operation->gate->ndim * sizeof(polar_t*));
                        for (int k = 0; k < operation->gate->ndim; k++) {
                            product[k] = malloc(operation->gate->ndim * sizeof(polar_t));
                        }
                        copy_matrix(operation->gate->elements, product, operation->gate->ndim, operation->gate->ndim);
                        // printf("Copied into product\n");
                        product_dim = operation->gate->ndim;
                        qubit_index += operation->n_qubit_indices;
                    }  else {
                        // product is not null and we multiply the product with the operation
                        // printf("Multiplying product with operation\n");

                        polar_t** new_product;
                        kronecker_product(product, operation->gate->elements, &new_product, product_dim, product_dim, operation->gate->ndim, operation->gate->ndim);
                        free(product); // free the old product before setting it to the new pointer
                        product = new_product;
                        product_dim *= operation->gate->ndim;
                        qubit_index += operation->n_qubit_indices;
                    }
                }
            } else {
                // printf("No operation on qubit %d\n", qubit_index);
                if (product == NULL){
                    // printf("Setting product to identity\n");
                    product = malloc(2 * sizeof(polar_t*));
                    for (int k = 0; k < 2; k++) {
                        product[k] = malloc(2 * sizeof(polar_t));
                    }

                    copy_matrix(I_gate.elements, product, 2, 2);
                    product_dim = 2;
                    qubit_index++;
                } else {

                    // multiply the product by the identity matrix
                    polar_t** new_product;
                    kronecker_product(product, I_gate.elements, &new_product, product_dim, product_dim, 2, 2);
                    free(product); // free the old product before setting it to the new pointer
                    product = new_product;
                    product_dim *= 2;
                    qubit_index++;
                }

            }            
        }
        // multiply the amps by the product
        polar_t* new_amps = calloc(product_dim, sizeof(polar_t));

        for (int i = 0; i < product_dim; i++) {
            new_amps[i] = (polar_t) {qc->amps[i].r, qc->amps[i].theta};

        }
        // printf("Doing matrix vector mult\n");
        matrix_vector_mult(product, qc->amps, new_amps, product_dim);
        // printf("New amps:\n");
        // copy new_amps to qc->amps
        for (int i = 0; i < product_dim; i++) {
            qc->amps[i] = (polar_t) {new_amps[i].r, new_amps[i].theta};
        }

        free(new_amps);
        free(product);
    }
}

void print_qc(qc_t* qc) {
    // print based on layers

    for (int i = 0; i < qc->n_qubits; i++) { // n qubits lines of output
        printf("|0>--");

        for (int j = 0; j < qc->n_layers; j++) {
            if (qc->layers[j]->qubits[i] == 1) {
                // find the operation that operates on qubit i
                char* gate_name = "X"; // default fallback
                for (int k = 0; k < qc->layers[j]->n_operations; k++) {
                    for (int l = 0; l < qc->layers[j]->operations[k]->n_qubit_indices; l++) {
                        if (qc->layers[j]->operations[k]->qubit_indices[l] == i) {
                            gate_name = qc->layers[j]->operations[k]->gate->name;
                            break;
                        }
                    }
                }
                printf("%s---", gate_name);
            } else {
                printf("----");
            }
        }

        printf("\n");

    }
}

void print_qc_layers(qc_t* qc) {
    for (int i = 0; i < qc->n_layers; i++) {
        printf("Layer %d:\n", i);
        for (int j = 0; j < qc->layers[i]->n_operations; j++) {
            printf("Operation %d: %s\n", j, qc->layers[i]->operations[j]->gate->name);
        }
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

float _probability_of_qubit(qc_t* qc, int qubit_index) {

    // for a given qubit we sum the squares of the amplitudes of the states that have a 1 in the qubit
    float sum = 0;
    for (int i = 0; i < qc->n_amplitudes; i++) {
        if (i & (1 << (qc->n_qubits - qubit_index - 1))) {
            sum += qc->amps[i].r * qc->amps[i].r;
        }
    }
    return sum;
}


void print_qc_probabilities(qc_t* qc) {
    for (int i = 0; i < qc->n_qubits; i++) {
        qc->probabilities[i] = _probability_of_qubit(qc, i);
        printf("Probability of qubit %d being 1: %f\n", i, qc->probabilities[i]);
    }
}