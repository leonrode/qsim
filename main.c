#include <stdio.h>
#include "gate.h"
#include "qc.h"
#include "utils.h"
#include "math.h"

int main(int argc, char** argv) {
    init_gates();

    qc_t qc;
    init_qc(&qc, 2);


    h(&qc, 0);
    x(&qc, 1);
    cx(&qc, 0, 1);

    print_qc_amplitudes(&qc);
    print_qc_operations(&qc);
    return 0;
}
