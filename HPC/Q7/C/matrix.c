// Adapted from Question 5 (the matrix multiplication one).
#include "matrix.h"

// Allocate memory for and create a matrix.
Matrix* matrix_create(int rows, int cols) {
    Matrix* mat = calloc(1, sizeof(Matrix));
    mat->rows = rows;
    mat->cols = cols;
    mat->data = calloc((size_t)(rows*cols), sizeof(double));
    matrix_fill(mat, 0.0);
    return mat;
}

// Free memory from the matrix.
void matrix_destroy(Matrix* mat) {
    free(mat->data);
    free(mat);
}

// Print out a matrix.
void matrix_print(const Matrix* mat) {
    int ncols = mat->cols;
    // Convert to unsigned long for machines on which size_t is int.
    printf("   %dx%d\n", mat->rows, mat->cols);
    for (int row = 0; row < mat->rows; row++) {
        printf("%d:", row+1);
        for (int col = 0; col < mat->cols; col++) {
            printf("\t%lf", mat->data[row*ncols + col]);
        }
        printf("\n");
    }
}

// Fill a matrix with the given constant.
void matrix_fill(Matrix* mat, double value) {
    int ncols = mat->cols;
    for (int row = 0; row < mat->rows; row++) {
        for (int col = 0; col < mat->cols; col++) {
            mat->data[row*ncols + col] = value;
        }
    }
}

// Copy elements from src into dest.
void matrix_copy(const Matrix* src, Matrix* dest) {
    // Assumes matrices are same size.
    int ncols = src->cols;
    for (int row = 0; row < src->rows; row++) {
        for (int col = 0; col < src->cols; col++) {
            dest->data[row*ncols + col] = src->data[row*ncols + col];
        }
    }
}

// See if each element of the matrices are equal to within tol.
bool matrix_equals(const Matrix* lhs, const Matrix* rhs, double tol) {
    // Assumes equally sized matrices.
    int ncols = lhs->cols;
    for (int row = 0; row < lhs->rows; row++) {
        for (int col = 0; col < lhs->cols; col++) {
            if (fabs(lhs->data[row*ncols + col] - rhs->data[row*ncols + col]) > tol) {
                return false;
            }
        }
    }

    return true;
}
