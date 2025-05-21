#include <stdio.h>
#include "utils.h"

void print_polar(polar_t p) {
    printf("(%f)exp(i[%f])\n", p.r, p.theta);
}

void print_cart(cart_t c) {
    printf("(%f) + (%f)i\n", c.a, c.b);
}