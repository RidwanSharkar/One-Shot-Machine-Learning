#include<stdio.h>
#include<stdlib.h>

/*The transposed matrix is dynamically allocated within the function and should be freed after use.
This function takes a matrix and its dimensions as input and returns the transposed matrix. 
It also allocates memory for the transposed matrix, so don't forget to free it once you are done using it.*/

void matrix_transpose(double **A, int rows, int cols, double ***transpose) {
    // Allocate memory for the transposed matrix
    *transpose = (double **)malloc(cols * sizeof(double *));
    for(int i = 0; i < cols; i++) {
        (*transpose)[i] = (double *)malloc(rows * sizeof(double));
    }

    // Perform the transposition
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            (*transpose)[j][i] = A[i][j];
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
    // Example usage of matrix_transpose
    double *A[] = {
        (double[]) {1, 2, 3},
        (double[]) {4, 5, 6}
    };
    
    double **transpose;
    matrix_transpose(A, 2, 3, &transpose);
    print_matrix(transpose, 3, 2);

    // Free allocated memory for transposed matrix
    for (int i = 0; i < 3; i++) {
        free(transpose[i]);
    }
    free(transpose);

    return 0;
}