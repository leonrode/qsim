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


    h(&qc, 0);
    x(&qc, 1);

    run_qc(&qc);
    print_qc_states(&qc);

    return 0;
}
