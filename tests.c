#include "qmath.h"
#include <assert.h>

static cart_t ZERO = {0, 0};
static cart_t ONE = {1, 0};
static cart_t I = {0, 1};
static cart_t A = {1, 3};
static cart_t B = {3, 4};
static cart_t A_PLUS_B = {4, 7};
static cart_t A_TIMES_B = {-9, 13};


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

void complex_conversions() {

}

int main() {
    cart_addition();
    cart_multiplication();

}