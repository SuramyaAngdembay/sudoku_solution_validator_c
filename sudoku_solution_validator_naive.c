/**
 * Single-threaded Sudoku Solution Validator
 * 
 * Validates a Sudoku solution by checking all rows, columns, and 3x3 subgrids
 * sequentially. Prints validation result and execution time.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Example Sudoku puzzle (valid)
int sudoku[9][9] = {
    {6, 2, 4, 5, 3, 9, 1, 8, 7},
    {5, 1, 9, 7, 2, 8, 6, 3, 4},
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {4, 9, 6, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}
};

// Get current time in milliseconds
double get_time() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}

// Check if a 1D array contains all digits 1-9 exactly once
int is_valid_set(int *set) {
    int seen[10] = {0}; // 1-based indexing for digits 1-9
    for (int i = 0; i < 9; i++) {
        int num = set[i];
        if (num < 1 || num > 9 || seen[num]++) {
            return 0;
        }
    }
    return 1;
}

// Validate all rows
int validate_rows() {
    for (int i = 0; i < 9; i++) {
        if (!is_valid_set(sudoku[i])) {
            return 0;
        }
    }
    return 1;
}

// Validate all columns
int validate_columns() {
    for (int j = 0; j < 9; j++) {
        int column[9];
        for (int i = 0; i < 9; i++) {
            column[i] = sudoku[i][j];
        }
        if (!is_valid_set(column)) {
            return 0;
        }
    }
    return 1;
}

// Validate all 3x3 subgrids
int validate_subgrids() {
    for (int grid_row = 0; grid_row < 3; grid_row++) {
        for (int grid_col = 0; grid_col < 3; grid_col++) {
            int subgrid[9];
            int idx = 0;
            for (int i = grid_row * 3; i < grid_row * 3 + 3; i++) {
                for (int j = grid_col * 3; j < grid_col * 3 + 3; j++) {
                    subgrid[idx++] = sudoku[i][j];
                }
            }
            if (!is_valid_set(subgrid)) {
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    double start_time = get_time();

    int rows_valid = validate_rows();
    int cols_valid = validate_columns();
    int subgrids_valid = validate_subgrids();

    double end_time = get_time();

    if (rows_valid && cols_valid && subgrids_valid) {
        printf("Sudoku solution is valid.\n");
    } else {
        printf("Sudoku solution is invalid.\n");
    }
    printf("Time taken: %.3f milliseconds\n", end_time - start_time);

    return 0;
}
