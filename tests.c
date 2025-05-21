#include <assert.h>
#include "qmath.h"
#include "utils.h"

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

int main() {
    cart_addition();
    cart_multiplication();
    polar_equality();

    polar_reduction();
    complex_conversions();
}