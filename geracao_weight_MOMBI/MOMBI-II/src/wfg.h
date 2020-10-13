/**************************************************************
 * wfg.h        WFG test suite.                               *
 *                                                            *
 * March 2013                                                 *
 *************************************************************/
/* 
  Reference:

  Simon Huband, Luigi Barone, Lyndon While, and Phil Hingston. 2005. 
  A scalable multi-objective test problem toolkit. 
  In Proceedings of the Third international conference on 
  Evolutionary Multi-Criterion Optimization (EMO'05), 
  Springer-Verlag, Berlin, Heidelberg, 280-295.

  
  Walking Fish Group Toolkit conform to the following format:

  Given z = {z1, ..., zk, zk+1, ..., zn}

  Min   fm=1:M (x) = xM + Sm * hm (x1, ..., xM-1)
 
  where x = {x1, ..., xM} = {max(t_{M}^{p}, A_{1}) * (t_{1}^{p} - 0.5) + 0.5,...,
                             max(t_{M}^{p}, A_{M-1}) * (t_{M-1]^{p} -0.5) +
                             0.5, t_{M}^{p}}

  t^{p} = {t_{1}^{p},...,t_{M}^{p}} <- t^{p-1} <- ... <- t^{1} <-- z_{[0,1]}

  z_{[0,1]} = {z_{1,[0,1]}, ..., z_{n,[0,1]}} = {z_{1}/z_{1,max}, ..., z_{n}/z_{n,max}}

  Where:

  M - number of objectives
  x - set of M underlying parameters (x_{M} is an underlying distance parameter, and
      x_{1:M-1} are underlying position parameters), the domain is in [0,1]
  z is a set of k + l = n >= M working parameters (each zi \in [0, z_{i,max}]) 
  k - position-related parameter
  l - distance-related parameter
  A_{1:M-1} \in {0,1} are degeneracy constants
  h_{1:M} shape functions
  S_{1:M} scaling constants
  t^{1:p} transition vectors

  "<-" indicates that each transition vector is created from another vector via
       transformation functions.
  ---

  S_{m=1:M} = 2 m
  A_{1} = 1, A_{2:M-1} = {0, WFG3; 1 otherwise}
  z_{i=1:n, max} = 2i
*/

#ifndef _WFG_
#define _WFG_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "wfg.h"
#include "numeric.h"
#include "common.h"

double correct_to_01(double a) {
  static double epsilon = 1.0e-10;
  static double min = 0.0;
  static double max = 1.0;
  double min_epsilon = min - epsilon;
  double max_epsilon = max + epsilon;

  if ( a <= min && a >= min_epsilon )
    return min;
  else if ( a >= max && a <= max_epsilon )
    return max;
  else
    return a;
}

/* Shape functions

   Determine the nature of Pareto optimal front, and map parameters 
   with domain [0,1] onto the range [0,1].

   In all cases x_{1},...,x_{M-1} in [0,1], alpha and beta are constants.
*/


/* Linear
   When h_{m=1:M} = linear_{m}, the Pareto optimal front is a linear
   hyperplane, where sum_{m=1}^{M} {h_{m}} = 1.
*/

double sh_linear_1(double *x, int M) {
  double v = 1.0;
  int i;

  for(i = M-2; i > -1; i--)
    v *= x[i];

  return correct_to_01(v);
}

/* m \in 2:M-1 */
double sh_linear_m(double *x, int M, int m) {
  double v = 1.0;
  int i;

  for(i = M-m-1; i > -1; i--)
    v *= x[i];

  return correct_to_01(v * (1.0 - x[M-m]));
}

double sh_linear_M(double *x) {
  return correct_to_01(1.0 - x[0]);
}

void sh_linear(double *h, double *x, int M) {
  int i;

  h[0] = sh_linear_1(x, M);

  for(i = M - 2; i > 0; i--)
    h[i] = sh_linear_m(x, M, i + 1);

  h[M-1] = sh_linear_M(x);
}


/* Convex
   When h_{m=1:M} = convex_{m}, the Pareto optimal front is purely convex.
 */

double sh_convex_1(double *x, int M) {
  double v = 1.0;
  int i;

  for(i = M-2; i > -1; i--)
    v *= 1.0 - cos(x[i] * PI * 0.5);

  return correct_to_01(v);
}

