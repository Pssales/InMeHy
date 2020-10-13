
#include <string.h>
#include <math.h>
#include <float.h>
#include "vector.h"

// n represent the dimension of each vector

/* p-Norm of a vector x
   p = 0, infinity norm
 */
double EMO_vnorm(double *x, double p, int n) {
  double r, v = 0;
  int i;

  if(p == 0) {
    for(i = 0; i < n; i++) {
      r = fabs(x[i]);
      if(r > v)
        v = r;
    }
    return v;
  }
  else {
    for(i = 0; i < n; i++)
      v += pow(fabs(x[i]), p);
  
    return pow(v, 1.0 / p);
  }
}

/* Multiplication of a vector by a scalar y = alpha * x */
void EMO_vmul(double *y, double *x, double alpha, int n) {
  int i;

  for(i = 0; i < n; i++)
    y[i] = alpha * x[i];
}

/* Sum of a vector by a scalar y =  x + (alpha,alpha,alpha) */
void EMO_vsum(double *y, double *x, double alpha, int n) {
  int i;

  for(i = 0; i < n; i++)
    y[i] = alpha + x[i];
}

/* Dot product */
double EMO_vdot(double *x, double *y, int n) {
  double v = 0;
  int i;

  for(i = 0; i < n; i++)
    v += x[i] * y[i];

  return v;
}

/* Projection of a vector x onto a vector y */
void EMO_vproj(double *r, double *x, double *y, int n) {
  EMO_vmul(r, y, EMO_vdot(x,y,n) / EMO_vdot(y,y,n), n);
}

void EMO_vadd(double *r, double *x, double *y, int n) {
  int i;

  for(i = 0; i < n; i++)
    r[i] = x[i] + y[i];
}

void EMO_vdiff(double *r, double *x, double *y, int n) {
  int i;

  for(i = 0; i < n; i++)
    r[i] = x[i] - y[i];
}

/* Orthogonal vector between a point and a line (vector) */
void EMO_vorth(double *r, double *p, double *v, int n) {
  EMO_vproj(r, p, v, n);
  EMO_vdiff(r, p, r, n);
}

/*Distance between thwo points */
double EMO_vdist(double *x, double *y, int n) {
  double v = 0;
  int i;

  for(i = 0; i < n; i++)
    v += (x[i] - y[i]) * (x[i] - y[i]);
  
  return sqrt(v);
}

void EMO_vzero(double *x, int n) {
  memset(x, 0, sizeof(double) * n);
}

int EMO_isvzero(double *x, int n) {
  int i;

  for(i = 0; i < n; i++)
    if(x[i] != 0)
     return 0;

  return 1; 
}

/* 1 0 0 ...
   0 1 0 ...
   0 0 1 ...
   ...
*/
void EMO_vaxes(double *x, int n) {
  int i;

  memset(x, 0, sizeof(double) * n * n);

  for(i = 0; i < n; i++)
    x[i*n + i] = 1.0;
}

void EMO_vcopy(double *dest, double *src, int n) {
  int i;

  for(i = 0; i < n; i++)
    dest[i] = src[i];
}

/* stdout para salida en pantalla */
void EMO_vprint(FILE *fp, double *x, int n, const char *s) {
  int i;

  if(s != NULL)
    fprintf(fp, "%s: ( ", s);

  for(i = 0; i < n; i++)
    fprintf(fp, "%f ", x[i]);
 
  if(s != NULL) 
    fprintf(fp, ")");

  fprintf(fp, "\n");
}

/* stdout para salida en pantalla */
void EMO_vprinti(FILE *fp, int *x, int n, const char *s) {
  int i;

  if(s != NULL)
    fprintf(fp, "%s: ( ", s);

  for(i = 0; i < n; i++)
    fprintf(fp, "%d ", x[i]);
 
  if(s != NULL) 
    fprintf(fp, ")");

  fprintf(fp, "\n");
}

