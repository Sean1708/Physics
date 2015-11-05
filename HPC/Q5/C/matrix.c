#include "matrix.h"

// Allocate memory for and create a matrix.
Matrix* matrix_create(size_t N) {
    Matrix* mat = calloc(1, sizeof(Matrix));
    mat->N = N;
    mat->rows = calloc(N, sizeof(double*));
    for (size_t row = 0; row < N; row++) {
        mat->rows[row] = calloc(N, sizeof(double));
    }
    matrix_fill_rand(mat);
    return mat;
}

// Free memory from the matrix.
void matrix_destroy(Matrix* mat) {
    for (size_t row = 0; row < mat->N; row++) {
        free(mat->rows[row]);
    }
    free(mat->rows);
    free(mat);
}

// Print out a matrix.
void matrix_print(Matrix* mat) {
    printf("   %lux%lu\n", mat->N, mat->N);
    for (size_t row = 0; row < mat->N; row++) {
        printf("%lu:", row+1);
        for (size_t col = 0; col < mat->N; col++) {
            printf("\t%lf", mat->rows[row][col]);
        }
        printf("\n");
    }
}

// Fill a Matrix with random double-precision floats.
void matrix_fill_rand(Matrix* mat) {
    for (size_t row = 0; row < mat->N; row++) {
        for (size_t col = 0; col < mat->N; col++) {
            // Set element of matrix to random double between 0 and 100.
            mat->rows[row][col] = drand48()*100;
        }
    }
}

// See if each element of the matrices are equal to within tol.
bool matrix_equals(const Matrix* lhs, const Matrix* rhs, double tol) {
    // Assumes equally sized square matrices.
    size_t N = lhs->N;
    for (size_t row = 0; row < N; row++) {
        for (size_t col = 0; col < N; col++) {
            if (fabs(lhs->rows[row][col] - rhs->rows[row][col]) > tol) {
                return false;
            }
        }
    }

    return true;
}

// Zeros each element of a matrix.
void matrix_zero(Matrix* mat) {
    for (size_t row = 0; row < mat->N; row++) {
        for (size_t col = 0; col < mat->N; col++) {
            mat->rows[row][col] = 0.0;
        }
    }
}

void matrix_multiply_stdf77(const Matrix* lhs, const Matrix* rhs, Matrix* res) {
    // Assumes equally sized square matrices.
    size_t N = lhs->N;
    for (size_t row = 0; row < N; row++) {
        for (size_t col = 0; col < N; col++) {
            double temp = 0.0;
            for (size_t inner = 0; inner < N; inner++) {
                temp += lhs->rows[row][inner] * rhs->rows[inner][col];
            }
            res->rows[row][col] = temp;
        }
    }
}

void matrix_multiply_fastf77(const Matrix* lhs, const Matrix* rhs, Matrix* res) {
    // Assumes equally sized square matrices.
    size_t N = lhs->N;
    matrix_zero(res);
    for (size_t row = 0; row < N; row++) {
        for (size_t inner = 0; inner < N; inner++) {
            double temp = lhs->rows[row][inner];
            for (size_t col = 0; col < N; col++) {
                res->rows[row][col] += rhs->rows[inner][col]*temp;
            }
        }
    }
}

void matrix_multiply_blas(const Matrix* lhs, const Matrix* rhs, Matrix* res) {
    printf("%p %p %p\n", lhs, rhs, res);
}
