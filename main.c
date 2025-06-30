#include <stdio.h>
#include "qc.h"
#include "qmath.h"
#include "utils.h"
#include <stdlib.h>

int main(int argc, char** argv) {


    init_gates();

    gate_t* CX = malloc(sizeof(gate_t));
    new_gate(CX, 4, "CX");

    print_gate(&X_gate);

    build_controlled_single_qubit_gate(&X_gate, 2, 2, 1, 0, CX);

    qc_t* qc = malloc(sizeof(qc_t));
    init_qc(qc, 2);

    operation_t H_op;
    H_op.gate = &H_gate;
    H_op.qubit_indices = malloc(sizeof(int) * 1);
    H_op.qubit_indices[0] = 0;
    H_op.n_qubit_indices = 1;
    add_operation(qc, &H_op);

    operation_t CX_op;
    CX_op.gate = CX;
    CX_op.qubit_indices = malloc(sizeof(int) * 2);
    CX_op.qubit_indices[0] = 0;
    CX_op.qubit_indices[1] = 1;
    CX_op.n_qubit_indices = 2;
    add_operation(qc, &CX_op);

    print_qc_operations(qc);

    run_qc(qc);


    return 0;
}
