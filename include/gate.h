#ifndef GATE_H
#define GATE_H

#include "qmath.h" // for polar_t

typedef struct {
    int ndim;
    char* name;
    polar_t** elements;
} gate_t;


// allocates a new gate with n dimensions and a name
void new_gate(gate_t* gate, int n, char* name);

// frees members of a gate_t, but not the gate_t itself
void free_gate(gate_t* gate);

// copies the contents of src into dest, assuming dest is already initialized
void copy_gate(gate_t* src, gate_t* dest);

// prints the gate's name and elements
void print_gate(gate_t* gate);

// populate's input gate's elements with the values of each elementary gate
void build_i_gate(gate_t* gate);
void build_h_gate(gate_t* gate);
void build_x_gate(gate_t* gate);
void build_z_gate(gate_t* gate);

// builds a controlled version of a single-qubit unitary U
// works properly only if control_qubit < target_qubit
void build_controlled_single_qubit_gate(gate_t* u, int control_qubit, int target_qubit, gate_t* output);

// builds a swap gate between qubit_1 and qubit_2 inclusive
void build_swap_gate(gate_t* output, int qubit_1, int qubit_2);

// builds a rotation gate about the x-axis
void build_rx_gate(gate_t* output, double theta);
// builds a rotation gate about the y-axis
void build_ry_gate(gate_t* output, double theta);
// builds a rotation gate about the z-axis
void build_rz_gate(gate_t* output, double theta);
// builds a phase shift gate
void build_phase_shift_gate(gate_t* output, double theta);

// builds the adjoint of a gate
void build_dagger_gate(gate_t* input, gate_t* output);

#endif