#include <stdio.h>
#include <math.h>
#include "utils.h"
#include <stdlib.h>

gate_t I_gate;
gate_t X_gate;
gate_t H_gate;
gate_t Z_gate;
gate_t CX_gate;
gate_t SWAP_gate;
gate_t RX_gate;
gate_t RY_gate;
gate_t RZ_gate;

void print_polar(polar_t p) {
    printf("(%f)exp(i[%f])\n", p.r, p.theta);
}

void print_cart(cart_t c) {
    printf("(%f) + (%f)i\n", c.a, c.b);
}

void init_gates() {
    new_gate(&I_gate, 2, "I");
    (I_gate.elements[0][0]) = (polar_t) {1, 0};
    (I_gate.elements[1][1]) = (polar_t) {1, 0};
    (I_gate.elements[0][1]) = (polar_t) {0, 0};
    
    new_gate(&X_gate, 2, "X");
    (X_gate.elements[0][1]) = (polar_t) {1, 0};
    (X_gate.elements[1][0]) = (polar_t) {1, 0};
    
    new_gate(&H_gate, 2, "H");
    (H_gate.elements[0][0]) = (polar_t) {1/sqrt(2), 0};
    (H_gate.elements[0][1]) = (polar_t) {1/sqrt(2), 0};
    (H_gate.elements[1][0]) = (polar_t) {1/sqrt(2), 0}; 
    (H_gate.elements[1][1]) = (polar_t) {-1/sqrt(2), 0};

    new_gate(&Z_gate, 2, "Z");
    (Z_gate.elements[0][0]) = (polar_t) {1, 0};
    (Z_gate.elements[0][1]) = (polar_t) {0, 0};
    (Z_gate.elements[1][0]) = (polar_t) {0, 0};
    (Z_gate.elements[1][1]) = (polar_t) {-1, 0};

}

void copy_matrix(polar_t** src, polar_t** dst, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

void print_matrix(polar_t** matrix, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            print_polar(matrix[i][j]);
        }
    }
}

void _generate_random_matrix(polar_t*** matrix, int m, int n) {
    *matrix = calloc(m, sizeof(polar_t*));
    for (int i = 0; i < m; i++) {
        (*matrix)[i] = calloc(n, sizeof(polar_t));
        for (int j = 0; j < n; j++) {
            (*matrix)[i][j] = (polar_t) {rand() % 100, rand() % 100};
        }
    }
}