#include <assert.h>
#include "math.h"
#include "qmath.h"
#include "utils.h"
#include "gate.h"

static cart_t ZERO = {0, 0};
static cart_t ONE = {1, 0};
static cart_t I = {0, 1};
static cart_t A = {1, 3};
static cart_t B = {3, 4};
static cart_t A_PLUS_B = {4, 7};
static cart_t A_TIMES_B = {-9, 13};

static polar_t ONE_POLAR = {1, 0};
static polar_t I_POLAR = {1, PI/2};

static polar_t A_UNRED = {1, 3*PI + 0.5};
static polar_t A_RED = {1, 3.6415926536};

static gate_t X_GATE;
static gate_t H_GATE;


void cart_addition() {
    assert(cart_equal(cart_add(A, B), A_PLUS_B) == TRUE);
    assert(cart_equal(cart_add(B, A), A_PLUS_B) == TRUE);
    assert(cart_equal(cart_add(A, ZERO), A) == TRUE);
    assert(cart_equal(cart_add(B, ZERO), B) == TRUE);
    assert(cart_equal(cart_add(ZERO, ZERO), ZERO) == TRUE);
}

void cart_multiplication() {
    assert(cart_equal(cart_mult(A, B), A_TIMES_B) == TRUE);
    assert(cart_equal(cart_mult(B, A), A_TIMES_B) == TRUE);
    assert(cart_equal(cart_mult(ZERO, A), ZERO) == TRUE);
    assert(cart_equal(cart_mult(ONE, A), A) == TRUE);
    assert(cart_equal(cart_mult(ZERO, ZERO), ZERO) == TRUE);

}

void polar_equality() {
    assert(polar_equal(ONE_POLAR, ONE_POLAR) == TRUE);
    assert(polar_equal(I_POLAR, ONE_POLAR) == FALSE);
}

void polar_reduction() {
    assert(polar_equal(reduce_polar(A_UNRED), A_RED) == TRUE);
}

void complex_conversions() {
    assert(polar_equal(ONE_POLAR, cart_to_polar(ONE)) == TRUE);
    assert(polar_equal(ONE_POLAR, cart_to_polar(I)) == FALSE);
}


void x_gate() {


    state_t zero_state, one_state;
    init_state(&zero_state, (polar_t[]) {{1, 0}, {0, 0}});
    init_state(&one_state, (polar_t[]) {{0, 0}, {1, 0}});

    apply_gate_to_state(&X_GATE, &zero_state);
    assert(states_equal(&zero_state, &one_state) == TRUE);

    // reset zero state

    init_state(&zero_state, (polar_t[]) {{1, 0}, {0, 0}});
    apply_gate_to_state(&X_GATE, &one_state);

    assert(states_equal(&zero_state, &one_state) == TRUE);

    // reset one state
    init_state(&one_state, (polar_t[]) {{0, 0}, {1, 0}});
}

void h_gate() {
    state_t zero_state, one_state, plus_state, minus_state;

    init_state(&zero_state, (polar_t[]) {{1, 0}, {0, 0}});
    init_state(&one_state, (polar_t[]) {{0, 0}, {1, 0}});
    init_state(&plus_state, (polar_t[]) {{1/sqrt(2), 0}, {1/sqrt(2), 0}});
    init_state(&minus_state, (polar_t[]) {{1/sqrt(2), 0}, {1/sqrt(2), PI}});

    // 0 --H-> +
    apply_gate_to_state(&H_GATE, &zero_state);
    assert(states_equal(&zero_state, &plus_state) == TRUE);
    init_state(&zero_state, (polar_t[]) {{1, 0}, {0, 0}});

    // 1 --H-> -
    apply_gate_to_state(&H_GATE, &one_state);
    assert(states_equal(&one_state, &minus_state) == TRUE);
    init_state(&one_state, (polar_t[]) {{0, 0}, {1, 0}});

    // 0--H--H-> 0

    state_t zero_comp;
    init_state(&zero_comp, (polar_t[]) {{1, 0}, {0, 0}});
    apply_gate_to_state(&H_GATE, &zero_state);
    apply_gate_to_state(&H_GATE, &zero_state);

    assert(states_equal(&zero_comp, &zero_state) == TRUE);
    init_state(&zero_state, (polar_t[]) {{1, 0}, {0, 0}});

    // 1--H--H-->1
    state_t one_comp; // comparison since modifying one_state directly
    init_state(&one_comp, (polar_t[]) {{0, 0}, {1, 0}});
    apply_gate_to_state(&H_GATE, &one_state);
    print_state(&one_comp);
    print_state(&one_state);
    apply_gate_to_state(&H_GATE, &one_state);
    print_state(&one_state);
    print_state(&one_comp);
    assert(states_equal(&one_comp, &one_state) == TRUE);
    init_state(&one_state, (polar_t[]) {{1, 0}, {0, 0}});

}

int main() {

    new_gate(&X_GATE, 2);
    (X_GATE.elements[0][1]) = (polar_t) {1, 0};
    (X_GATE.elements[1][0]) = (polar_t) {1, 0};

    new_gate(&H_GATE, 2);
    (H_GATE.elements[0][0]) = (polar_t) {1/sqrt(2), 0};
    (H_GATE.elements[0][1]) = (polar_t) {1/sqrt(2), 0};
    (H_GATE.elements[1][0]) = (polar_t) {1/sqrt(2), 0};
    (H_GATE.elements[1][1]) = (polar_t) {-1/sqrt(2), 0};

    cart_addition();
    cart_multiplication();
    polar_equality();

    polar_reduction();
    complex_conversions();

    x_gate();
    h_gate();
}