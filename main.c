#include <stdio.h>
#include "qc.h"
#include "qmath.h"
#include "utils.h"
#include <stdlib.h>

int main(int argc, char** argv) {


    init_gates();

    gate_t* CX = malloc(sizeof(gate_t));
    new_gate(CX, 4, "CX");

    print_gate(&X_gate);

    build_controlled_single_qubit_gate(&X_gate, 2, 2, 1, 0, CX);

    print_gate(CX);


    return 0;
}
