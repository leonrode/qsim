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
    state_t** states; // 2 ^ n_qubits states. for n = 1 we have |0> and |1>, n = 2 we have |00>, |01>, |10>, |11>
    int n_operations;
    int n_qubits;
} qc_t;

void init_qc(qc_t* qc, int n_qubits);
void add_operation(qc_t* qc, operation_t* operation);
void x(qc_t* qc, int qubit_index);
void h(qc_t* qc, int qubit_index);
void cx(qc_t* qc, int qubit_index1, int qubit_index2);
void cy(qc_t* qc, int qubit_index1, int qubit_index2);
void cz(qc_t* qc, int qubit_index1, int qubit_index2);
void swap(qc_t* qc, int qubit_index1, int qubit_index2);
void cswap(qc_t* qc, int qubit_index1, int qubit_index2);
void ccx(qc_t* qc, int qubit_index1, int qubit_index2, int qubit_index3);

void run_qc(qc_t* qc);
void print_qc_states(qc_t* qc);

#endif