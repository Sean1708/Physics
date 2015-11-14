#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

long sum(long N);

int main(int argc, char* argv[]) {
    if (argc > 1) {
        char* N_str = argv[1];
        char* N_end = N_str;
        long N = strtol(N_str, &N_end, 10);

        if (*N_end != '\0' || *N_str == '\0') {
            fprintf(stderr, "Error: invalid digit '%c' in %s\n", *N_end, N_str);
            return 1;
        }
        if (N <= 0) {
            fprintf(stderr, "Error: will not sum negative numbers\n");
            return 2;
        }
        
        long total = sum(N);
        printf("Sum = %ld\n", total);
    } else {
        fprintf(stderr, "Error: must supply an argument.\n");
    }
}

long sum(long N) {
    long* arr = calloc(N, sizeof(long));
    for (long i = 0; i < N; i++) { arr[i] = i+1; }
    
    long total = 0;
    for (long i = 0; i < N; i++) { total += arr[i]; }

    free(arr);
    return total;
}
