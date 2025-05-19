#include "math.h"

bool_t complex_equal(complex_t a, complex_t b) {
     return a.a == b.a && a.b == b.b;
}
complex_t complex_add(complex_t a, complex_t b) {
    complex_t sum = {a.a + b.a, a.b + b.b};

    return sum;
}


complex_t complex_mult(complex_t a, complex_t b) {
    complex_t product = {a.a * b.a - a.b * b.b, a.a * b.b + b.a * a.b};

    return product;
}