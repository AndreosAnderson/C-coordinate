#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

typedef struct {
    int row;
    int col;
    double value;
} COOEntry;

void multiplyMatrix(double** m1, double** m2, double** result, int N);
double** allocateMatrix(int N);
void freeMatrix(double** matrix, int N);
void printMemoryAndCPUUsage();
COOEntry* convertToCOO(double** matrix, int N, double density, int* nnz);

#endif
