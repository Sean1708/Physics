// Required for glibc to use [s/d]rand48.
// #define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "matrix.h"


typedef struct {
    clock_t stdf77;
    clock_t fastf77;
    clock_t blas;
} Timings;


Timings time_single_matrix(size_t N);
void print_timings(Timings results, size_t N);
void time_multiple_matrices(void);
void check_matrix_multiplication(size_t N, double tol);


// TODO: make all %ld into %lu and change longs to unsigned longs
// $ matmul
//
//     Times the three algorithms for NxN matrices of:
//
//         N = {100, 128, 250, 256, 500, 512, 1000, 1024}
//
//     Each algorithm is timed 10 times and an average is taken.
//
// $ matmul N
//
//     Times the three algorithms for NxN matrices. Each algorithm is timed 10
//     times and an average is taken.
//
// $ matmul N tol
//
//     Fills two NxN matrices with random numbers, prints them to stdout then
//     prints the results of the three algorithms to stdout before checking that
//     the three result matrices are equal to within tol.
int main(int argc, char* argv[]) {
    // Seed once for entire program.
    srand48((long)time(NULL));

    if (argc <= 1) {
        // Time with N={100,128,250,256,500,512,1000,1024}, 10 times each.
        time_multiple_matrices();
    } else if (argc == 2) {
        // Time with given N.
        char* N_str = argv[1];
        char* N_end = N_str;
        long long_N = strtol(N_str, &N_end, 10);
        if (*N_end != '\0' || *N_str == '\0') {
            fprintf(stderr, "Error: invalid digit '%c' in %s\n", *N_end, N_str);
            return 1;
        }
        if (long_N <= 0) {
            fprintf(stderr, "Error: can not have zero or negative element matrix\n");
            return 2;
        }
        size_t N = (size_t)long_N;

        Timings results = time_single_matrix(N);
        print_timings(results, N);
    } else {
        char* N_str = argv[1];
        char* N_end = N_str;
        long long_N = strtol(N_str, &N_end, 10);
        if (*N_end != '\0' || *N_str == '\0') {
            fprintf(stderr, "Error: invalid digit '%c' in %s\n", *N_end, N_str);
            return 1;
        }
        if (long_N <= 0) {
            fprintf(stderr, "Error: can not have zero or negative element matrix\n");
            return 2;
        }
        size_t N = (size_t)long_N;

        char* tol_str = argv[2];
        char* tol_end = tol_str;
        double tol = fabs(strtod(tol_str, &tol_end));
        if (*tol_end != '\0' || *tol_str == '\0') {
            fprintf(stderr, "Error: invalid digit '%c' in %s\n", *tol_end, tol_str);
            return 3;
        }

        check_matrix_multiplication(N, tol);
    }


    return 0;
}


// Time the three different matrix multiplication methods using a single size.
//
// Each method is timed ten times and an average taken.
Timings time_single_matrix(size_t N) {
    printf("Timing %ldx%ld matrix...\n", (long)N, (long)N);
    Timings results = {0, 0, 0};
    Matrix* lhs = matrix_create(N);
    Matrix* rhs = matrix_create(N);
    Matrix* res = matrix_create(N);

    clock_t start = 0;
    clock_t sum = 0;
    // Standard F77
    for (int _i = 0; _i < 10; _i++) {
        start = clock();
	    matrix_multiply_stdf77(lhs, rhs, res);
        sum += clock() - start;
    }
    results.stdf77 = sum/(clock_t)10;

    // Fast F77
    start = 0;
    sum = 0;
    for (int _i = 0; _i < 10; _i++) {
        start = clock();
        matrix_multiply_fastf77(lhs, rhs, res);
        sum += clock() - start;

    }
    results.fastf77 = sum/(clock_t)10;

    // BLAS
    start = 0;
    sum = 0;
    for (int _i = 0; _i < 10; _i++) {
        start = clock();
        matrix_multiply_blas(lhs, rhs, res);
        sum += clock() - start;
    }
    results.blas = sum/(clock_t)10;

    matrix_destroy(lhs);
    matrix_destroy(rhs);
    matrix_destroy(res);


    return results;
}

void print_timings(Timings results, size_t N) {
    printf("For a %lux%lu matrix the algorithms took:\n", (unsigned long)N, (unsigned long)N);
    printf("\t Std F77: %lfs\n", (double)results.stdf77/CLOCKS_PER_SEC);
    printf("\tFast F77: %lfs\n", (double)results.fastf77/CLOCKS_PER_SEC);
    printf("\t    BLAS: %lfs\n", (double)results.blas/CLOCKS_PER_SEC);
}

void time_multiple_matrices() {
    size_t Ns[] = {100, 128, 250, 256, 500, 512, 1000, 1024};
    for (size_t i = 0; i < (size_t)(sizeof(Ns)/sizeof(Ns[0])); i++) {
        Timings results = time_single_matrix(Ns[i]);
        print_timings(results, Ns[i]);
    }
}

void check_matrix_multiplication(size_t N, double tol) {
    Matrix* lhs = matrix_create(N);
    Matrix* rhs = matrix_create(N);
    Matrix* stdf77 = matrix_create(N);
    Matrix* fastf77 = matrix_create(N);
    Matrix* blas = matrix_create(N);

    printf("LHS matrix:\n");
    matrix_print(lhs);
    printf("\n");
    printf("RHS matrix:\n");
    matrix_print(rhs);
    printf("\n");

    printf("Testing Std F77...\n");
    matrix_multiply_stdf77(lhs, rhs, stdf77);
    printf("Result:\n");
    matrix_print(stdf77);
    printf("\n");

    printf("Testing Fast F77...\n");
    matrix_multiply_fastf77(lhs, rhs, fastf77);
    printf("Result:\n");
    matrix_print(fastf77);
    printf("\n");

    printf("Testing BLAS...\n");
    matrix_multiply_blas(lhs, rhs, blas);
    printf("Result:\n");
    matrix_print(blas);
    printf("\n");

    printf("Std F77 == Fast F77?\t%s\n", matrix_equals(stdf77, fastf77, tol) ? "yes" : "no");
    printf("Std F77 == BLAS?\t%s\n", matrix_equals(stdf77, blas, tol) ? "yes" : "no");
    printf("Fast F77 == BLAS?\t%s\n", matrix_equals(fastf77, blas, tol) ? "yes" : "no");

    matrix_destroy(lhs);
    matrix_destroy(rhs);
    matrix_destroy(stdf77);
    matrix_destroy(fastf77);
    matrix_destroy(blas);
}
