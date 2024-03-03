# ifndef SYMNMF_H_
# define SYMNMF_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int col;
    int row;
    double **cord;
} MATRIX;

int sym(MATRIX *X, MATRIX *A);
int ddg(MATRIX *X, MATRIX *D);
int norm(MATRIX *X, MATRIX *W);
int symnmf(MATRIX *W, MATRIX *H);
void free_matrix(MATRIX *matrix);
int init_matrix(MATRIX *matrix, int col, int row);

# endif
