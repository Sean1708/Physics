#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    #pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int cur_thread = omp_get_thread_num();
        printf("Hello world from thread %d of %d!\n", cur_thread+1, nthreads);
    }

    return 0;
}
