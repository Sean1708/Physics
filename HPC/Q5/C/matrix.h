#ifndef matrix_h
#define matrix_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <cblas.h>

typedef struct {
    // Stored in row-major order so accessed with [row*N + column].
    // Stored as double* rather than double** for interoperability with BLAS.
    double* data;
    // Number of rows of the square matrix (and therefore also number of columns).
    size_t N;
} Matrix;

Matrix* matrix_create(size_t N);
void matrix_fill_rand(Matrix* mat);
void matrix_destroy(Matrix* mat);
void matrix_print(Matrix* mat);
bool matrix_equals(const Matrix* lhs, const Matrix* rhs, double tol);
void matrix_zero(Matrix* mat);
void matrix_multiply_stdf77(const Matrix* lhs, const Matrix* rhs, Matrix* res);
void matrix_multiply_fastf77(const Matrix* lhs, const Matrix* rhs, Matrix* res);
void matrix_multiply_blas(const Matrix* lhs, const Matrix* rhs, Matrix* res);

#endif
