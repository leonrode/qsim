#include <stdio.h>
#include <math.h>
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include "layer.h"
#include "qc.h"

gate_t* I_gate;
gate_t* X_gate;
gate_t* H_gate;
gate_t* Z_gate;

void print_polar(polar_t p) {
    printf("(%f)exp(i[%f])\n", p.r, p.theta);
}

void print_cart(cart_t c) {
    printf("(%f) + (%f)i\n", c.a, c.b);
}


void copy_matrix(polar_t** src, polar_t** dst, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

void print_matrix(polar_t** matrix, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f exp(%f) ", matrix[i][j].r, matrix[i][j].theta);
            // print_polar(matrix[i][j]);
        }
        printf("\n");
    }
}

int longest_operation_name_in_layer(layer_t* layer) {
    int longest = 0;
    for (int i = 0; i < layer->n_operations; i++) {
        if (strlen(layer->operations[i]->gate->name) > longest) {
            longest = strlen(layer->operations[i]->gate->name);
        }
    }
    return longest;
}

int longest_operation_name_in_qc(qc_t* qc) {
    int longest = 0;
    for (int i = 0; i < qc->n_layers; i++) {
        if (longest_operation_name_in_layer(qc->layers[i]) > longest) {
            longest = longest_operation_name_in_layer(qc->layers[i]);
        }
    }
    return longest;
}

void _generate_random_matrix(polar_t*** matrix, int m, int n) {
    *matrix = calloc(m, sizeof(polar_t*));
    for (int i = 0; i < m; i++) {
        (*matrix)[i] = calloc(n, sizeof(polar_t));
        for (int j = 0; j < n; j++) {
            (*matrix)[i][j] = (polar_t) {rand() % 100, rand() % 100};
        }
    }
}

char* decimal_to_binary(int decimal, int length) {
    char* binary = calloc(length + 1, sizeof(char)); // +1 for null terminator
    for (int i = 0; i < length; i++) {
        binary[i] = (decimal & (1 << (length - 1 - i))) ? '1' : '0';
    }
    binary[length] = '\0';
    return binary;
}