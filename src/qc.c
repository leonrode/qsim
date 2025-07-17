#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gate.h"
#include "utils.h"
#include "qc.h"
#include "layer.h"
#include <string.h>

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
        free(qc->layers[i]);
    }
    free(qc->probabilities);
    free(qc->layers);
    free(qc);
}

void adjoint_qc(qc_t* qc_in, qc_t* qc_out) {

    // we copy everything in reverse order
    for (int i = qc_in->n_layers - 1; i >= 0; i--) {
        // we need to copy the layer to the output qc
        layer_t* new_layer = malloc(sizeof(layer_t));
        init_layer(new_layer, qc_in->n_qubits);
        for (int j = 0; j < qc_in->layers[i]->n_operations; j++) {
            operation_t* new_operation = malloc(sizeof(operation_t));
            copy_operation(qc_in->layers[i]->operations[j], new_operation);
            add_operation(qc_out, new_operation);
        }
    }
}

void append_qc(qc_t* src, qc_t* dest) {
    for (int i = 0; i < src->n_layers; i++) {
        for (int j = 0; j < src->layers[i]->n_operations; j++) {
            operation_t* new_operation = malloc(sizeof(operation_t));
            copy_operation(src->layers[i]->operations[j], new_operation);
            add_operation(dest, new_operation);
        }
    }
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
            if (operation->qubit_indices[i] == 1) {
                new_layer->qubits[i] = 1;
            }
        }


        qc->layers[qc->n_layers] = new_layer;
        qc->n_layers++;
    } else {
        int layer_fit_index = -1;

        int start_index = 0;

        // we find the last overlap layer and search from there

        for (int i = qc->n_layers - 1; i >= 0; i--) {

            if (operation_layer_overlap(operation, qc->layers[i]) == 1) {
                start_index = i;
                break;
            }
        }

        // printf("start_index: %d\n", start_index);

        for (int i = start_index; i < qc->n_layers; i++) {
            if (operation_layer_overlap(operation, qc->layers[i]) == 0) {
                layer_fit_index = i;
                break;
            }
        }

        // printf("layer_fit_index: %d for operation: %s\n", layer_fit_index, operation->gate->name);

        if (layer_fit_index == -1) {
            // create a new layer and add the operation to it
            layer_t* new_layer = malloc(sizeof(layer_t));
            init_layer(new_layer, qc->n_qubits);
            new_layer->operations[0] = operation;
            new_layer->n_operations = 1;
            // add qubits operated on by the operation to the layer
            for (int i = 0; i < operation->n_qubit_indices; i++) {
                if (operation->qubit_indices[i] == 1) {
                    new_layer->qubits[i] = 1;
                }
            }
    
            qc->layers[qc->n_layers] = new_layer;
            qc->n_layers++;
        } else {
            // add the operation to the layer
            qc->layers[layer_fit_index]->operations[qc->layers[layer_fit_index]->n_operations] = operation;
            qc->layers[layer_fit_index]->n_operations++;

            // add qubits operated on by the operation to the layer
            for (int i = 0; i < operation->n_qubit_indices; i++) {
                if (operation->qubit_indices[i] == 1) {
                    qc->layers[layer_fit_index]->qubits[i] = 1;
                }
            }
        }
    }
}


void x(qc_t* qc, int qubit_index) {
    operation_t* op = calloc(1, sizeof(operation_t));
    op->qubit_indices = (int*) calloc(qc->n_qubits, sizeof(int));
    op->qubit_indices[qubit_index] = 1;
    op->n_qubit_indices = qc->n_qubits;

    gate_t* x_gate = malloc(sizeof(gate_t));
    new_gate(x_gate, 2, "X");
    build_x_gate(x_gate);
    op->gate = x_gate;
    add_operation(qc, op);
}

void h(qc_t* qc, int qubit_index) {
    operation_t* op = calloc(1, sizeof(operation_t));
    op->qubit_indices = (int*) calloc(qc->n_qubits, sizeof(int));
    op->qubit_indices[qubit_index] = 1;

    op->n_qubit_indices = qc->n_qubits;

    gate_t* h_gate = malloc(sizeof(gate_t));
    new_gate(h_gate, 2, "H");
    build_h_gate(h_gate);

    op->gate = h_gate;
    add_operation(qc, op);
}

