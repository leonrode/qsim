#include <stdio.h>
#include "gate.h"
#include "state.h"
#include "utils.h"
#include "math.h"

int main(int argc, char** argv) {
    init_gates();

    state_t state;
    polar_t amplitudes[] = {{1, 0}, {0, 0}};
    init_state(&state, amplitudes);


    return 0;
}
