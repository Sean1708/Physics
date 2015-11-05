// Required for glibc to use [s/d]rand48.
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


typedef struct {
    // Stored in row-major order so accessed with [row][column].
    double** rows;
    // Number of columns of the square matrix
    // (and therefore also number of rows).
    size_t N;
} Matrix;

typedef struct {
    clock_t stdf77;
    clock_t fastf77;
    clock_t blas;
} Timings;


Matrix* matrix_create(size_t N);
void matrix_destroy(Matrix* mat);
void matrix_fill_rand(Matrix* mat);

Timings time_single_matrix(size_t N);


const char* USAGE = "matmul [N]\n\n"
"If N is omitted then time each algorithm with random NxN matrices where\n"
"    N = {100, 128, 250, 256, 500, 512, 1000, 1024}\n\n"
"If N is numeric then time each algorithm with an NxN matrix.";

int main(int argc, char* argv[]) {
    // Seed once for entire program.
    srand48((long)time(NULL));

    if (argc <= 1) {
        // Time with N={100,128,250,256,500,512,1000,1024}, 10 times each.
    } else if (argc == 2) {
        // Time with given N.
        char* N_str = argv[1];
        char* end = N_str;
        size_t N = (size_t)strtol(N_str, end, 10);
        if (*end != '\0' || *N_str == '\0') {
            fprintf(stderr, "Error: invalid digit '%c'\n", *end);
            return 1;
        } else if (N <= 0) {
            fprintf(stderr, "Error: can not have zero element matrix\n");
            return 2;
        } else {
	    Timings results = time_single_matrix(N);
        }
    } else {
        // Something's wrong.
    }


    return 0;
}


// Allocate memory for and create a matrix.
Matrix* matrix_create(size_t N) {
    Matrix* mat = calloc(1, sizeof(Matrix));
    mat->N = N;
    mat->rows = calloc(N, sizeof(double*));
    for (size_t r = 0; r < N; r++) {
        mat->rows[r] = calloc(N, sizeof(double));
    }
    matrix_fill_rand(mat);
    return mat;
}

// Free memory from the matrix.
void matrix_destroy(Matrix* mat) {
    for (size_t r; r < mat->N; r++) {
        free(mat->rows[r]);
    }
    free(mat->rows);
    free(mat);
}

// Fill a Matrix with random double-precision floats.
void matrix_fill_rand(Matrix* mat) {
    for (size_t r = 0; r < mat->N; r++) {
        for (size_t c = 0; c < mat->N; c++) {
            // Set element of matrix to random double between 0 and 100.
            mat->rows[r][c] = drand48()*100;
        }
    }
}

// Time the three different matrix multiplication methods using a single size.
//
// Each method is timed ten times and an average taken.
Timings time_single_matrix(size_t N) {
    Timings results = {0};
    Matrix* a = matrix_create(N);
    Matrix* b = matrix_create(N);
    Matrix* res = matrix_create(N);

    // Standard F77
    clock_t start = clock();
    for (int _i = 0; _i < 10; _i++) {
	    matrix_multiply_stdf77(a, b, res);
    }
    results.stdf77 = (start - clock())/(clock_t)10;

    // Fast F77
    start = clock();
    for (int _i = 0; _i < 10; _i++) {
        matrix_multiply_fastf77(a, b, res);
    }
    results.fastf77 = (start - clock())/(clock_t)10;

    // BLAS
    start = clock();
    for (int _i = 0; _i < 10; _i++) {
        matrix_multiply_blas(a, b, res);
    }
    results.blas = (start - clock())/(clock_t)10;

    matrix_destroy(a);
    matrix_destroy(b);
    matrix_destroy(res);


    return results;
}
