/**
 * Responsible for all declarations and definitions for relevant linear algebra required for quantum computing
 */

#ifndef QMATH_H
#define QMATH_H


#define PI 3.14159265
#define REAL_EQ_ERR 0.00000001;

typedef double real_t;
typedef enum {
    FALSE,
    TRUE
} bool_t;

// here because it is useful for intermediate operations
// but i think we'll rely on doing all 
// out-facing computations in polar 
typedef struct {
    real_t a;
    real_t b;
} cart_t; // a + bi3

typedef struct {
    real_t r;
    real_t theta;
} polar_t; // re^(i*theta)

typedef union {
    polar_t p;
    cart_t c;
} complex_t; // complex number is either polar or cartesian

bool_t cart_equal(cart_t a, cart_t b);
cart_t cart_add(cart_t a, cart_t b);
cart_t cart_mult(cart_t a, cart_t b);

polar_t cart_to_polar(cart_t a);
cart_t polar_to_cart(polar_t a);

bool_t polar_equal(polar_t a, polar_t b);
polar_t polar_add(polar_t a, polar_t b);
polar_t polar_mult(polar_t a, polar_t b);

polar_t reduce_polar(polar_t a);

bool_t equals(real_t a, real_t b);

// assumes a is n * n, b is n * 1, c is n * 1
void matrix_vector_mult(polar_t** a, polar_t* b, polar_t* c, int n);

// a is m * n, b is p * q, c is (m * p) * (n * q) matrix
void kronecker_product(polar_t** a, polar_t** b, polar_t** c, int m, int n, int p, int q);

// a and b are m * n matrices, c is m * n matrix
void matrix_add(polar_t** a, polar_t** b, polar_t** c, int m, int n);


#endif