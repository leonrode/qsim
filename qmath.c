#include "qmath.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>

bool_t cart_equal(cart_t a, cart_t b) {
     return a.a == b.a && a.b == b.b;
}
cart_t cart_add(cart_t a, cart_t b) {
    cart_t sum = {a.a + b.a, a.b + b.b};

    return sum;
}


cart_t cart_mult(cart_t a, cart_t b) {
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

bool_t polar_equal(polar_t a, polar_t b) {
    real_t a_theta_effective = (a.theta < 2 * PI) ? a.theta : a.theta / (2 * PI);
    real_t b_theta_effective = (b.theta < 2 * PI) ? b.theta : b.theta / (2 * PI);

    return a.r == b.r && equals(a_theta_effective, b_theta_effective);
}

polar_t polar_add(polar_t a, polar_t b) {
    cart_t a_cart = polar_to_cart(a);
    cart_t b_cart = polar_to_cart(b);

    cart_t a_b_sum = cart_add(a_cart, b_cart);

    return cart_to_polar(a_b_sum);
}

polar_t polar_mult(polar_t a, polar_t b) {
    polar_t prod = { a.r * b.r, a.theta + b.theta };
    return prod;
}

bool_t equals(real_t a, real_t b) {
    return fabs(a - b) < REAL_EQ_ERR;
}
