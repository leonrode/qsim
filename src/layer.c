#include "layer.h"
#include "qc.h"
#include <stdlib.h>

// void layer_qc(struct qc_t* qc, layer_t** layers, int* n_layers) {

// }

int operation_layer_overlap(operation_t* operation, layer_t* layer) {
    for (int i = 0; i < layer->n_qubits; i++) {
        int qubit_index = layer->qubits[i];

        // search through the operation's qubit indices for a match with the qubit index
        for (int j = 0; j < operation->n_qubit_indices; j++) {
            if (operation->qubit_indices[j] == qubit_index) {
                return 1; // they overlap
            }
        }
    }
    return 0; // they don't overlap
}

void add_operation_to_layer(operation_t* operation, layer_t* layer) {

    // since we preassume that the operation fits,
    // we are certain we must add it to this layer
    // and thus we can reallocate the layer's qubits array
    
    layer->operations = realloc(layer->operations, sizeof(operation_t) * (layer->n_operations + 1));
    layer->operations[layer->n_operations] = *operation;
    layer->n_operations++;

    // add the operation's qubit indices to the layer's qubits array
    layer->qubits = realloc(layer->qubits, sizeof(int) * (layer->n_qubits + operation->n_qubit_indices));
    for (int i = 0; i < operation->n_qubit_indices; i++) {
        layer->qubits[layer->n_qubits + i] = operation->qubit_indices[i];
    }
    layer->n_qubits += operation->n_qubit_indices;
}

// void add_layer_to_qc(layer_t* layer, qc_t* qc) {
//     qc->layers = realloc(qc->layers, sizeof(layer_t) * (qc->n_layers + 1));
//     qc->layers[qc->n_layers] = *layer;
//     qc->n_layers++;
// }