
#ifndef _INDICATOR_
#define _INDICATOR_

#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "indicator.h"
#include "numeric.h"
#include "vector.h"
#include "hv.h"

double mindist(double *v, int *filter, double *f, int n, int dim) {
  double min, d;
  int i;

  min = d = 0;

  if(filter == NULL) {
    for(i = 0; i < n; i++) {
      d = EMO_vdist(v, f+i*dim, dim);

      if(i == 0 || d < min) min = d;
    }
  }
  else {
    for(i = 0; i < n; i++) {
      if(filter[i])
        d = EMO_vdist(v, f+i*dim, dim);

      if(i == 0 || d < min) min = d;
    }
  }

  return min; 
}

/* v: reference vector
 * f: population
 * filter: active population
 * n: population size
 * dim: number of objectives
 */
double mindist_plus(double *v, double *f, int *filter, int n, int dim) {
  double min, d, s;
  int i, j;

  min = d = 0;

  if(filter == NULL) {
    for(i = 0; i < n; i++) {
      s = 0;

      for(j = 0; j < dim; j++) {
        d = max(f[i*dim + j] - v[j], 0);  /* minimization problems: a_i - zref_i */
        s += d * d;
      }

      d = sqrt(s);

      if(i == 0 || d < min) min = d;
    }
  }
  else {
    for(i = 0; i < n; i++) {
      if(filter[i]) {
        s = 0;

        for(j = 0; j < dim; j++) {
          d = max(f[i*dim + j] - v[j], 0);
          s += d * d;
        }

        d = sqrt(s);
      }

      if(i == 0 || d < min) min = d;
    }
  }

  return min; 
}

/* Average distance from the aproximation set to the
   discretization of the Pareto front */
double EMO_gd(double *f, int *filter, int n, double *pf, int m, int dim, double p) {
  double s;
  int i;

  s = 0.0;

  if(filter == NULL) {
    for(i = 0; i < n; i++)
      s += pow(mindist(f+i*dim, NULL, pf, m, dim), p);
  }
  else {
    for(i = 0; i < n; i++)
      if(filter[i])
        s += pow(mindist(f+i*dim, NULL, pf, m, dim), p);
  }

  return pow(s / (double) n, 1.0 / p);
}

void EMO_gdc(double *c, double *f, int *filter, int n, double *pf, int m, int dim, double p) {
  int i;

  if(filter == NULL) {
    for(i = 0; i < n; i++)
      c[i] = mindist(f+i*dim, NULL, pf, m, dim);
  }
  else {
    for(i = 0; i < n; i++)
      if(filter[i])
        c[i] = mindist(f+i*dim, NULL, pf, m, dim);
  }
}

/* Average distance from the discretization of the
   Pareto front to the aproximation set */
double EMO_igd(double *f, int *filter, int n, double *pf, int m, int dim, double p) {
  double s;
  int i;

  s = 0.0;

  for(i = 0; i < m; i++)
    s += pow(mindist(pf+i*dim, filter, f, n, dim), p);

  return pow(s / (double) m, 1.0 / p);
}

/* Average distance from the discretization of the
   Pareto front to the aproximation set */
double EMO_igd2(double *f, int *filter, int n, double *pf, int m, int dim) {
  double s;
  int i;

  s = 0.0;

  for(i = 0; i < m; i++)
    s += mindist(pf+i*dim, filter, f, n, dim);

  return s / (double) m;
}

/* Professor Ishibuchi */
double EMO_igd_plus(double *f, int *filter, int n, double *pf, int m, int dim) {
  double s;
  int i;

  s = 0.0;

  for(i = 0; i < m; i++)
    s += mindist_plus(pf+i*dim, f, filter, n, dim);

  return s / (double) m;
}

void EMO_igdc(double *c, double *f, int n, double *pf, int m, int dim, double p) {
  double d, min = 0;
  int i, j, idx = 0;

  for(j = 0; j < n; j++)
    c[j] = -1;

  for(i = 0; i < m; i++) {

    for(j = 0; j < n; j++) {
      d = EMO_vdist(pf+i*dim, f+j*dim, dim);

      if(j == 0 || d < min) {  /* Hay varios a la misma distancia ? */
        min = d;
        idx = j;
      }
    }

    c[idx] = (c[idx] == -1) ? pow(min, p) : c[idx] + pow(min, p);
  }

  for(j = 0; j < n; j++)
    if(c[j] != -1)
      c[j] = pow(c[j], 1.0 / p);
}

double EMO_deltap(double *f, int *filter, int n, double *pf, int m, int dim, double p) {
  double igd, gd;

  igd = EMO_igd(f, filter, n, pf, m, dim, p);
  gd = EMO_gd(f, filter, n, pf, m, dim, p);

  if(gd > igd) return gd;

  return igd;
}

double EMO_r2(double *data, int *filter, int size, double *W, int wsize, EMO_Utility *utl) {
  double d, vmin = 0, s = 0;
  int i, j;

  if(filter == NULL) {
    for(i = 0; i < wsize; i++) {
      vmin = DBL_MAX;

      for(j = 0; j < size; j++) {
        d = utl->uf(utl, &W[i * utl->nobj], data + j * utl->nobj);

        if(d < vmin) vmin = d; 
      }
      s += vmin;
    }
  }
  else {
    for(i = 0; i < wsize; i++) {
      vmin = DBL_MAX;

      for(j = 0; j < size; j++) {
        if(filter[j]) {
          d = utl->uf(utl, &W[i * utl->nobj], data + j * utl->nobj);

          if(d < vmin) vmin = d; 
        }
      }
      s += vmin;
    }
  }

  return s/wsize;
}

/* Compares two individuals */
int compare(const void **a, const void **b) {
  double *v1, *v2;

  v1 = (double *) *a;
  v2 = (double *) *b;

  if(v1[1] < v2[1]) return -1;
  else if(v1[1] > v2[1]) return 1;

  if(v1[2] < v2[2]) return -1;
  else if(v1[2] > v2[2]) return 1;

  return 0;
}

/* R2 ranking algorithm of the population */
void EMO_r2_ranking(double *rank, double **sort, double *norm, double *tmp, double *data, int size, double *W, int wsize, EMO_Utility *utl) {
  int i, j, k;

  for(j = 0; j < size; j++) {
    rank[j] = DBL_MAX;
    norm[j] = EMO_vnorm(data + j * utl->nobj, 2.0, utl->nobj);
  }

  for(i = 0; i < wsize; i++) {
    // Calculates the individual's contribution to a weight vector
    for(j = 0; j < size; j++) {
      sort[j][0] = j;
      sort[j][1] = utl->uf(utl, W + i * utl->nobj, data + j * utl->nobj);
      sort[j][2] = norm[j];

    }
    // Sorts individuals wrt. the utility value obtained in increasing order
    qsort(sort, size, sizeof(sort[0]), (int (*)(const void *, const void *))&compare);

    // Ranks individuals
    for(j = 1; j <= size; j++) {
      k = (int) sort[j-1][0]; 

      if((double) j < rank[k])
         rank[k] = (double) j;
    }
  }
}

int compare2(const void * a, const void * b)
{
 return ( *(int*)a - *(int*)b );
}

#endif

