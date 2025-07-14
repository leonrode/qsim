/**
 * Responsible for layering logic of a quantum circuit
 */

#ifndef LAYER_H
#define LAYER_H

#include "operation.h"
// #include "qc.h"

typedef struct {
    int* qubits; // qubits acted on by operations in the layer
    int n_qubits; // height of layer (always equal to thenumber of qubits in the qc)
    operation_t* operations; // operations within the layer
    int n_operations; // number of operations within the layer
} layer_t;

void init_layer(layer_t* layer, int n_qubits);

// returns 1 if the qubits involved in the operation are already in the layer, 0 otherwise
int operation_layer_overlap(operation_t* operation, layer_t* layer);

int layer_full(layer_t* layer);

#endif