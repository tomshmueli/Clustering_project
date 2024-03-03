#include "symnmf.h"

/**
 * Frees the points matrix.
*/
void free_points(double **points, int n)
{
    int i;

    for(i = 0; i < n; i++) {
        free(points[i]);
    }
}

/**
 * Frees the matrix.
*/
void free_matrix(MATRIX *matrix)
{
    int i;
    if (matrix == NULL) {
        return;
    }
    for (i = 0; i < matrix->m; i++) {
        free(matrix->points[i]);
    }
    free(matrix->points);
    free(matrix);
}

/**
 * Creates a matrix struct in M(mxn).
*/
int init_matrix(MATRIX *matrix, int m, int n)
{
    double **points;
    int i;

    if (matrix == NULL) {
        return 1;
    }
    points = (double **)calloc(m, sizeof(double *));
    if (points == NULL) {
        return 1;
    }

    for (i = 0; i < m; i++) {
        points[i] = (double *)calloc(n, sizeof(double));
        if (points[i] == NULL) {
            free_matrix(matrix);
            return 1;
        }
    }

    matrix->m = m;
    matrix->n = n;
    matrix->points = points;
    return 0;
}

/**
 * Copies the the source matrix into the destintation matrix.
*/
void copy_matrix(MATRIX *dest, MATRIX *src)
{
    int i, j;

    for (i = 0; i < src->m; i++) {
        for (j = 0; j < src->n; j++) {
            dest->points[i][j] = src->points[i][j];
        }
    }
}

/**
 * Returns the degree of the given matrix row.
*/
double get_row_degree(MATRIX *matrix, int row_index)
{
    int j;
    double degree;
    
    degree = 0;
    for (j = 0; j < matrix->n; j++) {
        degree += matrix->points[row_index][j];
    }
    return degree;
}

/**
 * Prints the matrix.
*/
void print_matrix(MATRIX *matrix)
{
    int i, j, m, n;
    m = matrix->m;
    n = matrix->n;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            printf("%.4f", (matrix->points)[i][j]);
            if (!(j == n-1)) {
                printf(",");
            }
        }
        printf("\n");
    }
}

/**
 * Returns the multiplication matrix of two matrices A,B where A is in R(mxn) and B is in R(nxm).
*/
MATRIX* multiply_matrices(MATRIX *A, MATRIX *B)
{
    int row_dim, col_dim, inner_dim, i, j, k;
    MATRIX *AB;

    row_dim = A->m;
    col_dim = B->n;
    inner_dim = A->n;
    AB = (MATRIX *)malloc(sizeof(MATRIX));
    if (A == NULL || B == NULL || init_matrix(AB, row_dim, col_dim) == 1) {
        free_matrix(AB);
        return NULL;
    };

    for (i = 0; i < row_dim; i++) {
        for (j = 0; j < col_dim; j++) {
            AB->points[i][j] = 0;
            for (k = 0; k < inner_dim; k++) {
                AB->points[i][j] += (A->points[i][k])*(B->points[k][j]);
            }
        }
    }
    return AB;
}

/**
 * Returns the transpose matrix of the given matrix A.
*/
MATRIX* transpose_matrix(MATRIX *A)
{
    int i, j;
    MATRIX *At;

    At = (MATRIX *)malloc(sizeof(MATRIX));
    if (A == NULL || init_matrix(At, A->n, A->m) == 1) {
        free_matrix(At);
        return NULL;
    };

    for (i = 0; i < A->m; i++) {
        for (j = 0; j < A->n; j++) {
            At->points[j][i] = A->points[i][j];
        }
    }
    return At;
}

/**
 * Returns the squared frobenius distance of two matrices in R(mxn).
*/
double calculate_squared_frobenius_distance(MATRIX *A, MATRIX *B)
{
    int i, j;
    double cubes_sum;

    cubes_sum = 0.0;
    for (i = 0; i < A->m; i++) {
        for (j = 0; j < A->n; j++) {
            cubes_sum += pow(A->points[i][j] - B->points[i][j], 2);
        }
    }
    return cubes_sum;
}
