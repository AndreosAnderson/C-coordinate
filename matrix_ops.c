#include <stdlib.h>
#include <windows.h>
#include <psapi.h>
#include "matrix_ops.h"

void multiplyMatrix(double** m1, double** m2, double** result, int N) {
    // Prosta wersja mnożenia macierzy bez blokowania
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0;
            for (int k = 0; k < N; k++) {
                sum += m1[i][k] * m2[k][j];
            }
            result[i][j] = sum;  // Ustawiamy wynik bezpośrednio
        }
    }
}

double** allocateMatrix(int N) {
    double** matrix = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        matrix[i] = (double*)malloc(N * sizeof(double));
    }
    return matrix;
}

void freeMatrix(double** matrix, int N) {
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void printMemoryAndCPUUsage() {
    PROCESS_MEMORY_COUNTERS memCounter;
    BOOL result = GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter));
    if (result) {
        printf("Memory usage: %ld KB\n", memCounter.WorkingSetSize / 1024);
    }

    FILETIME creationTime, exitTime, kernelTime, userTime;
    if (GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime)) {
        ULARGE_INTEGER kernel, user;
        kernel.LowPart = kernelTime.dwLowDateTime;
        kernel.HighPart = kernelTime.dwHighDateTime;
        user.LowPart = userTime.dwLowDateTime;
        user.HighPart = userTime.dwHighDateTime;

        double system_cpu_time = (double)kernel.QuadPart / 1e7;
        double user_cpu_time = (double)user.QuadPart / 1e7;

        printf("User CPU time: %f seconds\n", user_cpu_time);
        printf("System CPU time: %f seconds\n", system_cpu_time);
    }
}

COOEntry* convertToCOO(double** matrix, int N, double density, int* nnz) {
    *nnz = 0;
    // Count non-zero entries
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] != 0) (*nnz)++;
        }
    }

    COOEntry* coo = (COOEntry*)malloc(*nnz * sizeof(COOEntry));
    int index = 0;
    // Populate COO structure
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] != 0) {
                coo[index].row = i;
                coo[index].col = j;
                coo[index].value = matrix[i][j];
                index++;
            }
        }
    }
    return coo;
}
