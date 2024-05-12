#include <stdio.h>
#include <stdlib.h>

/*takes as input a matrix M and its dimension n. It modifies M and creates the inverse matrix.
Ensure that your matrix M is non-singular (its determinant is non-zero) before attempting to find its inverse. 
Otherwise, the function may not work correctly because a non-singular matrix does not have an inverse.*/

void gauss_jordan_inverse(double **M, int n, double ***inverse) {
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

// A function to print matrices
void print_matrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%.2f\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Example usage:
    double *M[] = {
        (double[]){4, 7},
        (double[]){2, 6}
    };
    
    double **inverse;
    gauss_jordan_inverse(M, 2, &inverse);
    
    printf("Inverse Matrix:\n");
    print_matrix(inverse, 2);
    
    // Free allocated memory
    for (int i = 0; i < 2; i++) {
        free(inverse[i]);
    }
    free(inverse);
    
    return 0;
}