void z(qc_t* qc, int qubit_index) {
    operation_t* op = calloc(1, sizeof(operation_t));
    op->qubit_indices = (int*) calloc(qc->n_qubits, sizeof(int));
    op->qubit_indices[qubit_index] = 1;
    op->n_qubit_indices = qc->n_qubits;

    gate_t* z_gate = malloc(sizeof(gate_t));
    new_gate(z_gate, 2, "Z");
    build_z_gate(z_gate);
    op->gate = z_gate;
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

    gate_t* cx_gate = malloc(sizeof(gate_t));
    new_gate(cx_gate, 1 << (target_index - ctrl_index + 1), "CX");

    gate_t* x_gate = malloc(sizeof(gate_t));
    new_gate(x_gate, 2, "X");
    build_x_gate(x_gate);
    build_controlled_single_qubit_gate(x_gate,ctrl_index, target_index, cx_gate);

    free_gate(x_gate);
    free(x_gate);

    operation_t* CX_op = calloc(1, sizeof(operation_t));
    CX_op->gate = cx_gate;
    CX_op->qubit_indices = calloc(qc->n_qubits, sizeof(int));
    for (int i = ctrl_index; i <= target_index; i++) {
        CX_op->qubit_indices[i] = 1;
    }
    CX_op->n_qubit_indices = qc->n_qubits;

    add_operation(qc, CX_op);

    if (swapped) {
        swap(qc, ctrl_index, target_index);
    }
}

// requires qubit_2 > qubit_1
void swap(qc_t* qc, int qubit_1, int qubit_2) {
    gate_t* swap_gate = malloc(sizeof(gate_t));
    new_gate(swap_gate, 1 << (qubit_2 - qubit_1 + 1), "SWP");
    
    for (int i = 0; i < swap_gate->ndim; i++) {
        for (int j = 0; j < swap_gate->ndim; j++) {
            swap_gate->elements[i][j] = (polar_t) {0, 0};
        }
    }

    build_swap_gate(swap_gate, qubit_1, qubit_2);

    operation_t* op = calloc(1, sizeof(operation_t));
    op->gate = swap_gate;
    op->qubit_indices = calloc(qc->n_qubits, sizeof(int));
    for (int i = qubit_1; i <= qubit_2; i++) {
        op->qubit_indices[i] = 1;
    }

    op->n_qubit_indices = qc->n_qubits;
    add_operation(qc, op);
}

void rx(qc_t* qc, int qubit_index, double theta) {
    operation_t* op = calloc(1, sizeof(operation_t));
    gate_t* rx_gate = malloc(sizeof(gate_t));
    new_gate(rx_gate, 2, "RX");
    build_rx_gate(rx_gate, theta);
    op->gate = rx_gate;
    op->qubit_indices = calloc(qc->n_qubits, sizeof(int));
    op->qubit_indices[qubit_index] = 1;
    op->n_qubit_indices = qc->n_qubits;
    add_operation(qc, op);
}

void ry(qc_t* qc, int qubit_index, double theta) {
    operation_t* op = calloc(1, sizeof(operation_t));
    gate_t* ry_gate = malloc(sizeof(gate_t));
    new_gate(ry_gate, 2, "RY");
    build_ry_gate(ry_gate, theta);
    op->gate = ry_gate;
    op->qubit_indices = calloc(qc->n_qubits, sizeof(int));
    op->qubit_indices[qubit_index] = 1;
    op->n_qubit_indices = qc->n_qubits;
    add_operation(qc, op);
}

void rz(qc_t* qc, int qubit_index, double theta) {
    operation_t* op = calloc(1, sizeof(operation_t));
    gate_t* rz_gate = malloc(sizeof(gate_t));
    new_gate(rz_gate, 2, "RZ");
    build_rz_gate(rz_gate, theta);
    op->gate = rz_gate;
    op->qubit_indices = calloc(qc->n_qubits, sizeof(int));
    op->qubit_indices[qubit_index] = 1;
    op->n_qubit_indices = qc->n_qubits;
    add_operation(qc, op);
}