/* m \in 2:M-1 */
double sh_convex_m(double *x, int M, int m) {
  double v = 1.0;
  int i;

  for(i = M-m-1; i > -1; i--)
    v *= 1.0 - cos(x[i] * PI * 0.5); 

  return correct_to_01(v * (1.0 - sin(x[M-m] * PI * 0.5)));
}

double sh_convex_M(double *x) {
  return correct_to_01(1.0 - sin(x[0] * PI * 0.5));
}

void sh_convex(double *h, double *x, int M) {
  int i;

  h[0] = sh_convex_1(x, M);

  for(i = M - 2; i > 0; i--)
    h[i] = sh_convex_m(x, M, i + 1);

  h[M-1] = sh_convex_M(x);
}

/* Concave
   When h_{m=1:M} = concave_{m}, the Pareto optimal front is purely
   concave, and a region of the hyper-sphere of radius one 
   centred at the origin, where sum_{m=1}^{M} h_{m}^{2} = 1.
 */

double sh_concave_1(double *x, int M) {
  double v = 1.0;
  int i;

  for(i = M-2; i > -1; i--)
    v *= sin(x[i] * PI * 0.5);

  return correct_to_01(v);
}

/* m \in 2:M-1 */
double sh_concave_m(double *x, int M, int m) {
  double v = 1.0;
  int i;

  for(i = M-m-1; i > -1; i--)
    v *= sin(x[i] * PI * 0.5);

  return correct_to_01(v * cos(x[M-m] * PI * 0.5));
}

double sh_concave_M(double *x) {
  return correct_to_01(cos(x[0] * PI * 0.5));
}

void sh_concave(double *h, double *x, int M) {
  int i;

  h[0] = sh_concave_1(x, M);

  for(i = M - 2; i > 0; i--) 
    h[i] = sh_concave_m(x, M, i + 1);
 
  h[M-1] = sh_concave_M(x);
}
 
/* Mixed convex / concave
   Causes the Pareto optimal front to contain both convex and 
   concave segments, the number of which is controlled by A.
   The overrall shape is controlled by alpha: when alpha > 1
   or when alpha < 1, the overall shape is convex or concave
   respectibely. When alpha = 1, the overall shape is linear.

   (alpha > 0, A \in {1,2,...})
 */
double sh_mixed_M(double *x, double A, double alpha) {
  double v;

  v = cos(2.0 * A * PI * x[0] + PI * 0.5);
  v /= 2.0 * A * PI;
  v = 1.0 - x[0] - v;
  return correct_to_01(pow(v, alpha));
}

/* Disconnected
   Causes the Pareto optimal front to have disconnected regions,
   the number of which is controlled by A. The overall shape
   is controlled by alpha (when alpha > 1 or when alpha < 1,
   the overall shape is concave or convex respectively, and
   when alpha = 1, the overall shape is linear), and beta
   influences the location of the disconnected regions
   (larger values push the location of disconnected regions
   towards larger values of x1, and vice versa).

   (alpha, beta > 0, A in {1,2,...})
 */
double sh_disc_M(double *x, double A, double alpha, double beta) {
  double v;

  v = cos(A * pow(x[0], beta) * PI);
  v = pow(x[0], alpha) * pow(v, 2.0);
  return correct_to_01(1.0 - v);
}


/* Transformations Functions
   Map input parameters with domain [0,1] onto the range [0,1].
   All transformation functions take a vector of parameters
   (called the primary parameters y) and map them to a single 
   value.
   Primary parameters allow us to qualify working parameters
   as being position and distance related.

   tr: 1:n -> Re 
*/

/* Bias
   Have a natural impact on the search process by biasing
   the fitness landscape.
 */

/* Bias: Polynomial (alpha > 0, alpha != 1)
   When alpha > 1 or when alpha < 1, y is biased towards zero or
   towards one respectively.
*/
double tr_b_poly(double y, double alpha) {
  return correct_to_01(pow(y, alpha));
}

/* Bias: Flat Region (A,B,C in [0,1], B < C, 
   B = 0 => A = 0 and C != 1, 
   C = 1 => A = 1 and B != 0)

   Values of y between B and C (the area of the flat region)
   are all mapped to the value A.
*/
double tr_b_flat(double y, double A, double B, double C) {
  double v, w;

  v =  min(0.0, floor(y - B));
  v *= A * (B - y) / B;
 
  w = min(0.0, floor(C - y));
  w *= (1.0 - A) * (y - C) / (1.0 - C);

  return correct_to_01(A + v - w);
}

