#include <stdio.h>
#include "qc.h"
#include "utils.h"
#include <stdlib.h>

void qpe() {
    qc_t* qc = malloc(sizeof(qc_t));
    qc_t* qc_adjoint = malloc(sizeof(qc_t));
    qc_t* qc_qft = malloc(sizeof(qc_t));
    init_qc(qc, 4);
    init_qc(qc_adjoint, 4);
    init_qc(qc_qft, 4);

    int target_qubit = 3;

    qft(qc_qft, 0, target_qubit - 1);
    adjoint_qc(qc_qft, qc_adjoint);




    x(qc, target_qubit);
    for (int i = 0; i < 4; i++) {
        h(qc, i);
    }

    for (int i = target_qubit - 1; i >= 0; i--) {
        // we build the controlled phase shift gate
        // controlled on i and target on qubit 4

        gate_t* ps_gate = malloc(sizeof(gate_t));
        new_gate(ps_gate, 2, "P");
        
        build_phase_shift_gate(ps_gate, PI / 4);

        // take matrix power of ps_gate

        polar_t** ps_gate_power_elements = NULL;
        matrix_power(ps_gate->elements, &ps_gate_power_elements, 2, 1 << (target_qubit - i - 1));
        printf("power %d\n", 1 << (target_qubit - i - 1));
        ps_gate->elements = ps_gate_power_elements;



        
        gate_t* cps_gate = malloc(sizeof(gate_t));
        new_gate(cps_gate, 1 << (target_qubit - i + 1), "CPS");
        build_controlled_single_qubit_gate(ps_gate, i, target_qubit, cps_gate);
        operation_t* op = malloc(sizeof(operation_t));
        op->qubit_indices = calloc(qc->n_qubits, sizeof(int));

        for (int j = i; j <= target_qubit; j++) {
            op->qubit_indices[j] = 1;
        }
        op->n_qubit_indices = qc->n_qubits;
        op->gate = cps_gate;


        add_operation(qc, op);
    
    }

    append_qc(qc_adjoint, qc);



    print_qc(qc);
    run_qc(qc);
    printf("ran qc\n");
    print_qc_probabilities(qc);
    print_qc_amplitude_probabilities(qc);
    free_qc(qc);
}


int main() {


    // qc_t* qc = malloc(sizeof(qc_t));
    // qc_t* qc_adjoint = malloc(sizeof(qc_t));
    // init_qc(qc, 3);
    // init_qc(qc_adjoint, 3);
    // qft(qc, 0, 2);
    // adjoint_qc(qc, qc_adjoint);
    // append_qc(qc_adjoint, qc);


    // print_qc(qc);
    // run_qc(qc);
    // print_qc_amplitude_probabilities(qc);
    // print_qc_probabilities(qc);
    // free_qc(qc);


    
    // free_qc(qc_adjoint);

    qpe();
    return EXIT_SUCCESS;
}

