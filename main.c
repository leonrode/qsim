#include <stdio.h>
#include "qc.h"
#include "qmath.h"
#include "utils.h"
#include <stdlib.h>

int main(int argc, char** argv) {


    init_gates();

    qc_t* qc = malloc(sizeof(qc_t));
    init_qc(qc, 2);


    h(qc, 0);
    cx(qc, 0, 1);

    // print_qc_operations(qc);

    run_qc(qc);

    print_qc_amplitudes(qc);

    return 0;
}