/* Bias: Parameter Dependent
   (A in (0,1), 0 < B < C)

   A, B, C, and the secondary parameter vector y' together
   determine the degree to which y is biased by being raised
   to an associated power: values of u(y') in [0,0.5] are mapped
   linearly onto [B, B+(C-B)A], and values of u(y') in [0.5,1]
   are mapped linearly onto [B + (C-B)A,C].
 */
double tr_b_param(double y, double u, double A, double B, double C) {
  double v;

  v = B + (C - B) * (A - (1.0 - 2.0 * u) * fabs(floor(0.5 - u) + A));
  return correct_to_01(pow(y, v));
}

/* Shift
   Move the location of optima. In the absence of any shift,
   all distance-related parameters would be extremal parameters,
   with optimal value at zero. Shift transformations can be
   used to set the location of parameter optima (subject to
   skewing by bias transformations), which is useful if
   medial and extremal parameters are to be avoided.
   Authors recommend that all distance-related parameters be subject
   to at least one shift transformation.
 */

/* Shift: Linear ( A in (0,1) )
   A is the value for which y is mapped to zero.
 */
double tr_s_linear(double y, double A) {
  double v;
  
  v = A - y;
  return correct_to_01(fabs(v) / fabs(floor(v) + A));
}

/* Shift: Deceptive
   A in (0,1), 
   0 < B << 1, 
   0 < C << 1, 
   A - B > 0, 
   A + B < 1

   A is the value at which y is mapped to zero, and the global
   minimum of the transformation. B is the aperture size of the
   well/basin leading to the global minimum at A, and C is the 
   value of the deceptive minima (there are always two deceptive
   minima).
 */
double tr_s_decept(double y, double A, double B, double C) {
  double v1, v2, v3, v4;
  
  v1 = A - B;
  v2 = A + B;
  v3 = 1.0 - v2;
  
  v4 = floor(y - v1) * (1.0 - C + v1/B) / v1;
  v4 += floor(v2 - y) * (1.0 - C + v3 / B) / v3 + 1.0 / B;
  v4 *= fabs(y - A) - B;

  return correct_to_01(1.0 + v4);
}

/* Shift: Multi-modal
   A in {1,2,...}
   B >= 0,
   (4A + 2) * PI >= 4B
   C in (0,1)

   A controls the number of minima, B controls the magnitud
   of the "hill sizes" of the multi-modality, and C is the value
   for which y is mapped to zero. When B = 0, 2A + 1 values
   of y (one at C) are mapped to zero, and when B != 0, there
   are 2A local minima, and one global minimum at C.
   Larger values of A and smaller values of B create more
   difficult problems.
 */
double tr_s_multi(double y, double A, double B, double C) {
  double v1, v2, v3, v4, v5;

  v1 = C - y;
  v2 = fabs(v1);
  v3 = floor(v1);
  v4 = 2.0 * (v3 + C);
  v5 = 1.0 + cos((4.0*A + 2.0) * PI * (0.5 - v2 / v4));
  v5 += 4.0 * B * pow(v2 / v4, 2.0);
  return correct_to_01(v5 / (B + 2.0));
}

/* Reduction: Weighted Sum
   |w| = |y|, w1,...,w_{|y|} > 0

   By varying the constants of the weight vector w, EAs can be
   forced to treat parameters differently.
 */
double tr_r_sum(double *y, double *w, int n) {
  double v1, v2;
  int i;
 
  v1 = v2 = 0.0;

  for(i = 0; i < n; i++) {
    v1 += w[i] * y[i];
    v2 += w[i];
  }

  return correct_to_01(v1 / v2);
}

/* Reduction: Non-separable
   A in {1,...,|y|}, |y| mod A = 0

   A controls the degree of non-separability (noting that
   r_nonsep(y,1) = r_sum(y,{1,...,1})).
 */
double tr_r_nonsep(double *y, double A, int n) {
  double v1, v2, v3;
  int j, k, f;

  v2 = 0.0;
  v3 = ceil(A * 0.5);
  f = A - 2;

  for(j = 0; j < n; j++) {
    v1 = 0;

    for(k = 0; k <= f; k++) 
      v1 += fabs(y[j] - y[(j+k+1) % n]);
 
    v2 += y[j] + v1;
  }

  return correct_to_01(v2 / (n / A * v3 * (1.0 + 2.0 * A - 2.0 * v3)));
}


