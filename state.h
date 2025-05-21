#ifndef STATE_H
#define STATE_H
#include "qmath.h"

typedef struct {
    polar_t amplitudes[2];
} state_t;

void init_state(state_t* state, polar_t amplitudes[2]);
void print_state(state_t* state);
#endif