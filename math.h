/**
 * Responsible for all declarations and definitions for relevant linear algebra required for quantum computing
 */

#ifndef MATH_H
#define MATH_H

typedef int real_t;
typedef enum {
    FALSE,
    TRUE
} bool_t;

typedef struct {
    real_t a;
    real_t b;
} complex_t; // a + bi


bool_t complex_equal(complex_t a, complex_t b);
complex_t complex_add(complex_t a, complex_t b);
complex_t complex_mult(complex_t a, complex_t b);


#endif