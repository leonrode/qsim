#include "qmath.h"
#include <math.h>

bool_t complex_equal(cart_t a, cart_t b) {
     return a.a == b.a && a.b == b.b;
}
cart_t complex_add(cart_t a, cart_t b) {
    cart_t sum = {a.a + b.a, a.b + b.b};

    return sum;
}


cart_t complex_mult(cart_t a, cart_t b) {
    cart_t product = {a.a * b.a - a.b * b.b, a.a * b.b + b.a * a.b};

    return product;
}

polar_t cart_to_polar(cart_t a) {
    double angle = atan2(a.b, a.a);
    double r = sqrt(a.a * a.a + a.b * a.b);

    polar_t ret = { r, angle };
    
    return ret;
}

cart_t polar_to_cart(polar_t a) {
    
    double real_c = a.r * cos(a.theta);
    double imag_c = a.r * sin(a.theta);

    cart_t ret = { real_c, imag_c };

    return ret;
}