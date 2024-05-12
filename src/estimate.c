#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void TheLearnedOne();
void Transpose();
void MatrixMultiply();
void Invert();
void VectorMultiply();

/*=======================================================================================================*/

int main(int argc, char *argv[]) 
{
    if (argc != 3) 
    {
        return 1;
    }

    double **X;
    double *Y;
    int cols;
    int rows;
   
    FILE *file = fopen(argv[1], "r");
    if (!file) 
    {
        return 1;
    }

    char train[10];                                       
    fscanf(file, "%s", train);
    if (strcmp(train, "train") != 0)
    {
        fclose(file);
        return 1;
    }

    fscanf(file, "%d %d", &cols, &rows);

    X = (double **)malloc(rows * sizeof(double *));           
    Y = (double *)malloc(rows * sizeof(double));             

    for (int i = 0; i < rows; i++) 
    {
        X[i] = (double *)malloc((cols + 1) * sizeof(double));
        X[i][0] = 1; 

        for (int j = 1; j < cols + 1; j++) 
        {
            fscanf(file, "%lf", &X[i][j]);
        }
        fscanf(file, "%lf", &Y[i]);
    }
    fclose(file);

/*-------------------------------------------------------------------------------------------------------*/

    double *W = NULL;
    W = (double *)malloc((cols + 1) * sizeof(double));
    TheLearnedOne(X, Y, rows, cols + 1, W);

/*-------------------------------------------------------------------------------------------------------*/
    
    FILE *primefile = fopen(argv[2], "r");
    if (!primefile) 
    {
        return 1;
    }

    char data[10];                                            
    fscanf(primefile, "%s", data);
    if (strcmp(data, "data") != 0)
    {
        fclose(primefile);
        return 1;
    }
    
    int colsprime;
    int rowsprime;
    fscanf(primefile, "%d %d", &colsprime, &rowsprime);
    if (colsprime != cols)
    {
        fclose(primefile);
        return 1;
    }

    double **Xprime;
    Xprime = (double **)malloc(rowsprime * sizeof(double *));  
    for (int i = 0; i < rowsprime; i++) 
    {
        (Xprime)[i] = (double *)malloc((colsprime + 1) * sizeof(double));
        (Xprime)[i][0] = 1; 

        for (int j = 1; j < colsprime + 1; j++) 
        {
            fscanf(primefile, "%lf", &Xprime[i][j]);
        }
    }
    fclose(primefile);

/*-------------------------------------------------------------------------------------------------------*/

    double *Yprime;
    Yprime = (double *)malloc(rowsprime * sizeof(double));

    VectorMultiply(Xprime, W, rowsprime, cols + 1, Yprime);

    for (int i = 0; i < rowsprime; i++) 
    {
        if (Yprime[i] < 0)
        {
            printf("%d\n", (int)(Yprime[i] - 0.5));
        }
        else
        {
            printf("%d\n", (int)(Yprime[i] + 0.5));
        }
    }
    
/*-------------------------------------------------------------------------------------------------------*/

    for (int i = 0; i < rows; i++) 
    {
        free(X[i]);
    }
    for (int i = 0; i < rowsprime; i++) 
    {
        free(Xprime[i]);
    }
    free(X);
    free(Y);
    free(W);
    free(Xprime);
    free(Yprime);

    return 0;
}

/*=======================================================================================================*/

void TheLearnedOne(double **X, double *Y, int rows, int cols, double *W) 
{
    // 1) Transpose X
    double **Xt; 
    Xt = (double **)malloc(cols * sizeof(double *));
    for(int i = 0; i < cols; i++) 
    {
        Xt[i] = (double *)malloc(rows * sizeof(double));
    }
    Transpose(X, rows, cols, Xt);

    
    // 2) Matrix Multiply Xt * X
    double **XtX; 
    XtX = (double **)malloc(cols * sizeof(double *));
    for (int i = 0; i < cols; i++) 
    {
        XtX[i] = (double *)malloc((cols) * sizeof(double));
    }
    MatrixMultiply(Xt, cols, rows, X, rows, cols, XtX);


    // 3) Invert Xt * X
    double **Inverse; 
    Inverse = (double **)malloc(cols * sizeof(double *));
    for (int i = 0; i < cols; i++) 
    {
        Inverse[i] = (double *)calloc(cols, sizeof(double));
        Inverse[i][i] = 1;
    }
    Invert(XtX, cols, Inverse);

    
    // 4) Vector Multiply Xt * Y
    double *XtY;
    XtY = (double *)malloc(cols * sizeof(double));
    VectorMultiply(Xt, Y, cols, rows, XtY);


    // 5) Vector Multiply to get W
    VectorMultiply(Inverse, XtY, cols, cols, W);


    // Free Memory
    for (int i = 0; i < cols; i++)
    {
        free(Xt[i]);
        free(XtX[i]);
        free(Inverse[i]);
    }
    free(Xt);
    free(XtX);
    free(Inverse);
    free(XtY);
}

/*=======================================================================================================*/

void Transpose(double **X, int rows, int cols, double **Xt) 
{
    for (int i = 0; i < rows; i++) 
    {
        for (int j = 0; j < cols; j++) 
        {
            Xt[j][i] = X[i][j];
        }
    }
}

/*=======================================================================================================*/

void MatrixMultiply(double **M1, int rows1, int cols1, double **M2, int rows2, int cols2, double **M3) 
{
    if (cols1 != rows2) 
    {
        printf("Incompatible Matrix Dimensions");
        return;
    }

    for (int i = 0; i < rows1; i++) 
    {
        for (int j = 0; j < cols2; j++) 
        {
            M3[i][j] = 0;           
            for (int x = 0; x < cols1; x++) 
            {
                M3[i][j] += M1[i][x] * M2[x][j];
            }
        }
    }
}

/*=======================================================================================================*/

void Invert(double **XtX, int dimension, double **inverse) 
{
    for (int x = 0; x < dimension; x++) 
    {
        double z = XtX[x][x];
        
        for (int j = 0; j < dimension; j++) 
        {
            XtX[x][j] = XtX[x][j] / z;
            inverse[x][j] = inverse[x][j] / z;
        }

        for (int i = x + 1; i < dimension; i++) 
        {
            z = XtX[i][x];
            
            for (int j = 0; j < dimension; j++) 
            {
                XtX[i][j] = XtX[i][j] - (XtX[x][j] * z);
                inverse[i][j] = inverse[i][j] - (inverse[x][j] * z);
            }
        }
    }

    for (int x = dimension - 1; x >= 0; x--) 
    {
        for (int i = x - 1; i >= 0; i--)
        {
            double z = XtX[i][x];
            
            for (int j = 0; j < dimension; j++) 
            {
                XtX[i][j] = XtX[i][j] - (XtX[x][j] * z);
                inverse[i][j] = inverse[i][j] - (inverse[x][j] * z);
            }
        }
    }
}

/*=======================================================================================================*/

void VectorMultiply(double **M1, double *M2, int rows, int cols, double *M3) 
{
    for (int i = 0; i < rows; i++) 
    {
        M3[i] = 0;
        for (int j = 0; j < cols; j++) 
        {
            M3[i] += M1[i][j] * M2[j];
        }
    }
}