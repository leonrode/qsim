#ifndef GATE_H
#define GATE_H

#include "qmath.h"
#include "state.h"

typedef struct {
    int ndim;
    polar_t** elements;
} gate_t; // assume square gate for now 


void new_gate(gate_t* gate, int n);
void print_gate(gate_t* gate);

void apply_gate_to_state(gate_t* gate, state_t* state);

#endif