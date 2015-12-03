// Adapted from Question 5 (the matrix multiplication one).
#ifndef matrix_h
#define matrix_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    // Stored in row-major order so accessed with [row*num_cols + column].
    double* data;
    int rows;
    int cols;
} Matrix;

Matrix* matrix_create(int rows, int cols);
void matrix_destroy(Matrix* mat);
void matrix_print(const Matrix* mat);
void matrix_fill(Matrix* mat, double value);
void matrix_copy(const Matrix* src, Matrix* dest);
bool matrix_equals(const Matrix* lhs, const Matrix* rhs, double tol);

#endif
