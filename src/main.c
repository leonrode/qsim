#include <stdio.h>
#include "qc.h"
#include "utils.h"
#include <stdlib.h>
#include "gate.h"

int main() {
    // initialize gates
    init_gates();
 
    qc_t* qc = malloc(sizeof(qc_t));
    init_qc(qc, 3);

    x(qc, 0);
    x(qc, 1);
    h(qc, 0);

    h(qc, 2);

    cx(qc, 0,2);

    print_qc(qc);
    print_qc_layers(qc);


    return 0;
}
