#include <stdio.h>
#include "gate.h"
#include "qc.h"
#include <stdlib.h>

void qpe() {
    qc_t* qc = malloc(sizeof(qc_t));
    
    int num_qubits = 4;
    int target_qubit = num_qubits - 1;

    init_qc(qc, num_qubits);

    for (int i = 0; i < target_qubit; i++) {
        h(qc, i);
    }

    x(qc, target_qubit);

    for (int i = 0; i < target_qubit; i++) {

        printf("i: %d\n", i);

        gate_t* u = malloc(sizeof(gate_t));
        new_gate(u, 2, "U");
        build_phase_shift_gate(u, PI / 4); // pi/4 phase shift is U
        
        gate_t* u_power = malloc(sizeof(gate_t));
        new_gate(u_power, 2, "U^2");
        matrix_power(u->elements, &u_power->elements, 2, 1 << (target_qubit - i - 1));
        // print_matrix(u_power->elements, u_power->ndim, u_power->ndim);

        gate_t* crps = malloc(sizeof(gate_t));
        new_gate(crps, 1 << (target_qubit - i + 1), "CPS");
        build_controlled_single_qubit_gate(u_power, i, target_qubit, crps);

        free_gate(u);
        free_gate(u_power);


        operation_t* op = calloc(1, sizeof(operation_t));
        op->gate = crps;
        op->qubit_indices = calloc(num_qubits, sizeof(int));
        for (int j = i; j <= target_qubit; j++) {
            op->qubit_indices[j] = 1;
        }
        op->n_qubit_indices = num_qubits;
        add_operation(qc, op);
        
    }

    qc_t* qft_qc = malloc(sizeof(qc_t));
    init_qc(qft_qc, num_qubits);
    qft(qft_qc, 0, target_qubit - 1);
    adjoint_qc(qft_qc, qc);
    free_qc(qft_qc);

    run_qc(qc);
    print_qc(qc);
    print_qc_amplitude_probabilities_range(qc, 0, target_qubit - 1);

    free_qc(qc);
}


int main() {
    qpe();
    return EXIT_SUCCESS;
}

