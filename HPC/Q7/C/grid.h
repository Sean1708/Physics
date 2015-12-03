#ifndef grid_h
#define grid_h

#include <stdlib.h>
#include <math.h>

typedef struct {
    double* data;
    int rows;
    int cols;
} Grid;

Grid* grid_create(int rows, int cols);
void grid_destroy(Grid* grid);
void grid_copy(const Grid* source, Grid* destination);
double grid_get(Grid* grid, int row, int col);
int grid_cmp(const Grid* lhs, const Grid* rhs, double tolerance);

#endif
