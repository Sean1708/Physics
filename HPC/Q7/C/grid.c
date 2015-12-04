#include "grid.h"


Grid* grid_create(int rows, int cols) {
    Grid* grid = calloc(1, sizeof(Grid));
    grid->rows = rows;
    grid->cols = cols;
    grid->data = calloc((size_t)(rows*cols), sizeof(double));

    return grid;
}

void grid_destroy(Grid* grid) {
    free(grid->data);
    free(grid);
}

void grid_copy(const Grid* source, Grid* destination) {
    // Assumes grids are the same size.
    int nelems = source->rows*source->cols;
    for (int i = 0; i < nelems; i++) {
        destination->data[i] = source->data[i];
    }
}

// Get the temperature of an element in the matrix, accounting for the boundary conditions.
double grid_get(Grid* grid, int row, int col) {
    if (row < 0 || col < 0 || row >= grid->rows || col >= grid->cols) {
        // Heatbath of temperature 0C surrounds the grid.
        return 0;
    } else {
        int ncols = grid->cols;
        return grid->data[row*ncols + col];
    }
}

int grid_cmp(const Grid* lhs, const Grid* rhs, double tolerance) {
    // Assumes grids are same size.
    int nelems = lhs->rows * rhs->rows;

    for (int i = 0; i < nelems; i++) {
        double diff = fabs(lhs->data[i] - rhs->data[i]);
        // If elements are not equal return false.
        if (diff > tolerance) { return 0; }
    }
    // If all elements were the same then return true.
    return 1;
}
