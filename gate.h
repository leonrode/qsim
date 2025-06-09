#ifndef GATE_H
#define GATE_H

#include "qmath.h"

typedef struct {
    int ndim;
    char* name;
    polar_t** elements;
} gate_t;


void new_gate(gate_t* gate, int n);
void print_gate(gate_t* gate);


#endif