/**
 * Responsible for layering logic of a quantum circuit
 */

#ifndef LAYER_H
#define LAYER_H

#include "operation.h"
// #include "qc.h"

typedef struct {
    int* qubits; // for qubit i (0 ≤ i < n_qubits), qubits[i] = 1 if qubit i is acted on by an operation in the layer, 0 otherwise
    int n_qubits; // height of layer (always equal to thenumber of qubits in the qc)
    operation_t** operations; // operations within the layer
    int n_operations; // number of operations within the layer
} layer_t;

void init_layer(layer_t* layer, int n_qubits);
void free_layer(layer_t* layer);
// returns 1 if the qubits involved in the operation are already in the layer, 0 otherwise
int operation_layer_overlap(operation_t* operation, layer_t* layer);

int layer_full(layer_t* layer);

#endif