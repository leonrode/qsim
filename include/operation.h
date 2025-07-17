#ifndef OPERATION_H
#define OPERATION_H

#include "gate.h"

typedef struct {
    int* qubit_indices; // for qubit i (0 ≤ i < n_qubits), qubits[i] = 1 if qubit i is acted on by an operation in the layer, 0 otherwise
    int n_qubit_indices;  // equal to the number of qubits in the circuit
    gate_t* gate;
} operation_t;

void free_operation(operation_t* operation);
void copy_operation(operation_t* src, operation_t* dest);
#endif 