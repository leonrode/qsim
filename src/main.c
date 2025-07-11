#include <stdio.h>
#include "qc.h"
#include "qmath.h"
#include "utils.h"
#include <stdlib.h>

int main() {


    init_gates();
 
    qc_t* qc = malloc(sizeof(qc_t));
    init_qc(qc, 1);


    rx(qc, 0, PI / 2);
    rz(qc, 0, PI / 4);
    print_qc(qc);

    print_qc_amplitudes(qc);
    run_qc(qc);
    print_qc_amplitudes(qc);


    return 0;
}