/* WFG Transformations */

void wfg1_t1(EMO_MOP *mop, double *z) {
  int i;

  if(mop->t != z)
    memcpy(mop->t, z, sizeof(double) * mop->npos);

  for(i = mop->npos; i < mop->nvar; i++) 
    mop->t[i] = tr_s_linear(z[i], 0.35);
}

void wfg1_t2(EMO_MOP *mop, double *z) {
  int i;

  if(mop->t != z)
    memcpy(mop->t, z, sizeof(double) * mop->npos);

  for(i = mop->npos; i < mop->nvar; i++) 
    mop->t[i] = tr_b_flat(z[i], 0.8, 0.75, 0.85); 
}

void wfg1_t3(EMO_MOP *mop, double *z) {
  int i;

  for(i = 0; i < mop->nvar; i++) 
    mop->t[i] = tr_b_poly(z[i], 0.02);
}

void wfg1_t4(EMO_MOP *mop, double *z) {
  int i, j, idx, head, tail, f;

  f = mop->nobj - 1;

  for(i = 1; i <= f; i++) {
    idx = 0;

    head = (i-1) * mop->npos / (mop->nobj - 1);
    tail = i * mop->npos / (mop->nobj - 1);

    for(j = head; j < tail;  j++) {
      mop->w[idx] = 2.0 * (j + 1.0);
      mop->y[idx] = z[j];
      idx++;
    }
    mop->t[i-1] = tr_r_sum(mop->y, mop->w, idx); 
  }

  idx = 0;

  for(j = mop->npos; j < mop->nvar; j++) {
    mop->w[idx] = 2.0 * (j + 1.0);
    mop->y[idx] = z[j];
    idx++;
  }

  mop->t[mop->nobj-1] = tr_r_sum(mop->y, mop->w, idx); 
}

void wfg2_t2(EMO_MOP *mop, double *z) {
  int i, j, idx, head, tail, l, f;

  l = mop->nvar - mop->npos;

  if(mop->t != z)
    memcpy(mop->t, z, sizeof(double) * mop->npos);

  f = mop->npos + l/2;

  for(i = mop->npos + 1; i <= f; i++) {
    head = mop->npos + 2 * (i - mop->npos) - 2;
    tail = mop->npos + 2 * (i - mop->npos);
    idx = 0;

    for(j = head; j < tail;  j++) {
      mop->y[idx] = z[j];
      idx++;
    }
    mop->t[i-1] = tr_r_nonsep(mop->y, 2.0, idx);
  }
}

void wfg2_t3(EMO_MOP *mop, double *z) {
  int i, j, idx, head, tail, l, f;

  l = mop->nvar - mop->npos;

  for(i = 1; i <= mop->nobj - 1; i++) {
    idx = 0;

    head = (i-1) * mop->npos / (mop->nobj - 1);
    tail = i * mop->npos / (mop->nobj - 1);

    for(j = head; j < tail;  j++) {
      mop->w[idx] = 1.0; 
      mop->y[idx] = z[j];
      idx++;
    }
    mop->t[i-1] = tr_r_sum(mop->y, mop->w, idx); 
  }

  idx = 0;
  f = mop->npos + l/2;

  for(j = mop->npos; j < f; j++) {
    mop->w[idx] = 1.0;
    mop->y[idx] = z[j];
    idx++;
  } 

  mop->t[mop->nobj-1] = tr_r_sum(mop->y, mop->w, idx); 
}

void wfg4_t1(EMO_MOP *mop, double *z) {
  int i;

  for(i = 0; i < mop->nvar; i++) 
    mop->t[i] = tr_s_multi(z[i], 30.0, 10.0, 0.35); 
}

void wfg4_t2(EMO_MOP *mop, double *z) {
  int i, j, idx, head, tail;

  for(i = 1; i <= mop->nobj - 1; i++) {
    idx = 0;

    head = (i-1) * mop->npos / (mop->nobj - 1);
    tail = i * mop->npos / (mop->nobj - 1);

    for(j = head; j < tail;  j++) {
      mop->w[idx] = 1.0; 
      mop->y[idx] = z[j];
      idx++;
    }
    mop->t[i-1] = tr_r_sum(mop->y, mop->w, idx); 
  }

  idx = 0;

  for(j = mop->npos; j < mop->nvar; j++) { /* modified from wfg2_t3 */
    mop->w[idx] = 1.0;
    mop->y[idx] = z[j];
    idx++;
  } 

  mop->t[mop->nobj-1] = tr_r_sum(mop->y, mop->w, idx); 
}

