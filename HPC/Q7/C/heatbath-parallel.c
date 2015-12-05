#include "grid.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>


int coord_to_elem(double coord);
int row_coord_to_rank(int rows_per_rank, double coord);


const static double TOLERANCE = 0.0000000001;
const static double COORD_TO_ELEM_FACTOR = 10;
const static int TOTAL_ROWS = 100;
const static int TOTAL_COLS = 100;
const static int FIRST_ROW_TAG = 1;
const static int LAST_ROW_TAG = 2;


int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank = -1;
    int rank_err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank_err != 0) {
        fprintf(stderr, "unable to obtain rank number\n");
        exit(rank_err);
    }

    int size = -1;
    int size_err = MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size_err != 0) {
        fprintf(stderr, "Rank %d: unable to obtain communicator size\n", rank);
        exit(size_err);
    }

    if (size != 1 && size != 2 && size != 4 && size != 5 && size != 10) {
        fprintf(stderr, "Rank %d: number of ranks (%d) is not factor of 100\n", rank, size);
        exit(EXIT_FAILURE);
    }


    // Number of rows and cols that each rank handles.
    // Each rank has all columns but only a subset of rows.
    int rank_cols = TOTAL_COLS;
    int rank_rows = TOTAL_ROWS/size;

    Grid* local_previous_state = grid_create(rank_rows, rank_cols);
    Grid* local_current_state = grid_create(rank_rows, rank_cols);

    // Set the heatsource at (5, 5).
    if (rank == row_coord_to_rank(rank_rows, 5)) {
        int full_grid_row = coord_to_elem(5);
        int row = full_grid_row - (rank*rank_rows);
        int ncols = local_current_state->cols;
        int col = coord_to_elem(5);
        local_current_state->data[row*ncols + col] = (double)10.0;
    }

    // Set the heatsource at (4, 6).
    if (rank == row_coord_to_rank(rank_rows, 4)) {
        int full_grid_row = coord_to_elem(4);
        int row = full_grid_row - (rank*rank_rows);
        int ncols = local_current_state->cols;
        int col = coord_to_elem(6);
        local_current_state->data[row*ncols + col] = (double)7.2;
    }

    // Set the heatsource at (7, 2.5).
    if (rank == row_coord_to_rank(rank_rows, 7)) {
        int full_grid_row = coord_to_elem(7);
        int row = full_grid_row - (rank*rank_rows);
        int ncols = local_current_state->cols;
        int col = coord_to_elem(2.5);
        local_current_state->data[row*ncols + col] = (double)-1.2;
    }


    int all_ranks_equal = 0;
    do {
        // There are two ways to handle the heatsources:
        //
        //     1. Each time we update an element check whether it's a heatsource and skip it if it is.
        //     2. Reset the heatsources at the start of each iteration.
        //
        // Since we have the concept of a current state and a previous state (current state is
        // updated based on the information in the previous state) #2 seems like the cleaner
        // solution.

        // This is the only point in the loop where local_previous_state is changed.
        grid_copy(local_current_state, local_previous_state);


        // Send first row to previous rank. Rank 0 has noone to send to.
        MPI_Request first_row_send_request;
        if (rank != 0) {
            int send_err = MPI_Isend(
                local_previous_state->data,
                rank_cols,
                MPI_DOUBLE,
                rank-1,
                FIRST_ROW_TAG,
                MPI_COMM_WORLD,
                &first_row_send_request
            );
            if (send_err != 0) {
                fprintf(stderr, "Rank %d: could not attempt to send first row\n", rank);
                exit(send_err);
            }
        }

        // Send last row to next rank. Rank size-1 has noone to send to.
        MPI_Request last_row_send_request;
        if (rank != size-1) {
            int send_err = MPI_Isend(
                local_previous_state->data + (rank_rows - 1)*rank_cols,  // address of first element in last row.
                rank_cols,
                MPI_DOUBLE,
                rank+1,
                LAST_ROW_TAG,
                MPI_COMM_WORLD,
                &last_row_send_request
            );
            if (send_err != 0) {
                fprintf(stderr, "Rank %d: could not attempt to send last row\n", rank);
                exit(send_err);
            }
        }

        // Receive first row from previous rank (is previous rank's last row). Rank 0 has noone to receive from.
        MPI_Request first_row_receive_request;
        double* received_first_row = calloc((size_t)rank_cols, sizeof(double));
        if (rank == 0) {
            for (int col = 0; col < rank_cols; col++) {
                // Boundary is maintained at 0.
                received_first_row[col] = 0;
            }
        } else {
            int recv_err = MPI_Irecv(
                received_first_row,
                rank_cols,
                MPI_DOUBLE,
                rank-1,
                LAST_ROW_TAG,
                MPI_COMM_WORLD,
                &first_row_receive_request
            );
            if (recv_err != 0) {
                fprintf(stderr, "Rank %d: could not attempt to receive first row\n", rank);
                exit(recv_err);
            }
        }

        // Receive last row from next rank (is previous rank's first row). Rank size-1 has noone to receive from.
        MPI_Request last_row_receive_request;
        double* received_last_row = calloc((size_t)rank_cols, sizeof(double));
        if (rank == size-1) {
            for (int col = 0; col < rank_cols; col++) {
                // Boundary is maintained at 0.
                received_last_row[col] = 0;
            }
        } else {
            int recv_err = MPI_Irecv(
                received_last_row,
                rank_cols,
                MPI_DOUBLE,
                rank+1,
                FIRST_ROW_TAG,
                MPI_COMM_WORLD,
                &last_row_receive_request
            );
            if (recv_err != 0) {
                fprintf(stderr, "Rank %d: could not receive last row\n", rank);
                exit(recv_err);
            }
        }


        // Update non-boundary data.
        for (int row = 1; row < (rank_rows - 1); row++) {
            for (int col = 0; col < rank_cols; col++) {
                // Use data from the previous state (since that is not changing) to update the
                // current state. Can happily change the heatsources since they are changed back
                // before the current state is copied into the previous state.
                double self = grid_get(local_previous_state, row, col);
                double up = grid_get(local_previous_state, row-1, col);
                double down = grid_get(local_previous_state, row+1, col);
                double left = grid_get(local_previous_state, row, col-1);
                double right = grid_get(local_previous_state, row, col+1);
                double average = (self + up + down + left + right)/(double)5;
                local_current_state->data[row*rank_cols + col] = average;
            }
        }

        // Update the first row.
        if (rank != 0) MPI_Wait(&first_row_receive_request, MPI_STATUS_IGNORE);
        int update_row = 0;
        for (int col = 0; col < rank_cols; col++) {
            double self = grid_get(local_previous_state, update_row, col);
            double up = received_first_row[col];
            double down = grid_get(local_previous_state, update_row+1, col);
            double left = grid_get(local_previous_state, update_row, col-1);
            double right = grid_get(local_previous_state, update_row, col+1);
            double average = (self + up + down + left + right)/(double)5;
            local_current_state->data[update_row*rank_cols + col] = average;
        }

        // Update last row.
        if (rank != size-1) MPI_Wait(&last_row_receive_request, MPI_STATUS_IGNORE);
        update_row = rank_rows-1;
        for (int col = 0; col < rank_cols; col++) {
            double self = grid_get(local_previous_state, update_row, col);
            double up = grid_get(local_previous_state, update_row-1, col);
            double down = received_last_row[col];
            double left = grid_get(local_previous_state, update_row, col-1);
            double right = grid_get(local_previous_state, update_row, col+1);
            double average = (self + up + down + left + right)/(double)5;
            local_current_state->data[update_row*rank_cols + col] = average;
        }

        // Clean up sending memory.
        if (rank != 0) MPI_Wait(&first_row_send_request, MPI_STATUS_IGNORE);
        if (rank != size-1) MPI_Wait(&last_row_send_request, MPI_STATUS_IGNORE);


        // Reset the heatsources before the current and previous states are compared.

        // Set the heatsource at (5, 5).
        if (rank == row_coord_to_rank(rank_rows, 5)) {
            int full_grid_row = coord_to_elem(5);
            int source_row = full_grid_row - (rank*rank_rows);
            int ncols = local_current_state->cols;
            int source_col = coord_to_elem(5);
            local_current_state->data[source_row*ncols + source_col] = (double)10.0;
        }

        // Set the heatsource at (4, 6).
        if (rank == row_coord_to_rank(rank_rows, 4)) {
            int full_grid_row = coord_to_elem(4);
            int source_row = full_grid_row - (rank*rank_rows);
            int ncols = local_current_state->cols;
            int source_col = coord_to_elem(6);
            local_current_state->data[source_row*ncols + source_col] = (double)7.2;
        }

        // Set the heatsource at (7, 2.5).
        if (rank == row_coord_to_rank(rank_rows, 7)) {
            int full_grid_row = coord_to_elem(7);
            int source_row = full_grid_row - (rank*rank_rows);
            int ncols = local_current_state->cols;
            int source_col = coord_to_elem(2.5);
            local_current_state->data[source_row*ncols + source_col] = (double)-1.2;
        }


        // If current state is identical to previous state FOR ALL RANKS then the system is in
        // equillibrium. So for each rank broadcast whether the states are equal, if any are not
        // equal then keep running the loop.
        int rank_equal = grid_cmp(local_previous_state, local_current_state, TOLERANCE);
        all_ranks_equal = 1;
        for (int bcast_rank = 0; bcast_rank < size; bcast_rank++) {
            int bcast_rank_equal = rank_equal;
            int bcast_err = MPI_Bcast(&bcast_rank_equal, 1, MPI_INT, bcast_rank, MPI_COMM_WORLD);
            if (bcast_err != 0) {
                fprintf(stderr, "Rank %d: broadcasting operation failed\n", rank);
                exit(bcast_err);
            }

            // If any of the ranks is not equal then we have to keep looping.
            if (!bcast_rank_equal) {
                all_ranks_equal = 0;
                break;
            }
        }
    } while(!all_ranks_equal);


    // Check the value for point (5.5, 5.5).
    if (rank == row_coord_to_rank(rank_rows, 5.5)) {
        int full_grid_row = coord_to_elem(5.5);
        int row = full_grid_row - (rank*rank_rows);
        int col = coord_to_elem(5.5);
        printf("(5.5, 5.5) = %lf\n", grid_get(local_current_state, row, col));
    }

    grid_destroy(local_previous_state);
    grid_destroy(local_current_state);

    MPI_Finalize();


    return EXIT_SUCCESS;
}


int coord_to_elem(double coord) {
    return (int)lround(coord * COORD_TO_ELEM_FACTOR);
}

int row_coord_to_rank(int rows_per_rank, double coord) {
    int row = coord_to_elem(coord);
    return row/rows_per_rank;
}
