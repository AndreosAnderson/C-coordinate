#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "matrix_ops.h"

#define ITERATIONS 10

int main() {
    int N = 1024;
    double density = 0.1;

    double** m1 = allocateMatrix(N);
    double** m2 = allocateMatrix(N);
    double** result = allocateMatrix(N);
    int nnzA, nnzB;

    // Randomize matrices
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m1[i][j] = ((double)rand() / RAND_MAX) * 10.0;
            m2[i][j] = ((double)rand() / RAND_MAX) * 10.0;
        }
    }

    // Convert matrices to COO format
    COOEntry* cooA = convertToCOO(m1, N, density, &nnzA);
    COOEntry* cooB = convertToCOO(m2, N, density, &nnzB);

    // Initialize result matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = 0;
        }
    }

    double min_time = 1e9;
    double max_time = 0;
    double total_time = 0;
    double times[ITERATIONS];

    // Benchmarking
    for (int i = 0; i < ITERATIONS; i++) {
        clock_t start_time = clock();
        multiplyMatrix(m1, m2, result, N);
        double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;

        times[i] = elapsed_time;
        total_time += elapsed_time;
        if (elapsed_time < min_time) {
            min_time = elapsed_time;
        }
        if (elapsed_time > max_time) {
            max_time = elapsed_time;
        }
    }

    double average_time = total_time / ITERATIONS;

    double sum_squared_diffs = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        sum_squared_diffs += pow(times[i] - average_time, 2);
    }
    double standard_deviation = sqrt(sum_squared_diffs / ITERATIONS);

    printf("Minimum time: %f seconds\n", min_time);
    printf("Maximum time: %f seconds\n", max_time);
    printf("Average time: %f seconds\n", average_time);
    printf("Standard deviation: %f seconds\n", standard_deviation);

    printMemoryAndCPUUsage();

    // Free allocated resources
    free(cooA);
    free(cooB);
    freeMatrix(m1, N);
    freeMatrix(m2, N);
    freeMatrix(result, N);

    return 0;
}