void wfg5_t1(EMO_MOP *mop, double *z) {
  int i;

  for(i = 0; i < mop->nvar; i++) 
    mop->t[i] = tr_s_decept(z[i], 0.35, 0.001, 0.05);
} 

void wfg6_t2(EMO_MOP *mop, double *z) {
  int i, j, idx, head, tail, f;

  f = mop->nobj - 1;

  for(i = 1; i <= f; i++) {
    head = (i-1) * mop->npos / (mop->nobj - 1);
    tail = i * mop->npos / (mop->nobj - 1);
    idx = 0;

    for(j = head; j < tail;  j++) {
      mop->y[idx] = z[j];
      idx++;
    }
    mop->t[i-1] = tr_r_nonsep(mop->y, (double) mop->npos / (mop->nobj - 1.0), idx); 
  }

  idx = 0;

  for(j = mop->npos; j < mop->nvar;  j++) {
    mop->y[idx] = z[j];
    idx++;
  }
  mop->t[mop->nobj-1] = tr_r_nonsep(mop->y, mop->nvar-mop->npos, idx); 
} 

void wfg7_t1(EMO_MOP *mop, double *z) {
  int i, j, idx;

  for(i = 0; i < mop->npos; i++) { 
    idx = 0;

    for(j = i + 1; j < mop->nvar;  j++) {
      mop->w[idx] = 1.0; 
      mop->y[idx] = z[j];
      idx++;
    }
    mop->t[i] = tr_b_param(z[i], tr_r_sum(mop->y, mop->w, idx), 0.98/49.98, 0.02, 50.0); 
  }

  for(i = mop->npos; i < mop->nvar; i++) 
    mop->t[i] = z[i];
}
 
void wfg8_t1(EMO_MOP *mop, double *z) {
  int i, j, idx;

  if(mop->t != z)
    memcpy(mop->t, z, sizeof(double) * mop->npos);

  for(i = mop->npos; i < mop->nvar; i++) { 
    idx = 0;

    for(j = 0; j <= i - 1;  j++) {
      mop->w[idx] = 1.0; 
      mop->y[idx] = z[j];
      idx++;
    }
    mop->t[i] = tr_b_param(z[i], tr_r_sum(mop->y, mop->w, idx), 0.98/49.98, 0.02, 50.0); 
  }
}

void wfg9_t1(EMO_MOP *mop, double *z) {
  int i, j, idx;

  for(i = 0; i < mop->nvar - 1; i++) { 
    idx = 0;

    for(j = i+1; j < mop->nvar;  j++) {
      mop->w[idx] = 1.0; 
      mop->y[idx] = z[j];
      idx++;
    }
    mop->t[i] = tr_b_param(z[i], tr_r_sum(mop->y, mop->w, idx), 0.98/49.98, 0.02, 50.0); 
  }

  mop->t[mop->nvar-1] = z[mop->nvar-1];
}

void wfg9_t2(EMO_MOP *mop, double *z) {
  int i;

  for(i = 0; i < mop->npos; i++) 
    mop->t[i] = tr_s_decept(z[i], 0.35, 0.001, 0.05);
 
  for(i = mop->npos; i < mop->nvar; i++) 
    mop->t[i] = tr_s_multi(z[i], 30.0, 95.0, 0.35); 
}

/* x: set of M underlying parameters */
void calc_x(double *x, double *t, double *A, int M) {
  int i;

  for(i = 0; i < M - 1; i++) {
    x[i] = (t[i] - 0.5);
    x[i] *= max(t[M-1], A[i]);
    x[i] += 0.5;
  }

  x[M-1] = t[M-1];
}
 
/* Objective functions */ 
void calc_f(double *f, double *x, double *S, double *h, int M) {
  int i;

  for(i = 0; i < M; i++)
    f[i] = x[M-1] + S[i] * h[i];
}

/*void print_vec(char *s, double *v, int n) {
  int i;

  printf("%s", s);

  for(i = 0; i < n; i++)
    printf("%f ", v[i]);
  printf("\n");
}*/