void phase_shift(qc_t* qc, int qubit_index, double theta) {

    operation_t* op = calloc(1, sizeof(operation_t));
    gate_t* ps_gate = malloc(sizeof(gate_t));
    new_gate(ps_gate, 2, "P");

    build_phase_shift_gate(ps_gate, theta);


    op->gate = ps_gate;
    op->qubit_indices = calloc(qc->n_qubits, sizeof(int));
    op->qubit_indices[qubit_index] = 1;
    op->n_qubit_indices = qc->n_qubits;
    add_operation(qc, op);
}

void qft(qc_t* qc, int start_qubit, int end_qubit) {
    for (int i = start_qubit; i <= end_qubit; i++) {
        h(qc, i);

        for (int j = i + 1; j <= end_qubit; j++) {
            swap(qc, i, j);
            gate_t* ps = malloc(sizeof(gate_t));
            new_gate(ps, 2, "PS");

            ps->ndim = 2;
            build_phase_shift_gate(ps, PI / (1 << (j - i)));
            // build_rz_gate(rz, 2 * PI / (1 << (j - i)));


            gate_t* crps = malloc(sizeof(gate_t));
            new_gate(crps, 1 << (j - i + 1), "CRPS");
            build_controlled_single_qubit_gate(ps, i, j, crps);
            crps->ndim = 1 << (j - i + 1);
            
            free_gate(ps);
            free(ps);
            
            operation_t* op = calloc(1, sizeof(operation_t));
            op->gate = crps;
            op->qubit_indices = calloc(qc->n_qubits, sizeof(int));
            for (int k = i; k <= j; k++) {
                op->qubit_indices[k] = 1;
            }
            op->n_qubit_indices = qc->n_qubits;
            add_operation(qc, op);
            swap(qc, i, j);
        }
    }
}

void print_qc_amplitudes(qc_t* qc) {
    for (int i = 0; i < qc->n_amplitudes; i++) {
        printf("|%d> = %f exp(%f i)\n", i, qc->amps[i].r, qc->amps[i].theta);
    }
}

