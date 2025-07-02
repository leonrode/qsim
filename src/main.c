#include <stdio.h>
#include "qc.h"
#include "qmath.h"
#include "utils.h"
#include <stdlib.h>

int main() {


    init_gates();
 
    qc_t* qc = malloc(sizeof(qc_t));
    init_qc(qc, 2);

    
    x(qc, 1);
    cx(qc, 1, 0);


    // cx(qc, 0, 1);
    print_qc_operations(qc);
    run_qc(qc);
    // build_swap_gate(&SWAP_gate, 1, 0);
    // print_gate(&SWAP_gate);



    print_qc_amplitudes(qc);

    return 0;
}