/* Normalization is omitted, in PISA it is multiplied and divided 
   each parameter vector by 2 * i

void normalize(double **x, int N) {
  int i;

  for(i = 0; i < N; i++) {
    x[i] = x[i] / (2 * (i + 1));
  }
}*/


void wfg1(EMO_MOP *mop, double *f, double *z) {
  int i;

  wfg1_t1(mop, z);      /* Transformation t1 */
  wfg1_t2(mop, mop->t); /* Transformation t2 */
  wfg1_t3(mop, mop->t); /* Transformation t3 */
  wfg1_t4(mop, mop->t); /* Transformation t4 */

  calc_x(mop->x, mop->t, mop->A, mop->nobj); /* Vector x */ 
 
  /* Shape */ 
  mop->h[0] = sh_convex_1(mop->x, mop->nobj);

  for(i = mop->nobj - 2; i > 0; i--) 
    mop->h[i] = sh_convex_m(mop->x, mop->nobj, i + 1);
 
  mop->h[mop->nobj-1] = sh_mixed_M(mop->x, 5.0, 1.0);

  calc_f(f, mop->x, mop->S, mop->h, mop->nobj); /* Objective functions */ 
  mop->feval++;
}

void wfg2(EMO_MOP *mop, double *f, double *z) {
  int i;

  wfg1_t1(mop, z);      /* Transformation t1 */
  wfg2_t2(mop, mop->t); /* Transformation t2 */
  wfg2_t3(mop, mop->t); /* Transformation t3 */

  calc_x(mop->x, mop->t, mop->A, mop->nobj); /* Vector x */ 

  /* Shape */ 
  mop->h[0] = sh_convex_1(mop->x, mop->nobj);

  for(i = mop->nobj - 2; i > 0; i--) 
    mop->h[i] = sh_convex_m(mop->x, mop->nobj, i + 1);
 
  mop->h[mop->nobj-1] = sh_disc_M(mop->x, 5.0, 1.0, 1.0);
 
  calc_f(f, mop->x, mop->S, mop->h, mop->nobj);      /* Objective functions */ 
  mop->feval++;
}

void wfg3(EMO_MOP *mop, double *f, double *z) {
  wfg1_t1(mop, z);                              /* Transformation t1 */
  wfg2_t2(mop, mop->t);                         /* Transformation t2 */
  wfg2_t3(mop, mop->t);                         /* Transformation t3 */
  calc_x(mop->x, mop->t, mop->A, mop->nobj);    /* Vector x */ 
  sh_linear(mop->h, mop->x, mop->nobj);         /* Shape */ 
  calc_f(f, mop->x, mop->S, mop->h, mop->nobj); /* Objective functions */ 
  mop->feval++;
}

void wfg4(EMO_MOP *mop, double *f, double *z) {
  wfg4_t1(mop, z);                              /* Transformation t1 */
  wfg4_t2(mop, mop->t);                         /* Transformation t2 */
  calc_x(mop->x, mop->t, mop->A, mop->nobj);    /* Vector x */ 
  sh_concave(mop->h, mop->x, mop->nobj);        /* Shape */ 
  calc_f(f, mop->x, mop->S, mop->h, mop->nobj); /* Objective functions */ 
  mop->feval++;
}

void wfg5(EMO_MOP *mop, double *f, double *z) {
  wfg5_t1(mop, z);                              /* Transformation t1 */
  wfg4_t2(mop, mop->t);                         /* Transformation t2 */
  calc_x(mop->x, mop->t, mop->A, mop->nobj);    /* Vector x */ 
  sh_concave(mop->h, mop->x, mop->nobj);        /* Shape */ 
  calc_f(f, mop->x, mop->S, mop->h, mop->nobj); /* Objective functions */ 
  mop->feval++;
}

void wfg6(EMO_MOP *mop, double *f, double *z) {
  wfg1_t1(mop, z);                               /* Transformation t1 */
  wfg6_t2(mop, mop->t);                          /* Transformation t2 */
  calc_x(mop->x, mop->t, mop->A, mop->nobj);     /* Vector x */ 
  sh_concave(mop->h, mop->x, mop->nobj);         /* Shape */ 
  calc_f(f, mop->x, mop->S, mop->h, mop->nobj);  /* Objective functions */ 
  mop->feval++;
}

