#include <stdlib.h>
#include "layer.h"
#include <stdio.h>

void init_layer(layer_t* layer, int n_qubits) {
    layer->qubits = calloc(n_qubits, sizeof(int));
    layer->n_qubits = n_qubits;
    layer->operations = calloc(n_qubits, sizeof(operation_t));
    layer->n_operations = 0;
}

void free_layer(layer_t* layer) {
    free(layer->qubits);

    for (int i = 0; i < layer->n_operations; i++) {
        free_operation(layer->operations[i]);
        free(layer->operations[i]);
    }
    free(layer->operations);
    free(layer);
}

int operation_layer_overlap(operation_t* operation, layer_t* layer) {
    // we check if the qubits involved in the operation are already in the layer
    for (int i = 0; i < operation->n_qubit_indices; i++) {
        if (layer->qubits[i] == 1 && operation->qubit_indices[i] == 1) {
            return 1;
        }
    }
    return 0;
}

int layer_full(layer_t *layer) {
    for (int i = 0; i < layer->n_qubits; i++) {
        if (layer->qubits[i] == 0) {
            return 0;
        }
    }
    return 1;
}