#ifndef QUBIT_H
#define QUBIT_H

#include "math.h"
#include "state.h"

typedef struct {
    int _id;
    char* name;
    state_t state;
} qubit_t;

#endif