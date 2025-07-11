/**
 * Responsible for layering logic of a quantum circuit
 */

#ifndef LAYER_H
#define LAYER_H

// forward declarations
struct qc_t;
#include "operation.h"

typedef struct {
    int* qubits;
    int n_qubits;
} layer_t;

void layer_qc(struct qc_t* qc, layer_t** layers, int* n_layers);

// returns 1 if the qubits involved in the operation are already in the layer, 0 otherwise
int operation_layer_overlap(operation_t* operation, layer_t* layer);

// adds the operation to the layer, performing reallocation if necessary
void add_operation_to_layer(operation_t* operation, layer_t* layer);

// adds the layer to the qc
void add_layer_to_qc(layer_t* layer, struct qc_t* qc);

#endif