#include<stdio.h>
#include<stdlib.h>
/*Keep in mind that this code assumes that all matrices are non-null and that their dimensions are correct. 
You may want to add additional error checking depending on your needs. Additionally, memory for the result 
matrix is allocated outside of the matrix_multiply function and should be freed after use to avoid memory leaks.*/


void matrix_multiply(double **A, int rowsA, int colsA,
                     double **B, int rowsB, int colsB,
                     double **result) 
{
    if (colsA != rowsB) 
    {
        printf("Error: Matrix dimensions do not match for multiplication!\n");
        return;
    }

    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            result[i][j] = 0; // initialize elements to 0
            for (int k = 0; k < colsA; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to print a matrix
void print_matrix(double **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Example usage of matrix_multiply
    double *A[] = {
        (double[]) {1, 2},
        (double[]) {3, 4}
    };
    
    double *B[] = {
        (double[]) {2, 0},
        (double[]) {1, 3}
    };
    
    double **result = malloc(2 * sizeof(double*));
    for (int i = 0; i < 2; i++) {
        result[i] = malloc(2 * sizeof(double));
    }
    
    matrix_multiply(A, 2, 2, B, 2, 2, result);
    print_matrix(result, 2, 2);

    // Free allocated memory for result matrix
    for (int i = 0; i < 2; i++) {
        free(result[i]);
    }
    free(result);

    return 0;
}