#include <stdio.h>
#include "qc.h"
#include "utils.h"
#include <stdlib.h>

int main() {
    // initialize gates
    init_gates();

    qc_t* qc = malloc(sizeof(qc_t));
    init_qc(qc, 2);


    x(qc, 1);

    h(qc, 0);
    h(qc, 1);

    cx(qc, 0, 1);
    
    h(qc, 0);
    print_qc(qc);

    run_qc(qc);
    
    print_qc_amplitudes(qc);
    print_qc_probabilities(qc);
    free_qc(qc);

    return EXIT_SUCCESS;
}
