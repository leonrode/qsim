#include "layer.h"
#include "qc.h"

void layer_qc(struct qc_t* qc, layer_t** layers, int* n_layers) {

}

int operation_layer_overlap(operation_t* operation, layer_t* layer) {
    for (int i = 0; i < layer->n_qubits; i++) {
        if (operation->qubit_indices[i] == layer->qubits[i]) {
            return 1;
        }
    }
    return 0;
}