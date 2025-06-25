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

    return reduce_polar(ret);
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

    return equals(a.r, b.r) && equals(a_theta_effective, b_theta_effective);
}

polar_t polar_add(polar_t a, polar_t b) {
    cart_t a_cart = polar_to_cart(a);
    cart_t b_cart = polar_to_cart(b);

    cart_t a_b_sum = cart_add(a_cart, b_cart);

    return reduce_polar(cart_to_polar(a_b_sum));
}

polar_t polar_mult(polar_t a, polar_t b) {
    polar_t prod = { a.r * b.r, a.theta + b.theta };


    return reduce_polar(prod);
}

/**
 * static polar_t A_UNRED = {1, 3*PI + 0.5};
 * static polar_t A_RED = {1, 3.6415926536};
 */
polar_t reduce_polar(polar_t a) {
    // puts a's angle between [0, 2pi)

    real_t theta_effective = a.theta;
    if (theta_effective < 0) theta_effective *= -1;

    theta_effective = theta_effective - (2 * PI * floor(theta_effective / (2 * PI)));

    return (polar_t) {a.r, theta_effective};
}

bool_t equals(real_t a, real_t b) {
    return fabs(a - b) < REAL_EQ_ERR;
}
// stores result in c
void matrix_vector_mult(polar_t** a, polar_t** b, polar_t** c, int n) {
    for (int i = 0; i < n; i++) {
        *c[i] = (polar_t) {0, 0};
        for (int j = 0; j < n; j++) {
            *c[i] = polar_add(*c[i], polar_mult(a[i][j], *b[j]));
        }
    }
}


// performs kronecker product of matrix a with dimension m * n
// with matrix b with dimension p * q
// stores result in c with dimension (m * p) * (n * q)
void kronecker_product(polar_t** a, polar_t** b, polar_t** c, int m, int n, int p, int q) {

    // c is a 2D array of size m * p * n * q
    // we iterate through the blocks each q columns 

    for (int block_x = 0; block_x < n * q; block_x += q) {
        for (int block_y = 0; block_y < m * p; block_y += p) {

            printf("block_x %d block_y %d\n", block_x, block_y);
            // now we iterate through the p rows and q columns of B
            for (int row = 0; row < p; row++) {
                for (int col = 0; col < q; col++) {
                    printf("col %d row %d\n", row, col);
                    c[block_y + row][block_x + col] = polar_mult(a[block_y / p][block_x / q], b[row][col]);
                }
            }
        }
    }
}