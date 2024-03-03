#include "symnmf.h"

/**
 * Frees the cord matrix.
*/
void free_cord(double **cord, int row)
{
    int i;

    for(i = 0; i < row; i++) {
        free(cord[i]);
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
    for (i = 0; i < matrix->col; i++) {
        free(matrix->cord[i]);
    }
    free(matrix->cord);
    free(matrix);
}

/**
 * Frees an array of matrices terminated by a NULL pointer.
 */
void free_matrices(MATRIX **matrices)
{
    if (matrices == NULL) {
        return;
    }

    for (int i = 0; matrices[i] != NULL; i++) {
        if (matrices[i] != NULL) {
            free_matrix(matrices[i]);
        }
    }

    // Optionally, free the array of MATRIX pointers itself
    free(matrices);
}




/**
 * Creates a matrix struct in M(mxn).
*/
int init_matrix(MATRIX *matrix, int col, int row)
{
    double **cord;
    int i;

    if (matrix == NULL) {
        return 1;
    }
    cord = (double **)calloc(col, sizeof(double *));
    if (cord == NULL) {
        return 1;
    }

    for (i = 0; i < col; i++) {
        cord[i] = (double *)calloc(row, sizeof(double));
        if (cord[i] == NULL) {
            free_matrix(matrix);
            return 1;
        }
    }

    matrix->col = col;
    matrix->row = row;
    matrix->cord = cord;
    return 0;
}

/**
 * Returns the d of the given matrix row.
*/
double get_d(MATRIX *matrix, int row_index)
{
    int j;
    double d;

    d = 0;
    for (j = 0; j < matrix->row; j++) {
        d += matrix->cord[row_index][j];
    }
    return d;
}

/**
 * Prints the matrix.
*/
void print_matrix(MATRIX *matrix)
{
    int i, j, col, row;
    col = matrix->col;
    row = matrix->row;
    for (i = 0; i < col; i++) {
        for (j = 0; j < row; j++) {
            printf("%.4f", (matrix->cord)[i][j]);
            printf("%s", (j == row - 1) ? "" : ",");
        }
        printf("\n");
    }
}

/**
 * Returns the multiplication matrix of two matrices A,B where A is in R(mxn) and B is in R(nxm).
*/
MATRIX* matrix_product(MATRIX *A, MATRIX *B)
{
    int row_dim, col_dim, inner_dim, i, j, k;
    MATRIX *AB;

    row_dim = A->col;
    col_dim = B->row;
    inner_dim = A->row;
    AB = (MATRIX *)malloc(sizeof(MATRIX));
    if (A == NULL || B == NULL || init_matrix(AB, row_dim, col_dim) == 1) {
        free_matrix(AB);
        return NULL;
    };

    for (i = 0; i < row_dim; i++) {
        for (j = 0; j < col_dim; j++) {
            AB->cord[i][j] = 0;
            for (k = 0; k < inner_dim; k++) {
                AB->cord[i][j] += (A->cord[i][k])*(B->cord[k][j]);
            }
        }
    }
    return AB;
}

/**
 * Returns the transpose matrix of the given matrix A.
*/
MATRIX* matrix_transpose(MATRIX *A)
{
    int i, j;
    MATRIX *At;

    At = (MATRIX *)malloc(sizeof(MATRIX));
    if (A == NULL || init_matrix(At, A->row, A->col) == 1) {
        free_matrix(At);
        return NULL;
    };

    for (i = 0; i < A->col; i++) {
        for (j = 0; j < A->row; j++) {
            At->cord[j][i] = A->cord[i][j];
        }
    }
    return At;
}

/**
 * Returns the squared frobenius distance of two matrices in R(mxn).
*/
double frobenius_distance(MATRIX *A, MATRIX *B)
{
    int i, j;
    double cubes_sum;

    cubes_sum = 0.0;
    for (i = 0; i < A->col; i++) {
        for (j = 0; j < A->row; j++) {
            cubes_sum += pow(A->cord[i][j] - B->cord[i][j], 2);
        }
    }
    return cubes_sum;
}

/**
 * Calculates the normalized similarity matrix.
*/
int norm(MATRIX *X, MATRIX *W)
{
    int i, j, col_d;
    MATRIX *A, *D;

    A = (MATRIX *)malloc(sizeof(MATRIX));
    D = (MATRIX *)malloc(sizeof(MATRIX));
    if (init_matrix(A, X->col, X->col) == 1 || sym(X, A) == 1 || init_matrix(D, X->col, X->col) == 1 || ddg(X, D) == 1) {
        free_matrix(A);
        free_matrix(D);
        return 1;
    }

    col_d = A->col;
    for (i = 0; i < col_d; i++) {
        D->cord[i][i] = pow(D->cord[i][i], -0.5);
    }
    for (i = 0; i < col_d; i++) {
        for (j = 0; j < col_d; j++) {
            W->cord[i][j] = (D->cord[i][i])*(A->cord[i][j])*(D->cord[j][j]);
        }
    }

    return 0;
}

/**
 * Initializes the normalized similarity matrix W and populates it using the `norm` method.
 * $ret: 0=success || 1=fail
*/
int norm_print(MATRIX *X) {
    MATRIX *W;

    W = (MATRIX *)malloc(sizeof(MATRIX));
    if (init_matrix(W, X->col, X->col) == 1) {
        free(W);
        return 1;
    }
    norm(X, W);
    print_matrix(W);
    free_matrix(W);
    return 0;
}

/**
 * Initializes the similarity matrix A and populates it using the `sym` method.
 * $ret: 0=success || 1=fail
*/
int sym_print(MATRIX *X) {
    MATRIX *A;

    A = (MATRIX *)malloc(sizeof(MATRIX));
    if (init_matrix(A, X->col, X->col) == 1) {
        free(A);
        return 1;
    }
    sym(X, A);
    print_matrix(A);
    free_matrix(A);
    return 0;
}

/**
 * Initializes the diagonal d matrix D and populates it using the `ddg` method.
 * $ret: 0=success || 1=fail
*/
int ddg_print(MATRIX *X) {
    MATRIX *D;

    D = (MATRIX *)malloc(sizeof(MATRIX));
    if (init_matrix(D, X->col, X->col) == 1) {
        free(D);
        return 1;
    }
    ddg(X, D);
    print_matrix(D);
    free_matrix(D);
    return 0;
}


/**
 * Creates a matrix of coordinates (array of pointers to vectors, which themselves are arrays of doubles).
 * Uses dymanic arrays to handle memory allocation.
 * $ret: 0=success || 1=fail
*/
int parse(MATRIX *mat, int buffer_size, FILE *ofp)
{
    double v;
    char c;
    double *row, **cord, **tmp;
    int row_d, col_d, row_index;
    col_d = 0;
    row_index = 0;

    cord = (double **)calloc(buffer_size, sizeof(double *));
    if (cord == NULL) {
        free_cord(cord, col_d);
        return 1;
    }

    /* get data row_d */
    row_d = 0;

    while (fscanf(ofp, "%lf%c", &v, &c) == 2) {
        row_d++;
        if (c == '\n') {
            break;
        }
    }
    rewind(ofp);
    
    while (fscanf(ofp, "%lf%c", &v, &c) == 2) {
        if (row_index == 0) {
            row = (double *)calloc(row_d, sizeof(double));
            if (row == NULL) {
                free_cord(cord, col_d);
                return 1;
            }
            if (col_d >= buffer_size) {
                buffer_size *= 2; /* default growth factor == 2 */
                tmp = realloc(cord, buffer_size * sizeof(double *));
                if (tmp == NULL) {
                    free_cord(cord, col_d);
                    return 1;
                } else {
                    cord = tmp;
                }
            }
            cord[col_d] = row;
            col_d++;
        }
        row[row_index] = v;
        row_index++;
        if (c == '\n') {
            row_index = 0;
        }
    }

    tmp = realloc(cord, col_d * sizeof(double *));
    if (tmp == NULL) {
        free_cord(cord, col_d);
        return 1;
    } else {
        cord = tmp;
    }

    mat->cord = cord;
    mat->row = row_d;
    mat->col = col_d;
    return 0;

}

/**
 * Returns the euclidian distance of two vectors in R^d.
*/
double e_distance(double *p1, double *p2, int d)
{
    int i;
    double sum;

    sum = 0.0;
    for (i = 0; i < d; i++) {
        sum += pow(p1[i] - p2[i], 2);
    }
    return sqrt(sum);
}

/**
 * Calculates the similarity matrix.
 * The similarity matrix A in R(NxN) is defined as: a_ij = exp((||x_i-x_j||^2)/2) if i != j, 0 otherwise.
*/
int sym(MATRIX *X, MATRIX *A) 
{
    int i, j, col_d, d;

    col_d = X->col;
    d = X->row;
    for (i = 0; i < col_d; i++) {
        for (j = 0; j < col_d; j++) {
            if (i != j) {
                A->cord[i][j] = exp(-pow(e_distance(X->cord[i], X->cord[j], d), 2)/2);
            }
        }
    }
    return 0;
}

/**
 * Calculates the diagonal d matrix.
 * The d of vertex x_i in X is defined as: d_i = sum(a_ij) j=1,..,col_d.
*/
int ddg(MATRIX *X, MATRIX *D)
{
    int i;
    MATRIX *A;

    A = (MATRIX *)malloc(sizeof(MATRIX));
    if (init_matrix(A, X->col, X->col) == 1 || sym(X, A) == 1) {
        free_matrix(A);
        return 1;
    }

    for (i = 0; i < A->col; i++) {
        D->cord[i][i] = get_d(A, i);
    }

    return 0;
}

/**
 * Finds the Hn×k that solves: min||W − HHT||F for H>=0.
*/
int symnmf(MATRIX *W, MATRIX *H) 
{
    int col, row, i, j, k;
    MATRIX *WH, *Ht, *HHt, *HHtH, *prev_H;
    WH = Ht = HHt = HHtH = NULL;
    prev_H = (MATRIX *)malloc(sizeof(MATRIX));
    MATRIX* matrices[] = {WH, Ht, HHt, HHtH, prev_H};

    col = H->col;
    row = H->row;
    
    if (init_matrix(prev_H, H->col, H->row) == 1) {
        free_matrices(matrices);
        return 1;
    }

    for (i = 0; i < H->col; i++) {
        for (j = 0; j < H->row; j++) {
            prev_H->cord[i][j] = H->cord[i][j];
        }
    }

    for (k = 0; k < 300; k++) { /* default max iterations == 300 */
        WH = matrix_product(W, H);
        Ht = matrix_transpose(H);
        HHt = matrix_product(H, Ht);
        HHtH = matrix_product(HHt, H);
        if (WH == NULL || Ht == NULL || HHt == NULL || HHtH == NULL) {
            free_matrices(matrices);
            return 1;
        }

        for (i = 0; i < col; i++) {
            for (j = 0; j < row; j++) {
                H->cord[i][j] = (H->cord[i][j]) * (0.5 + 0.5*(WH->cord[i][j] / HHtH->cord[i][j]));
            }
        }
        if (frobenius_distance(H, prev_H) < 0.0001) { /* default epsilon == 0.0001 */
            break;
        }
        for (i = 0; i < H->col; i++) {
            for (j = 0; j < H->row; j++) {
                prev_H->cord[i][j] = H->cord[i][j];
            }
        }
    }
    
    return 0;
}

/**
 * Main flow:
 * 1. Gets the user input.
 * 2. print relevant matrix according to the passed goal.
 * $ret: 0=success || 1=fail
*/
int main(int argc, char **argv)
{
    char *goal;
    MATRIX *X = (MATRIX *)malloc(sizeof(MATRIX));
    FILE *ofp;

    if (argc != 3) {
        printf("An Error Has Occurred");
        return 1;
    }
    goal = argv[1];
    ofp = fopen(argv[2], "r");
    if (ofp == NULL) {
        free_matrix(X);
        return 1;
    }
    if (parse(X, 500, ofp) == 1) { /* default buffer size == 500 */
        fclose(ofp);
        return 1;
    }

    if (strcmp(goal, "sym") == 0) {
        if (sym_print(X) == 1) {
            free_matrix(X);
            printf("An Error Has Occurred");
            return 1;
        }
    }
    if (strcmp(goal, "ddg") == 0) {
        if (ddg_print(X) == 1) {
            free_matrix(X);
            printf("An Error Has Occurred");
            return 1;
        }
    }
    if (strcmp(goal, "norm") == 0) {
        if (norm_print(X) == 1) {
            free_matrix(X);
            printf("An Error Has Occurred");
            return 1;
        }
    }
    return 0;
}
