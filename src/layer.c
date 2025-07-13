#include <stdlib.h>
#include "layer.h"
#include <stdio.h>

void init_layer(layer_t* layer, int n_qubits) {
    layer->qubits = calloc(n_qubits, sizeof(int));
    layer->n_qubits = n_qubits;
    layer->operations = calloc(n_qubits, sizeof(operation_t));
    layer->n_operations = 0;
}

int operation_layer_overlap(operation_t* operation, layer_t* layer) {


    // we check if the qubits involved in the operation are already in the layer
    for (int i = 0; i < operation->n_qubit_indices; i++) {
        printf("qubit %d\n", operation->qubit_indices[i]);
        if (layer->qubits[operation->qubit_indices[i]] == 1) {
            return 1;
        }
    }
    return 0;
}

