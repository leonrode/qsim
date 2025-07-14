#include <stdio.h>
#include "qc.h"
#include "utils.h"
#include <stdlib.h>
#include "gate.h"

int main() {
    // initialize gates
    init_gates();

    qc_t* qc = malloc(sizeof(qc_t));
    init_qc(qc, 2);

    x(qc, 0);
    x(qc, 1);

    h(qc, 0);
    cx(qc, 0, 1);


    z(qc, 1);

    print_qc_amplitudes(qc);
    run_qc(qc);

    print_qc(qc);
    print_qc_amplitudes(qc);

    free_qc(qc);

    return 0;
}