void wfg7(EMO_MOP *mop, double *f, double *z) {
  wfg7_t1(mop, z);                              /* Transformation t1 */
  wfg1_t1(mop, mop->t);                         /* Transformation t2 */
  wfg4_t2(mop, mop->t);                         /* Transformation t3 */
  calc_x(mop->x, mop->t, mop->A, mop->nobj);    /* Vector x */ 
  sh_concave(mop->h, mop->x, mop->nobj);        /* Shape */ 
  calc_f(f, mop->x, mop->S, mop->h, mop->nobj); /* Objective functions */ 
  mop->feval++;
}

void wfg8(EMO_MOP *mop, double *f, double *z) {
  wfg8_t1(mop, z);                              /* Transformation t1 */
  wfg1_t1(mop, mop->t);                         /* Transformation t2 */
  wfg4_t2(mop, mop->t);                         /* Transformation t3 */
  calc_x(mop->x, mop->t, mop->A, mop->nobj);    /* Vector x */ 
  sh_concave(mop->h, mop->x, mop->nobj);        /* Shape */ 
  calc_f(f, mop->x, mop->S, mop->h, mop->nobj); /* Objective functions */ 
  mop->feval++;
}

void wfg9(EMO_MOP *mop, double *f, double *z) {
  wfg9_t1(mop, z);                              /* Transformation t1 */
  wfg9_t2(mop, mop->t);                         /* Transformation t2 */
  wfg6_t2(mop, mop->t);                         /* Transformation t3 */
  calc_x(mop->x, mop->t, mop->A, mop->nobj);    /* Vector x */ 
  sh_concave(mop->h, mop->x, mop->nobj);        /* Shape */ 
  calc_f(f, mop->x, mop->S, mop->h, mop->nobj); /* Objective functions */ 
  mop->feval++;
}

void wfgRange(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = 0.0;
    mop->xmax[i] = 1.0;
  }
}

int mallocWFG(EMO_MOP *mop) {
  double v;
  int i;

  if(mop->nobj < 2) {
    printf("Error, nobj must be greather than 2 in wfg.h\n");
    return 1;
  }

  if(mop->npos < 1) {
    printf("Error, npos must be greather than 1 in wfg.h\n");
    return 1;
  }

  if(mop->npos > mop->nvar) {
    printf("Error, npos must be less than nvar in wfg.h\n");
    return 1;
  }

  if(mop->npos % (mop->nobj-1) != 0) {
    printf("Error, npos must be in {nobj-1, 2(nobj-1), 3(nobj-1),...} in wfg.h\n");
    return 1;
  }

  mop->t = (double *) malloc(sizeof(double) * mop->nvar);     /* transition vector     */
  mop->x = (double *) malloc(sizeof(double) * mop->nobj);     /* underlying parameters */
  mop->h = (double *) malloc(sizeof(double) * mop->nobj);     /* shape                 */
  mop->S = (double *) malloc(sizeof(double) * mop->nobj);     /* constant              */
  mop->A = (double *) malloc(sizeof(double) * (mop->nobj-1)); /* constant              */
  mop->y = (double *) malloc(sizeof(double) * mop->nvar);     /* temporary vector      */
  mop->w = (double *) malloc(sizeof(double) * mop->nvar);     /* weight vector         */

  if(mop->t == NULL || mop->x == NULL || mop->h == NULL || mop->S == NULL || mop->A == NULL || mop->y == NULL || mop->w == NULL) {
    printf("Not enough memory in wfg.c\n");
    return 1;
  }

  for(i = 0; i < mop->nobj; i++)
    mop->S[i] = 2.0 * (i+1);

  mop->A[0] = 1.0;

  if(strcmp(mop->name, "WFG3") == 0) { /* degenerate */
    v = 0.0;
    printf("degenerate wfg3, v %f\n", v);
  }
  else {
    v = 1.0;
  }
 
  //v = (wfg == 3)? 0.0 : 1.0; /* degenerate */

  for(i = mop->nobj - 2; i > 0; i--)
    mop->A[i] = v; 

  return 0;
}

void freeWFG(EMO_MOP *mop) {
  free(mop->t);
  free(mop->x);
  free(mop->h);
  free(mop->S);
  free(mop->A);
  free(mop->y);
  free(mop->w);
}

#endif

