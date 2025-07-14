#ifndef GATE_H
#define GATE_H

#include "qmath.h"

typedef struct {
    int ndim;
    char* name;
    polar_t** elements;
} gate_t;


void new_gate(gate_t* gate, int n, char* name);
void free_gate(gate_t* gate);
void print_gate(gate_t* gate);
void fill_with_zeros(gate_t* gate);

void build_controlled_single_qubit_gate(gate_t* u, int control_qubit, int target_qubit, gate_t* output);
void build_swap_gate(gate_t* output, int qubit_1, int qubit_2);
void build_rx_gate(gate_t* output, double theta);
void build_ry_gate(gate_t* output, double theta);
void build_rz_gate(gate_t* output, double theta);

#endif