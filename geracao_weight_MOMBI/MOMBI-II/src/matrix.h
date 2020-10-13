#ifndef _MATRIX_
#define _MATRIX_

#include <stdio.h>

void EMO_mprint(FILE *fp, const char *s, double *A, int n, int m);
void EMO_matmul(double *a, double *b, double *c, int n, int m, int l);
int EMO_minverse(double *B, double *A, int n, int nthread, int metodo, int tridiag);

#endif

