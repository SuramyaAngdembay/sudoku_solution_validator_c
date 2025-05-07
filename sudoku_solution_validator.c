/**
 * Multi-threaded Sudoku Solution Validator with Execution Time Measurement
 * 
 * Uses 27 threads (9 rows, 9 columns, 9 subgrids) to validate a Sudoku solution.
 * Measures and prints the time taken for validation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 27
#define SUBGRID_OFFSET 0
#define ROW_OFFSET 9
#define COL_OFFSET 18

int valid[NUM_THREADS] = {0}; // Global validation array

typedef struct {
    int row;
    int column;
} parameters;

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

void *validate_subgrid(void *param) {
    parameters *params = (parameters *)param;
    int row = params->row;
    int col = params->column;
    int validity[9] = {0};

    for (int i = row; i < row + 3; i++) {
        for (int j = col; j < col + 3; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || validity[num - 1]++) {
                free(param);
                pthread_exit(NULL);
            }
        }
    }
    valid[SUBGRID_OFFSET + (row / 3) * 3 + (col / 3)] = 1;
    free(param);
    pthread_exit(NULL);
}

void *validate_row(void *param) {
    parameters *params = (parameters *)param;
    int row = params->row;
    int validity[9] = {0};

    for (int j = 0; j < 9; j++) {
        int num = sudoku[row][j];
        if (num < 1 || num > 9 || validity[num - 1]++) {
            free(param);
            pthread_exit(NULL);
        }
    }
    valid[ROW_OFFSET + row] = 1;
    free(param);
    pthread_exit(NULL);
}

void *validate_column(void *param) {
    parameters *params = (parameters *)param;
    int col = params->column;
    int validity[9] = {0};

    for (int i = 0; i < 9; i++) {
        int num = sudoku[i][col];
        if (num < 1 || num > 9 || validity[num - 1]++) {
            free(param);
            pthread_exit(NULL);
        }
    }
    valid[COL_OFFSET + col] = 1;
    free(param);
    pthread_exit(NULL);
}

int main() {
    double start_time = get_time();
    pthread_t threads[NUM_THREADS];
    int thread_idx = 0;

    // Create threads
    for (int i = 0; i < 9; i++) {
        // Subgrids
        if (i % 3 == 0) {
            for (int j = 0; j < 9; j += 3) {
                parameters *data = malloc(sizeof(parameters));
                data->row = i;
                data->column = j;
                pthread_create(&threads[thread_idx++], NULL, validate_subgrid, data);
            }
        }
        // Rows
        parameters *row_data = malloc(sizeof(parameters));
        row_data->row = i;
        row_data->column = 0;
        pthread_create(&threads[thread_idx++], NULL, validate_row, row_data);
        // Columns
        parameters *col_data = malloc(sizeof(parameters));
        col_data->row = 0;
        col_data->column = i;
        pthread_create(&threads[thread_idx++], NULL, validate_column, col_data);
    }

    // Wait for threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Check results
    int is_valid = 1;
    for (int i = 0; i < NUM_THREADS; i++) {
        if (valid[i] == 0) {
            is_valid = 0;
            break;
        }
    }

    double end_time = get_time();
    printf("Sudoku solution is %s.\n", is_valid ? "valid" : "invalid");
    printf("Time taken: %.3f milliseconds\n", end_time - start_time);

    return is_valid ? EXIT_SUCCESS : EXIT_FAILURE;
}
