/**
 * Responsible for all declarations and definitions for relevant linear algebra required for quantum computing
 */

#ifndef QMATH_H
#define QMATH_H

typedef double real_t;
typedef enum {
    FALSE,
    TRUE
} bool_t;

typedef struct {
    real_t a;
    real_t b;
} cart_t; // a + bi

typedef struct {
    real_t r;
    real_t theta;
} polar_t; // re^(i*theta)


bool_t complex_equal(cart_t a, cart_t b);
cart_t complex_add(cart_t a, cart_t b);
cart_t complex_mult(cart_t a, cart_t b);

polar_t cart_to_polar(cart_t a);
cart_t polar_to_cart(polar_t a);

#endif