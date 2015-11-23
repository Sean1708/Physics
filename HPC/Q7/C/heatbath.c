#include "matrix.h"

#include <stdio.h>
#include <math.h>

double matrix_get(Matrix* grid, int row, int col);
int coord_to_elem(double coord);
double matrix_get_coord(Matrix* grid, double row, double col);
void matrix_set_coord(Matrix* grid, double row, double col, double value);

const static double TOLERANCE = 0.0000000001;
const static double COORD_TO_ELEM_FACTOR = 10;

// int main(int argc, char* argv[]) {
int main() {
    int rows = 100;
    int cols = 100;
    Matrix* previous_state = matrix_create(rows, cols);
    Matrix* current_state = matrix_create(rows, cols);
    // Set the heat sources.
    matrix_set_coord(current_state, 5, 5, 10);
    matrix_set_coord(current_state, 4, 6, 7.2);
    matrix_set_coord(current_state, 7, 2.5, -1.2);

    matrix_print(current_state);

    while (!matrix_equals(previous_state, current_state, TOLERANCE)) {
        matrix_copy(current_state, previous_state);
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
                    double self = matrix_get(current_state, row, col);
                    double up = matrix_get(current_state, row-1, col);
                    double down = matrix_get(current_state, row+1, col);
                    double left = matrix_get(current_state, row, col-1);
                    double right = matrix_get(current_state, row, col+1);
                    double average = (self + up + down + left + right)/(double)5;
                    current_state->data[row][col] = average;
                }
            }
        }
    }

    matrix_print(current_state);
    fprintf(stderr, "(5.5, 5.5) = %.15lf\n", matrix_get_coord(current_state, 5.5, 5.5));

    matrix_destroy(previous_state);
    matrix_destroy(current_state);
    return 0;
}

// Get the temperature of an element in the matrix, accounting for the boundary conditions.
double matrix_get(Matrix* grid, int row, int col) {
    if (row < 0 || col < 0 || row >= grid->rows || col >= grid->cols) {
        // Heatbath of temperature 0K surrounds the grid.
        return 0;
    } else {
        return grid->data[row][col];
    }
}

int coord_to_elem(double coord) {
    return (int)lround(coord * COORD_TO_ELEM_FACTOR);
}

// Get an element of the matrix from coordinates.
//
// Since the matrix is 100x100 but the coordinates go from 0 to 10.
double matrix_get_coord(Matrix* grid, double row, double col) {
    return matrix_get(grid, coord_to_elem(row), coord_to_elem(col));
}

// Set an element of the matrix from a coordinate.
void matrix_set_coord(Matrix* grid, double row, double col, double value) {
    // Assumes row and col are in bounds.
    grid->data[coord_to_elem(row)][coord_to_elem(col)] = value;
}
