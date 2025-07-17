#include "gate.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * Initialize gate struct with n dimensions i.e. n x n matrix
 */
void new_gate(gate_t* gate, int n, char* name) {
    gate->ndim = n;
    gate->elements = malloc(n * sizeof(polar_t*));
    gate->name = name;
    for (int i = 0; i < n; i++) {
        gate->elements[i] = malloc(n * sizeof(polar_t));
        for (int j = 0; j < n; j++) { // init the gate to zero
            (gate->elements[i][j]) = (polar_t) {0, 0};
        }
    }
}

void free_gate(gate_t* gate) {
    for (int i = 0; i < gate->ndim; i++) {
        free(gate->elements[i]);
    }
    
    free(gate->elements);
}

void copy_gate(gate_t* src, gate_t* dest) {
    dest->ndim = src->ndim;
    dest->name = src->name;
    dest->elements = malloc(src->ndim * sizeof(polar_t*));
    for (int i = 0; i < src->ndim; i++) {
        dest->elements[i] = malloc(src->ndim * sizeof(polar_t));
        for (int j = 0; j < src->ndim; j++) {
            dest->elements[i][j] = src->elements[i][j];
        }
    }
}

void print_gate(gate_t* gate) {
    printf("Gate: %s\n", gate->name);
    for (int r = 0; r < gate->ndim; r++) {
        polar_t* row = gate->elements[r];

        for (int c = 0; c < gate->ndim; c++) {
            printf("(%f)exp([%f]i) ", (row + c)->r, (row + c)->theta);
        }
        printf("\n");
    }
}


void fill_with_zeros(gate_t* gate) {
    for (int i = 0; i < gate->ndim; i++) {
        for (int j = 0; j < gate->ndim; j++) {
            (gate->elements[i][j]) = (polar_t) {0, 0};
        }
    }
}

void build_i_gate(gate_t* gate) {
    gate->elements[0][0] = (polar_t) {1, 0};
    gate->elements[0][1] = (polar_t) {0, 0};
    gate->elements[1][0] = (polar_t) {0, 0};
    gate->elements[1][1] = (polar_t) {1, 0};
}

void build_h_gate(gate_t* gate) {
    gate->elements[0][0] = (polar_t) {1/sqrt(2), 0};
    gate->elements[0][1] = (polar_t) {1/sqrt(2), 0};
    gate->elements[1][0] = (polar_t) {1/sqrt(2), 0};
    gate->elements[1][1] = (polar_t) {-1/sqrt(2), 0};
}

void build_x_gate(gate_t* gate) {
    gate->elements[0][0] = (polar_t) {0, 0};
    gate->elements[0][1] = (polar_t) {1, 0};
    gate->elements[1][0] = (polar_t) {1, 0};
    gate->elements[1][1] = (polar_t) {0, 0};
}

void build_z_gate(gate_t* gate) {
    gate->elements[0][0] = (polar_t) {1, 0};
    gate->elements[0][1] = (polar_t) {0, 0};
    gate->elements[1][0] = (polar_t) {0, 0};
    gate->elements[1][1] = (polar_t) {-1, 0};
}






void build_controlled_single_qubit_gate(gate_t* u, int control_qubit, int target_qubit, gate_t* output) {

    // number of qubits between the control and target
    // e.g. for CX[0, 1] we have n = 0
    int n = target_qubit - control_qubit - 1;
    // compute N = 2^(n+2)
    int N = 1 << (n + 2); 
    
    // insert (N / 2) order identity in top left corner
    // we assume the output gate is already initialized with zeros
    for (int i = 0; i < N / 2; i++) {
        output->elements[i][i] = (polar_t) {1, 0};
    }

    // we compute kronecker product of I_(N/4) and U
    
    // this will be of size N/2 x N/2

    polar_t** IN_4 = malloc(N/4 * sizeof(polar_t*));
    for (int i = 0; i < N/4; i++) {
        IN_4[i] = malloc(N/4 * sizeof(polar_t));
        for (int j = 0; j < N/4; j++) {
            if (i == j) IN_4[i][j] = (polar_t) {1, 0}; // 1's along diagonal
            else IN_4[i][j] = (polar_t) {0, 0};
        }
    }

    // we init product of size N/2
    polar_t** product = NULL;  // let kronecker_product allocate it

    kronecker_product(IN_4, u->elements, &product, N/4, N/4, u->ndim, u->ndim);

    // copy product into bottom right corner
    for (int i = 0; i < N/2; i++) {
        for (int j = 0; j < N/2; j++) {
            output->elements[i + N/2][j + N/2] = product[i][j];
        }
    }


    for (int i = 0; i < N/4; i++) {
        free(IN_4[i]);
    }
    free(IN_4);
    for (int i = 0; i < N/2; i++) {
        free(product[i]);
    }
    free(product);

}

