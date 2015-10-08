// Targets any C Standard later than C99.
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


double percent_err(double estimate, double actual);
double estimate_pi(long iterations);
void calculate_estimates(long start, long stop, FILE* file);


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("main <iterations>\n");
        printf("\t- calculate pi using a set number of iterations\n");
        printf("main <start> <stop> [output]\n");
        printf("\t- calculate pi for every number of iterations from start to stop\n");
        printf("\t- if output is given the results are saved to that file else they are printed to stdout\n");
        return 0;
    } else if (argc == 2) {
        long iterations = strtol(argv[1], NULL, 10);
        double pi = estimate_pi(iterations);
        printf("pi = %.15lf\n", pi);
        printf("error: %le%%\n", percent_err(pi, M_PI));
    } else if (argc == 3) {
        long start = strtol(argv[1], NULL, 10);
        long stop = strtol(argv[2], NULL, 10);
        calculate_estimates(start, stop, stdout);
    } else if (argc == 4) {
        long start = strtol(argv[1], NULL, 10);
        long stop = strtol(argv[2], NULL, 10);
        char* filename = argv[3];
        FILE* output = fopen(filename, "w");
        if (output == NULL) {
            printf("Error: could not open file %s", filename);
            return 1;
        }
        fprintf(stderr, "Warning: cancelling with Ctrl-C will leave %s open", filename);
        calculate_estimates(start, stop, output);
        fclose(output);
    } else {
        printf("Error: too many arguments, run with no arguments to see a usage message");
        return 2;
    }

    return 0;
}


double percent_err(double estimate, double actual) {
    // The closer actual and estimate get the less accurate this value gets.
    return fabs(100*(actual - estimate)/actual);
}

// Limited by number of iterations.
// Limited by use of double:
//     - long double or quad_t will provide extra accuracy at a small
//       performance loss
//     - using a multiple-precision floating point library (such as GNU's MPFR)
//       will provide theoretically infinite accuracy but incurs a significant
//       penalty
double estimate_pi(long iterations) {
    double pi = 0;
    for (long i = 0; i < iterations; i++) {
        double frac = (i - 0.5)/iterations;
        pi += 1/(1 + pow(frac, 2));
    }

    return 4*pi/iterations;
}

void calculate_estimates(long start, long stop, FILE* file) {
    fprintf(file, "N\tpi\terr (%%)\n");

    // ma_ variables track the values at which the most accurate pi is found
    long ma_iters = 0;
    double ma_pi = 0;
    double ma_err = INFINITY;  // this value must always be greater then the first err value

    for (long i = start; i < stop; i++) {
        double pi = estimate_pi(i);
        double err = percent_err(pi, M_PI);
        fprintf(file, "%ld\t%.15lf\t%le\n", i, pi, err);

        if (err < ma_err) {
            ma_err = err;
            ma_pi = pi;
            ma_iters = i;
        }
    }

    printf(
        "Most accurate:\n\tIterations: %ld\n\tValue: %le\n\tError: %le%%\n",
        ma_iters, ma_pi, ma_err
    );
}
