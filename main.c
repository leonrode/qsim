#include <stdio.h>
#include "qc.h"
#include "utils.h"

int main(int argc, char** argv) {
    init_gates();

    qc_t qc;
    init_qc(&qc, 4);


    h(&qc, 0);
    x(&qc, 1);
    cx(&qc, 0, 1);
    cx(&qc, 2, 3);

    print_qc_amplitudes(&qc);
    print_qc_operations(&qc);
    return 0;
}
