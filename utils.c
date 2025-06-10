#include <stdio.h>
#include <math.h>
#include "utils.h"

gate_t I_gate;
gate_t X_gate;
gate_t H_gate;

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
    I_gate.name = "I";
    
    new_gate(&X_gate, 2, "X");
    (X_gate.elements[0][1]) = (polar_t) {1, 0};
    (X_gate.elements[1][0]) = (polar_t) {1, 0};
    X_gate.name = "X";
    
    new_gate(&H_gate, 2, "H");
    (H_gate.elements[0][0]) = (polar_t) {1/sqrt(2), 0};
    (H_gate.elements[0][1]) = (polar_t) {1/sqrt(2), 0};
    (H_gate.elements[1][0]) = (polar_t) {1/sqrt(2), 0};
    (H_gate.elements[1][1]) = (polar_t) {-1/sqrt(2), 0};
    H_gate.name = "H";

}


