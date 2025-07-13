/**
 * Responsible for layering logic of a quantum circuit
 */

#ifndef LAYER_H
#define LAYER_H

#include "operation.h"
// #include "qc.h"

typedef struct {
    operation_t* operations;
    int n_operations; // number of operations in the layer
    int* qubits; // qubits acted on in the layer. the number of qubits in this array is <= the size of the qc
    int n_qubits; // number of qubits acted on in the layer
} layer_t;

void layer_qc(struct qc_t* qc, layer_t** layers, int* n_layers);

// returns 1 if the qubits involved in the operation are already in the layer, 0 otherwise
int operation_layer_overlap(operation_t* operation, layer_t* layer);

// adds the operation to the layer, performing reallocation if necessary
// assumes that the operation does not overlap with the layer
void add_operation_to_layer(operation_t* operation, layer_t* layer);

// adds the layer to the qc
// void add_layer_to_qc(layer_t* layer, qc_t* qc);

#endif