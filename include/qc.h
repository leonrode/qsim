#ifndef QC_H
#define QC_H

#include "gate.h"

#define MAX_OPERATIONS 100 // max number of operations in a qc_t
typedef struct {
    int* qubit_indices;
    int n_qubit_indices;
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
    polar_t* amps; // 2^n_qubits polar_t representing the amplitudes of the states
    int n_operations;
    int n_qubits;
    int n_amplitudes; // we don't want to have to calculate this every time
} qc_t;

void init_qc(qc_t* qc, int n_qubits);
void add_operation(qc_t* qc, operation_t* operation);
void x(qc_t* qc, int qubit_index);
void h(qc_t* qc, int qubit_index);
void cx(qc_t* qc, int ctrl_index, int target_index);
void swap(qc_t* qc, int qubit_1, int qubit_2);
void rx(qc_t* qc, int qubit_index, double theta);
void ry(qc_t* qc, int qubit_index, double theta);
void rz(qc_t* qc, int qubit_index, double theta);

// remove global phase within a qc
void _remove_global_phase(qc_t* qc);

void run_qc(qc_t* qc);
void print_qc_amplitudes(qc_t* qc);
void print_qc_operations(qc_t* qc);
void print_qc(qc_t* qc);

#endif