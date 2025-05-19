#include "qmath.h"
#include <assert.h>

static cart_t ZERO = {0, 0};
static cart_t ONE = {1, 0};
static cart_t I = {0, 1};
static cart_t A = {1, 3};
static cart_t B = {3, 4};
static cart_t A_PLUS_B = {4, 7};
static cart_t A_TIMES_B = {-9, 13};

void complex_addition() {
    assert(complex_equal(complex_add(A, B), A_PLUS_B) == TRUE);
    assert(complex_equal(complex_add(B, A), A_PLUS_B) == TRUE);
    assert(complex_equal(complex_add(A, ZERO), A) == TRUE);
    assert(complex_equal(complex_add(B, ZERO), B) == TRUE);
    assert(complex_equal(complex_add(ZERO, ZERO), ZERO) == TRUE);
}

void complex_multiplication() {
    assert(complex_equal(complex_mult(A, B), A_TIMES_B) == TRUE);
    assert(complex_equal(complex_mult(B, A), A_TIMES_B) == TRUE);
    assert(complex_equal(complex_mult(ZERO, A), ZERO) == TRUE);
    assert(complex_equal(complex_mult(ONE, A), A) == TRUE);
    assert(complex_equal(complex_mult(ZERO, ZERO), ZERO) == TRUE);

}

int main() {
    
    complex_addition();

    complex_multiplication();

}