void build_swap_gate(gate_t* output, int qubit_1, int qubit_2) {

    // the size of this gate is 2^(qubit_2 - qubit_1 + 1)
    if (qubit_1 > qubit_2) { // account for the case where qubit_1 > qubit_2
        int t = qubit_1;
        qubit_1 = qubit_2;
        qubit_2 = t;
    }

    int n = qubit_2 - qubit_1 + 1;
    int N = 1 << n;
    int relative_qubit_1 = 0;
    int relative_qubit_2 = qubit_2 - qubit_1; // This is the offset from qubit_1

    // Initialize all elements to zero
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            output->elements[i][j] = (polar_t) {0, 0};
        }
    }

    // Populate the matrix
    for (int i = 0; i < N; i++) {
        // Get the bits at the *relative* positions within the current index 'i'
        int bit_at_rel_q1 = (i >> relative_qubit_1) & 1;
        int bit_at_rel_q2 = (i >> relative_qubit_2) & 1;

        int swapped_i = i; // Start with the original index

        // Only swap if the bits are different
        if (bit_at_rel_q1 != bit_at_rel_q2) {
            // Clear the bit at relative_qubit_1 position and set it to bit_at_rel_q2
            swapped_i &= ~(1 << relative_qubit_1);
            swapped_i |= (bit_at_rel_q2 << relative_qubit_1);

            // Clear the bit at relative_qubit_2 position and set it to bit_at_rel_q1
            swapped_i &= ~(1 << relative_qubit_2);
            swapped_i |= (bit_at_rel_q1 << relative_qubit_2);
        }

        // Place the 1 at the (i, swapped_i) position
        output->elements[i][swapped_i] = (polar_t) {1, 0};
    }
}

void build_rx_gate(gate_t* output, double theta) {
    output->elements[0][0] = cart_to_polar((cart_t) {cos(theta / 2), 0});
    output->elements[0][1] = cart_to_polar((cart_t) {0, -sin(theta / 2)});
    output->elements[1][0] = cart_to_polar((cart_t) {0, -sin(theta / 2)});
    output->elements[1][1] = cart_to_polar((cart_t) {cos(theta / 2), 0});
}

void build_ry_gate(gate_t* output, double theta) {
    output->elements[0][0] = cart_to_polar((cart_t) {cos(theta / 2), 0});
    output->elements[0][1] = cart_to_polar((cart_t) {0, -sin(theta / 2)});
    output->elements[1][0] = cart_to_polar((cart_t) {0, sin(theta / 2)});
    output->elements[1][1] = cart_to_polar((cart_t) {cos(theta / 2), 0});
}   

void build_rz_gate(gate_t* output, double theta) {
    output->elements[0][0] = cart_to_polar((cart_t) {cos(theta / 2), -sin(theta / 2)});
    output->elements[0][1] = cart_to_polar((cart_t) {0, 0});
    output->elements[1][0] = cart_to_polar((cart_t) {0, 0});
    output->elements[1][1] = cart_to_polar((cart_t) {cos(theta / 2), sin(theta / 2)});
}

void build_phase_shift_gate(gate_t* output, double theta) {
    output->elements[0][0] = (polar_t) {1, 0};
    output->elements[0][1] = (polar_t) {0, 0};
    output->elements[1][0] = (polar_t) {0, 0};
    output->elements[1][1] = (polar_t) {1, theta};
}