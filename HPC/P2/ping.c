#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = rank;
    if (rank == 0) {
        int err = MPI_Send(&data, 1, MPI_INT, 1, 42, MPI_COMM_WORLD);
        fprintf(stderr, "{rank: %d, data: %d, err: %d}\n", rank, data, err);
    } else if (rank == 1) {
        MPI_Status _status;
        int err = MPI_Recv(&data, 1, MPI_INT, 0, 42, MPI_COMM_WORLD, &_status);
        fprintf(stderr, "{rank: %d, data: %d, err: %d}\n", rank, data, err);
    }

    printf("{rank: %d, data: %d}\n", rank, data);

    data = rank;
    if (rank == 0) {
        MPI_Status _status;
        int err = MPI_Recv(&data, 1, MPI_INT, 1, 24, MPI_COMM_WORLD, &_status);
        fprintf(stderr, "{rank: %d, data: %d, err: %d}\n", rank, data, err);
    } else if (rank == 1) {
        int err = MPI_Send(&data, 1, MPI_INT, 0, 24, MPI_COMM_WORLD);
        fprintf(stderr, "{rank: %d, data: %d, err: %d}\n", rank, data, err);
    }

    printf("{rank: %d, data: %d}\n", rank, data);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
