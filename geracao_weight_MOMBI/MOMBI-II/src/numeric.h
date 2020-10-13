
#ifndef _NUMERIC_H
#define _NUMERIC_H

#include "list.h"

#define PI 3.1415926535897932384626433832795

#define max(A,B) ((A)>(B) ? (A) : (B))  //ver funcion fmin:  double fmin(double x, double y); en math
#define min(A,B) ((A)<(B) ? (A) : (B))

double EMO_dmin(int *idx, double *arr, int *filter, int size);
double EMO_dmax(int *idx, double *arr, int *filter, int size);
int EMO_min(int *idx, int *arr, int *filter, int size);
int EMO_max(int *idx, int *arr, int *filter, int size);
void EMO_maxBound(double *max, double *data, int *filter, int row, int col);
void EMO_minBound(double *min, double *data, int *filter, int row, int col);
void EMO_maxminBound(double *max, double *min, double *data, int *filter, int row, int col);
void EMO_findmaxminBound(int *max, int *min, double *data, int *filter, int row, int col);
void EMO_normalize(double *norm, double *data, int *filter, int size, double *min, double *max, int dim);
void EMO_lsq(double *a, double *tmp1, double *tmp2, double *y, int n);
int EMO_find(int *data, int size, int elem);

#endif

