#include <stdio.h>
#include "gate.h"
#include "state.h"
#include "qc.h"
#include "utils.h"
#include "math.h"

int main(int argc, char** argv) {
    init_gates();

    qc_t qc;
    init_qc(&qc, 2);

    operation_t op;
    op.qubit_index = 0;
    op.gate = &H_gate;
    add_operation(&qc, &op);

    operation_t op2;
    op2.qubit_index = 1;
    op2.gate = &X_gate;
    add_operation(&qc, &op2);
    run_qc(&qc);
    print_qc_states(&qc);

    return 0;
}
