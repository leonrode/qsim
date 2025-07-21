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
// out-facing computations in polar form
typedef struct {
    real_t a;
    real_t b;
} cart_t; // a + bi3

typedef struct {
    real_t r;
    real_t theta;
} polar_t; // re^(i*theta)

// compares two cartesian numbers
bool_t cart_equal(cart_t a, cart_t b);

// adds two cartesian numbers
cart_t cart_add(cart_t a, cart_t b);

// multiplies two cartesian numbers
cart_t cart_mult(cart_t a, cart_t b);

// converts a cartesian number to a polar number
polar_t cart_to_polar(cart_t a);

// converts a polar number to a cartesian number
cart_t polar_to_cart(polar_t a);

// compares two polar numbers
bool_t polar_equal(polar_t a, polar_t b);

// adds two polar numbers
polar_t polar_add(polar_t a, polar_t b);

// multiplies two polar numbers
polar_t polar_mult(polar_t a, polar_t b);

// reduces a polar number to the range [0, 2*pi)
polar_t reduce_polar(polar_t a);

// compares two real numbers
bool_t equals(real_t a, real_t b);

// multiplies a vector by a matrix
// assumes a is n * n, b is n * 1, c is n * 1
// does not allocate c
void matrix_vector_mult(polar_t** a, polar_t* b, polar_t* c, int n);

// a is m * n, b is p * q, c is (m * p) * (n * q) matrix
// c will be dynamically allocated
void kronecker_product(polar_t** a, polar_t** b, polar_t*** c, int m, int n, int p, int q);

// a and b are m * n matrices, c is m * n matrix
// does not allocate c
void matrix_add(polar_t** a, polar_t** b, polar_t** c, int m, int n);

// a is m * n, b is n * p, and c is m * p matrix
// does not allocate c
void matrix_mult(polar_t** a, polar_t** b, polar_t*** c, int m, int n, int p);

// raises a to the power of power and stores the result in b
void matrix_power(polar_t** a, polar_t*** b, int n, int power);

// conjugates a matrix and stores the result in b
// allocates b
void matrix_conjugate(polar_t** a, polar_t*** b, int n);

// transposes a square matrix of size n and stores the result in b
// allocates b
void matrix_transpose(polar_t** a, polar_t*** b, int n);

// frees a matrix
void free_matrix(polar_t** matrix, int m);


#endif