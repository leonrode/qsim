#ifndef OPERATION_H
#define OPERATION_H

#include "gate.h"

typedef struct {
    int* qubit_indices;
    int n_qubit_indices;
    gate_t* gate;
} operation_t;

#endif 