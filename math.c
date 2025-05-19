#include "math.h"

complex_t complex_add(complex_t a, complex_t b) {
    complex_t sum = {a.a + b.a, a.b + b.b};

    return sum;
}

/**
 * (a+bi)*(c+di)
 * = (a*c+a*di+c*bi-b*d)
 * = (a*c-b*d)+ (a*d+c*b)i
 * = (a.a*b.a-a.b*b.b) + (a.a*b.b+b.a*a.b)
 */
complex_t complex_mult(complex_t a, complex_t b) {
    complex_t product = {a.a * b.a - a.b * b.b, a.a * b.b + b.a * a.b};

    return product;
}