#include <stdio.h>
#include <stdlib.h>

void matrix_multiply();
void matrix_transpose();
void gauss_jordan_inverse();
void one_shot_learning();

/*=====================================================================================================*/

int main(int argc, char *argv[]) 
{
    if (argc != 3) 
    {
        return 1;
    }

    // TODO: Read matrices X and Y from the training file
    double **X_train;
    double *Y_train;
    int rows_train;
    int cols_train;
    read_matrix(argv[1], &X_train, &Y_train, &rows_train, &cols_train);

    // TODO: Implement the one-shot learning algorithm to get weights
    double **weights;
    one_shot_learning(X_train, Y_train, rows_train, cols_train, &weights);

    // TODO: Read matrix X' from the data file
    double **X_test;
    int rows_test, cols_test;
    read_X_matrix(argv[2], &X_test, &rows_test, &cols_test);

    // TODO: Estimate prices Y' = X'W
    double **Y_test;
    matrix_multiply(X_test, weights, rows_test, cols_test, 1, &Y_test);
    printf("Estimated Prices:\n");
    for(int i = 0; i < rows_test; i++) {
        printf("%lf\n", Y_test[i][0]);
    }

    // Clean up dynamically allocated memory
    // ...

    return 0;
}

void read_matrix(char *filename, double ***X, double **Y, int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s for reading.\n", filename);
        exit(1);
    }

    fscanf(file, "%d %d", rows, cols);
    
    // Allocate and read X matrix
    *X = (double **)malloc(*rows * sizeof(double *));
    for (int i = 0; i < *rows; i++) {
        (*X)[i] = (double *)malloc(*cols * sizeof(double));
        for (int j = 0; j < *cols; j++) {
            fscanf(file, "%lf", &(*X)[i][j]);
        }
    }

    // Allocate and read Y matrix
    *Y = (double *)malloc(*rows * sizeof(double));
    for (int i = 0; i < *rows; i++) {
        fscanf(file, "%lf", &(*Y)[i]);
    }

    fclose(file);
}

void read_X_matrix(char *filename, double ***X, int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s for reading.\n", filename);
        exit(1);
    }

    fscanf(file, "%d %d", rows, cols);
    
    // Allocate and read X matrix
    *X = (double **)malloc(*rows * sizeof(double *));
    for (int i = 0; i < *rows; i++) {
        (*X)[i] = (double *)malloc(*cols * sizeof(double));
        for (int j = 0; j < *cols; j++) {
            fscanf(file, "%lf", &(*X)[i][j]);
        }
    }

    fclose(file);
}

/*=====================================================================================================*/

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

/*=====================================================================================================*/

void matrix_transpose(double **A, int rows, int cols, double ***transpose) 
{
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

/*=====================================================================================================*/

void gauss_jordan_inverse(double **M, int n, double ***inverse) 
{
    // Initializing N as an identity matrix
    *inverse = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        (*inverse)[i] = (double *)calloc(n, sizeof(double));
        (*inverse)[i][i] = 1;
    }

    // Processing the matrix as per the algorithm
    for (int p = 0; p < n; p++) {
        double f = M[p][p];
        
        // Dividing Mp and Np by f
        for (int j = 0; j < n; j++) {
            M[p][j] /= f;
            (*inverse)[p][j] /= f;
        }

        for (int i = p+1; i < n; i++) {
            f = M[i][p];
            
            // Subtracting Mp*f from Mi and Np*f from Ni
            for (int j = 0; j < n; j++) {
                M[i][j] -= M[p][j] * f;
                (*inverse)[i][j] -= (*inverse)[p][j] * f;
            }
        }
    }

    // Back substitution
    for (int p = n-1; p >= 0; p--) {
        for (int i = p-1; i >= 0; i--) {
            double f = M[i][p];
            
            // Subtracting Mp*f from Mi and Np*f from Ni
            for (int j = 0; j < n; j++) {
                M[i][j] -= M[p][j] * f;
                (*inverse)[i][j] -= (*inverse)[p][j] * f;
            }
        }
    }
}

/*=====================================================================================================*/

void one_shot_learning(double **X, double *Y, int rows, int cols, double **weights) 
{
    double **X_transpose, **result, **inv_result, **XY_result;

    // 1. Compute X_transpose * X
    matrix_transpose(X, rows, cols, &X_transpose);
    matrix_multiply(X_transpose, X, cols, rows, cols, &result);

    // 2. Compute the inverse of the above using Gauss-Jordan
    gauss_jordan_inverse(result, cols, &inv_result);
    
    // 3. Compute X_transpose * Y
    matrix_multiply_vector(X_transpose, Y, cols, rows, &XY_result);
    
    // 4. Multiply the two results to get weights
    matrix_multiply(inv_result, XY_result, cols, cols, 1, weights);
    
    // Clean up dynamically allocated memory
    // ...
}

// Modify the matrix_multiply function to handle vector multiplication if necessary
void matrix_multiply_vector(double **A, double *B, int rowsA, int colsA, double ***C) 
{
    *C = (double **)malloc(rowsA * sizeof(double *));
    for (int i = 0; i < rowsA; i++) {
        (*C)[i] = (double *)malloc(sizeof(double));
        (*C)[i][0] = 0;
        for (int j = 0; j < colsA; j++) {
            (*C)[i][0] += A[i][j] * B[j];
        }
    }
}






