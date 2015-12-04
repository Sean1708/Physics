#include "grid.h"

#include <stdio.h>
#include <math.h>

int coord_to_elem(double coord);
double grid_get_coord(Grid* grid, double row, double col);
void grid_set_coord(Grid* grid, double row, double col, double value);

const static double TOLERANCE = 0.0000000001;
const static double COORD_TO_ELEM_FACTOR = 10;

int main() {
    int rows = 100;
    int cols = 100;
    Grid* previous_state = grid_create(rows, cols);
    Grid* current_state = grid_create(rows, cols);
    // Set the heat sources.
    grid_set_coord(current_state, 5, 5, 10);
    grid_set_coord(current_state, 4, 6, 7.2);
    grid_set_coord(current_state, 7, 2.5, -1.2);

    do {
        grid_copy(current_state, previous_state);
        for (int row = 0; row < current_state->rows; row++) {
            for (int col = 0; col < current_state->cols; col++) {
                if (
                    (coord_to_elem(5) == row && coord_to_elem(5) == col) ||
                    (coord_to_elem(4) == row && coord_to_elem(6) == col) ||
                    (coord_to_elem(7) == row && coord_to_elem(2.5) == col)
                ) {
                    // Don't change the heat sources.
                    continue;
                } else {
                    double self = grid_get(previous_state, row, col);
                    double up = grid_get(previous_state, row-1, col);
                    double down = grid_get(previous_state, row+1, col);
                    double left = grid_get(previous_state, row, col-1);
                    double right = grid_get(previous_state, row, col+1);
                    double average = (self + up + down + left + right)/(double)5;
                    current_state->data[row*cols + col] = average;
                }
            }
        }
    } while (!grid_cmp(previous_state, current_state, TOLERANCE));

    fprintf(stderr, "(5.5, 5.5) = %lf\n", grid_get_coord(current_state, 5.5, 5.5));

    grid_destroy(previous_state);
    grid_destroy(current_state);
    return 0;
}

int coord_to_elem(double coord) {
    return (int)lround(coord * COORD_TO_ELEM_FACTOR);
}

// Get an element of the grid from coordinates.
//
// Since the grid is 100x100 but the coordinates go from 0 to 10.
double grid_get_coord(Grid* grid, double row, double col) {
    return grid_get(grid, coord_to_elem(row), coord_to_elem(col));
}

// Set an element of the grid from a coordinate.
void grid_set_coord(Grid* grid, double row, double col, double value) {
    // Assumes row and col are in bounds.
    int ncols = grid->cols;
    grid->data[coord_to_elem(row)*ncols + coord_to_elem(col)] = value;
}
