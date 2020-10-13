
#ifndef _VECTORH_
#define _VECTORH_

#include <stdio.h>

double EMO_vnorm(double *x, double p, int n);
void EMO_vmul(double *y, double *x, double alpha, int n);
void EMO_vsum(double *y, double *x, double alpha, int n);
double EMO_vdot(double *x, double *y, int n);
void EMO_vproj(double *r, double *x, double *y, int n);
void EMO_vadd(double *r, double *x, double *y, int n);
void EMO_vdiff(double *r, double *x, double *y, int n);
void EMO_vorth(double *r, double *p, double *v, int n);
double EMO_vdist(double *x, double *y, int n);
void EMO_vzero(double *x, int n);
int EMO_isvzero(double *x, int n);
void EMO_vaxes(double *x, int n);
void EMO_vcopy(double *dest, double *src, int n);
void EMO_vprint(FILE *fp, double *x, int n, const char *s);
void EMO_vprinti(FILE *fp, int *x, int n, const char *s);

#endif

