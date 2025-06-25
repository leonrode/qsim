#ifndef UTILS_H
#define UTILS_H

#include "qmath.h"
#include "gate.h"

void print_polar(polar_t p);
void print_cart(cart_t c);

extern gate_t I_gate;
extern gate_t X_gate;
extern gate_t H_gate;
extern gate_t Z_gate;

void init_gates();


#endif