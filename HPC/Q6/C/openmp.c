#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

void time_kahan(size_t N);
double kahan(size_t N, double* A, double* B);

int main(int argc, char* argv[]) {
    size_t Ns[] = {1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000, 2000000, 5000000};
    size_t num_Ns = (size_t)(sizeof(Ns)/sizeof(Ns[0]));
    for (size_t i = 0; i < num_Ns; i++) {
        size_t N = Ns[i];
        time_kahan(N);
    }

    return 0;
}

void time_kahan(size_t N) {
    double* A = calloc(N, sizeof(double));
    double* B = calloc(N, sizeof(double));

    double start = omp_get_wtime();
    double sum = kahan(N, A, B);
    double stop = omp_get_wtime();
    double diff = (stop - start);

    printf("       N: %lu\n", (unsigned long)N);
    printf("     sum: %le\n", sum);
    printf("time (s): %lf\n", diff);
    printf("\n");

    free(A);
    free(B);
}

double kahan(size_t N, double* A, double* B) {
    double sum = 0;
    #pragma omp parallel for shared(A, B) reduction(+:sum)
    for (size_t i = 0; i < N; i++) {
        A[i] = (double)i;
        B[i] = A[i]*A[i];
        A[i] += sqrt(fabs(sin(B[i])))*2.34;
        sum += A[i];
    }

    return sum;
}
