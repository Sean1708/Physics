#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int cur = 0; cur < size; cur++) {
        if (rank == cur) {
            printf("Hello from rank %d of %d!\n", rank+1, size);
        }
        int err = MPI_Barrier(MPI_COMM_WORLD);
        fprintf(stderr, "{rank: %d, err: %d}\n", rank, err);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