void run_qc(qc_t* qc) {

    gate_t* i_gate = malloc(sizeof(gate_t));
    new_gate(i_gate, 2, "I");
    build_i_gate(i_gate);

    // iterate through the layers and apply the operations in order
    for (int i = 0; i < qc->n_layers; i++) {
        // for each qubit in the layer we multiply either the operation that exists 
        // or the identiy matrix 

        int qubit_index = 0;

        polar_t** product = NULL;

        int product_dim = 1; // product is currently product_dim * product_dim

        


        while (qubit_index < qc->n_qubits) {

            if (qc->layers[i]->qubits[qubit_index] == 1) {

                // find the operation that operates on qubit_index
                operation_t* operation = NULL;
                for (int j = 0; j < qc->layers[i]->n_operations; j++) {
                    if (qc->layers[i]->operations[j]->qubit_indices[qubit_index] == 1) {
                        operation = qc->layers[i]->operations[j];
                        break;
                    }
                }

                if (operation != NULL) { // should never be null
                    // if product is null, we set product to the operation matrix
                    if (product == NULL) {
                        product = malloc(operation->gate->ndim * sizeof(polar_t*));
                        for (int k = 0; k < operation->gate->ndim; k++) {
                            product[k] = malloc(operation->gate->ndim * sizeof(polar_t));
                        }
                        copy_matrix(operation->gate->elements, product, operation->gate->ndim, operation->gate->ndim);
                        product_dim = operation->gate->ndim;
                        qubit_index += (int) log2(operation->gate->ndim);
                    }  else {
                        // product is not null and we multiply the product with the operation

                        polar_t** new_product;
                        kronecker_product(product, operation->gate->elements, &new_product, product_dim, product_dim, operation->gate->ndim, operation->gate->ndim);
                        for (int i = 0; i < product_dim; i++) {
                            free(product[i]);
                        }
                        free(product); // free the old product before setting it to the new pointer
                        product = new_product;
                        product_dim *= operation->gate->ndim;
                        qubit_index += (int) log2(operation->gate->ndim);
                    }
                }
            } else {
                if (product == NULL){
                    product = malloc(2 * sizeof(polar_t*));
                    for (int k = 0; k < 2; k++) {
                        product[k] = malloc(2 * sizeof(polar_t));
                    }                                    

                    copy_matrix(i_gate->elements, product, 2, 2);

                    product_dim = 2;
                    qubit_index++;
                } else {

                    // multiply the product by the identity matrix
                    polar_t** new_product;
                    kronecker_product(product, i_gate->elements, &new_product, product_dim, product_dim, 2, 2);
                    for (int i = 0; i < product_dim; i++) {
                        free(product[i]);
                    }
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

        matrix_vector_mult(product, qc->amps, new_amps, product_dim);

        // copy new_amps to qc->amps
        for (int i = 0; i < product_dim; i++) {
            qc->amps[i] = (polar_t) {new_amps[i].r, new_amps[i].theta};
        }

        free(new_amps);
        for (int i = 0; i < product_dim; i++) {
            free(product[i]);
        }
        free(product);
    }

    free_gate(i_gate);
    free(i_gate);
}

void print_qc(qc_t* qc) {
    // print based on layers

    int longest_operation_name = longest_operation_name_in_qc(qc);
    int left_right_padding = 0;
    if (longest_operation_name % 2 == 0) {
        longest_operation_name++;
    }

    for (int i = 0; i < qc->n_qubits; i++) { // n qubits lines of output
        printf("qubit %d |0>", i);

        for (int j = 0; j < qc->n_layers; j++) {

            // int longest_operation_name = longest_operation_name_in_layer(qc->layers[j]);
            if (qc->layers[j]->qubits[i] == 1) {
                // find the operation that operates on qubit i
                char* gate_name = "X"; // default fallback
                for (int k = 0; k < qc->layers[j]->n_operations; k++) {
                    if (qc->layers[j]->operations[k]->qubit_indices[i] == 1) {
                        gate_name = qc->layers[j]->operations[k]->gate->name;
                        break;
                    }
                }
                int padding = (longest_operation_name - strlen(gate_name)) / 2 + left_right_padding;
                for (int k = 0; k < padding; k++) {
                    printf("-");
                }
                printf("%s", gate_name);
                for (int k = 0; k < padding; k++) {
                    printf("-");
                }
            } else {
                for (int k = 0; k < longest_operation_name + 2 * left_right_padding; k++) {
                    printf("-");
                }
            }
            printf("|");
        }

        printf("\n");
    }

    printf("layer #");
    int width = longest_operation_name + 2 * left_right_padding;
    for (int j = 0; j < width / 2 + 4; j++) {
        printf(" ");
    }
    // width of each layer is longest_operation_name + 2 * left_right_padding
    // we print the layer number under each
    for (int i = 0; i < qc->n_layers; i++) {
        printf("%d", i);
        for (int j = 0; j < (i >= 9 ? width - 1 : width); j++) {
            printf(" ");
        }
    }
    printf("\n");
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

void print_qc_amplitude_probabilities(qc_t* qc) {
    printf("|");
    for (int i = 0; i < qc->n_qubits; i++) {
        printf("q%d", i);
    }
    printf(">\n");
    for (int i = 0; i < qc->n_amplitudes; i++) {
        printf("|%d> = |%s> = %f exp(%f i) with probability %f\n", i, decimal_to_binary(i, qc->n_qubits), qc->amps[i].r, qc->amps[i].theta, qc->amps[i].r * qc->amps[i].r);
    }
}

void print_qc_amplitude_probabilities_range(qc_t* qc, int start_qubit_index, int end_qubit_index) {

    // we need to iterate through the nubmers from 0 to 2^ (end_qubit_index - start_qubit_index + 1)
    printf("Measuring qubits %d to %d: ", start_qubit_index, end_qubit_index);
    printf("|");
    for (int j = 0; j < end_qubit_index - start_qubit_index + 1; j++) {
        printf("q%d", j);
    }
    printf(">\n");
    for (int i = 0; i < 1 << (end_qubit_index - start_qubit_index + 1); i++) {

        double sum = 0;

        for (int j = 0; j < qc->n_amplitudes; j++) {
            if (((j >> (qc->n_qubits - end_qubit_index - 1)) ^ i) == 0) {
                sum += qc->amps[j].r * qc->amps[j].r;
            }
        }

 
        printf("|%d> = |%s> = probability %f\n", i, decimal_to_binary(i, end_qubit_index - start_qubit_index + 1), sum);


        }
        // then we iterate through the amplitudes and 
    

}