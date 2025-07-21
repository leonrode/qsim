#ifndef UTILS_H
#define UTILS_H

#include "qmath.h"
#include "layer.h"
#include "qc.h"

void print_polar(polar_t p);
void print_cart(cart_t c);

// src and dest are m x n matrices
void copy_matrix(polar_t** src, polar_t** dst, int m, int n);

void print_matrix(polar_t** matrix, int m, int n);
int longest_operation_name_in_layer(layer_t* layer);
int longest_operation_name_in_qc(qc_t* qc);

// allocate memory for a m * n matrix and fill it with random values
void _generate_random_matrix(polar_t*** matrix, int m, int n);

char* decimal_to_binary(int decimal, int length);


#endif