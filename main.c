#include <stdio.h>
#include "qc.h"
#include "qmath.h"
#include "utils.h"
#include <stdlib.h>

int main(int argc, char** argv) {


    init_gates();

    // we init a 4x4 matrix to test
    // H * I and I * H

    polar_t** res = malloc(4 * sizeof(polar_t*));
    for (int i = 0; i < 4; i++) {
        res[i] = malloc(4 * sizeof(polar_t));
    }

    polar_t** res2 = malloc(8 * sizeof(polar_t*));
    for (int i = 0; i < 8; i++) {
        res2[i] = malloc(8 * sizeof(polar_t));
    }

    printf("H * I\n");
    kronecker_product(H_gate.elements, I_gate.elements, res, 2, 2, 2, 2);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // print_polar(*(*(res + j) + i));
            print_polar(res[i][j]);
        }
        printf("\n");
    }
    printf("I * H\n");
    kronecker_product(I_gate.elements, H_gate.elements, res, 2, 2, 2, 2);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // print_polar(*(*(res + j) + i));
            print_polar(res[i][j]);
        }
        printf("\n");
    }
    printf("I * I\n");
    kronecker_product(I_gate.elements, I_gate.elements, res, 2, 2, 2, 2);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // print_polar(*(*(res + j) + i));
            print_polar(res[i][j]);
        }
        printf("\n");
    }
    printf("H * Z\n");
    kronecker_product(H_gate.elements, Z_gate.elements, res, 2, 2, 2, 2);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // print_polar(*(*(res + j) + i));
            print_polar(res[i][j]);
        }
        printf("\n");
    }

    printf("X * H * I\n");
    kronecker_product(X_gate.elements, H_gate.elements, res, 2, 2, 2, 2);
    kronecker_product(res, I_gate.elements, res2, 4, 4, 2, 2);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            print_polar(res2[i][j]);
        }
        printf("\n");
    }


    return 0;
}
