#ifndef QC_H
#define QC_H

#include "gate.h"
#include "state.h"

#define MAX_OPERATIONS 100 // max number of operations in a qc_t
typedef struct {
    int qubit_index;
    gate_t* gate;
} operation_t;

/**
 * --X--H---X
 * -----H---X
 * would be represented as (X, 0), (H, 0), (H, 1), (X, 0), (X, 1)
 * where the layering can be done after.

 */

typedef struct {
    operation_t* operations; // contains gate and applied qubit information in one dimensional array
    state_t* states;
    int n_operations;
    int n_qubits;
} qc_t;

void init_qc(qc_t* qc, int n_qubits);
void add_operation(qc_t* qc, operation_t* operation);
void run_qc(qc_t* qc);
void print_qc_states(qc_t* qc);

#endif