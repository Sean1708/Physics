#include "matrix.h"

// Allocate memory for and create a matrix.
Matrix* matrix_create(size_t N) {
    Matrix* mat = calloc(1, sizeof(Matrix));
    mat->N = N;
    mat->data = calloc(N*N, sizeof(double));
    matrix_fill_rand(mat);
    return mat;
}

// Free memory from the matrix.
void matrix_destroy(Matrix* mat) {
    free(mat->data);
    free(mat);
}

// Print out a matrix.
void matrix_print(Matrix* mat) {
    printf("   %lux%lu\n", mat->N, mat->N);
    size_t N2 = mat->N * mat->N;
    for (size_t row = 0; row < N2; row += mat->N) {
        printf("%lu:", row+1);
        for (size_t col = 0; col < mat->N; col++) {
            printf("\t%lf", mat->data[row+col]);
        }
        printf("\n");
    }
}

// Fill a Matrix with random double-precision floats.
void matrix_fill_rand(Matrix* mat) {
    size_t N2 = mat->N * mat->N;
    for (size_t row = 0; row < N2; row += mat->N) {
        for (size_t col = 0; col < mat->N; col++) {
            // Set element of matrix to random double between 0 and 100.
            mat->data[row+col] = drand48()*100;
        }
    }
}

// See if each element of the matrices are equal to within tol.
bool matrix_equals(const Matrix* lhs, const Matrix* rhs, double tol) {
    // Assumes equally sized square matrices.
    size_t N2 = lhs->N * lhs->N;
    for (size_t row = 0; row < N2; row += lhs->N) {
        for (size_t col = 0; col < lhs->N; col++) {
            if (fabs(lhs->data[row+col] - rhs->data[row+col]) > tol) {
                return false;
            }
        }
    }

    return true;
}

// Zeros each element of a matrix.
void matrix_zero(Matrix* mat) {
    size_t N2 = mat->N * mat->N;
    for (size_t row = 0; row < N2; row += mat->N) {
        for (size_t col = 0; col < mat->N; col++) {
            mat->data[row+col] = 0.0;
        }
    }
}

void matrix_multiply_stdf77(const Matrix* lhs, const Matrix* rhs, Matrix* res) {
    // Assumes equally sized square matrices.
    size_t N2 = lhs->N * lhs->N;
    // Increase row by lhs->N each loop so a multiplication isn't needed in the
    // loop.
    for (size_t row = 0; row < N2; row += lhs->N) {
        for (size_t col = 0; col < lhs->N; col++) {
            double temp = 0.0;
            for (size_t inner = 0; inner < lhs->N; inner++) {
                // inner*rhs->N is needed because inner increases by one but the
                // row needs to be increased by rhs->N.
                temp += lhs->data[row+inner] * rhs->data[inner*rhs->N + col];
            }
            res->data[row+col] = temp;
        }
    }
}

void matrix_multiply_fastf77(const Matrix* lhs, const Matrix* rhs, Matrix* res) {
    // Assumes equally sized square matrices.
    size_t N2 = lhs->N * lhs->N;
    // Algorithm assumes that res starts out zeroed but we can't guarantee that
    // it is called as such.
    matrix_zero(res);
    for (size_t row = 0; row < N2; row += lhs->N) {
        for (size_t inner = 0; inner < lhs->N; inner++) {
            double temp = lhs->data[row+inner];
            for (size_t col = 0; col < lhs->N; col++) {
                res->data[row+col] += rhs->data[inner*rhs->N + col]*temp;
            }
        }
    }
}

void matrix_multiply_blas(const Matrix* lhs, const Matrix* rhs, Matrix* res) {
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans, CblasNoTrans,
        (blasint)lhs->N, (blasint)rhs->N, (blasint)rhs->N,
        1.0,
        lhs->data, (blasint)lhs->N,
        rhs->data, (blasint)rhs->N,
        0.0,
        res->data, (blasint)res->N
    );
}
