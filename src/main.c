#include <stdio.h>
#include "qc.h"
#include "qmath.h"
#include "utils.h"
#include <stdlib.h>

int main() {


    init_gates();
 
    qc_t* qc = malloc(sizeof(qc_t));
    init_qc(qc, 2);


    x(qc, 0);
    h(qc, 1);
    cx(qc, 0, 1);
    print_qc(qc);

    return 0;
}
