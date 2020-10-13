
#include "utility.h"

double EMO_gd(double *f, int *filter, int n, double *pf, int m, int dim, double p);
double EMO_igd(double *f, int *filter, int n, double *pf, int m, int dim, double p);
double EMO_igd2(double *f, int *filter, int n, double *pf, int m, int dim);
double EMO_igd_plus(double *f, int *filter, int n, double *pf, int m, int dim);
double EMO_deltap(double *f, int *filter, int n, double *pf, int m, int dim, double p);

double EMO_r2(double *data, int *filter, int size, double *W, int wsize, EMO_Utility *utl);
void EMO_r2_ranking(double *rank, double **sort, double *norm, double *tmp, double *data, int size, double *W, int wsize, EMO_Utility *utl);

