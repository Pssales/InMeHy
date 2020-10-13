/* 
  wfg.c: WFG Test Suite
  Departamento de Computacion - CINVESTAV

  Profesor: Carlos A. Coello Coello
  Autor:    Raquel Hernandez Gomez
  Enero 2013


  Given z = {z1, ..., zk, zk+1, ..., zn}

  Min   fm=1:M (x) = xM + Sm * hm (x1, ..., xM-1)
 
  where x = {x1, ..., xM} = {max(tM^p, A1)*(t1^p - 0.5) + 0.5,...,
                             max(tM^p, AM-1) * (tM-1^p -0.5) +
                             0.5, tM^p}

  t^p = {t1^p,...,tM^p} <- t^p-1 <- ... <- t^1 <-- z[0,1]

  z[0,1] = {z1,[0,1], ..., zn,[0,1]} = {z1/z1,max, ..., zn/zn,max}

  M - number of objectives
  x - conjunto de M parametros subyacentes (ocultos)
  k + l = n >= M
  k - position-related parameter
  l - distance-related parameter
  A1:M-1 in {0,1} - degeneracy constants
  h1:M shape functions
  S1:M scaling constants
  t^1:p transition vectors

  z in [0, zi,max]
  x in [0, 1]
  ---

  Sm=1:M = 2 m
  A1 = 1
  A2:M-1 = {0, WFG3; 1 otherwise}
  zi=1:n, max = 2i
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "wfg.h"

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
 Determine the nature of Pareto optimal front, and map
 parameters with domain [0,1] onto the range [0,1].

 x1,...,xM-1 in [0,1]
 alpha and beta are constants
*/

/* Linear
   When hm=1:M = linearm, the Pareto optimal front is a linear
   hyperplane, where sum_{m=1}^{M} {hm} = 1.
*/

double sh_linear_1(double *x, int M) {
  double v = 1;
  int i;

  for(i = M-2; i > -1; i--)
    v *= x[i];

  return correct_to_01(v);
}

/* m in 2:M-1 */
double sh_linear_m(double *x, int M, int m) {
  double v = 1;
  int i;

  for(i = M-m-1; i > -1; i--)
    v *= x[i];

  return correct_to_01(v*(1 - x[M-m]));
}

double sh_linear_M(double *x) {
  return correct_to_01(1 - x[0]);
}

void sh_linear(double **h, double *x, int M) {
  int i;

  (*h)[0] = sh_linear_1(x, M);

  for(i = M - 2; i > 0; i--)
    (*h)[i] = sh_linear_m(x, M, i + 1);

  (*h)[M-1] = sh_linear_M(x);
}

/* Convex
   When hm=1:M = convexm, the Pareto optimal front is
   purely convex.
 */

double sh_convex_1(double *x, int M) {
  double v = 1;
  int i;

  for(i = M-2; i > -1; i--)
    v *= 1 - cos(x[i]*PI/2);

  return correct_to_01(v);
}

/* m in 2:M-1 */
double sh_convex_m(double *x, int M, int m) {
  double v = 1;
  int i;

  for(i = M-m-1; i > -1; i--)
    v *= 1 - cos(x[i]*PI/2); 

  return correct_to_01(v * (1 - sin(x[M-m]*PI/2)));
}

double sh_convex_M(double *x) {
  return correct_to_01(1 - sin(x[0]*PI/2));
}

void sh_convex(double **h, double *x, int M) {
  int i;

  (*h)[0] = sh_convex_1(x, M);

  for(i = M - 2; i > 0; i--)
    (*h)[i] = sh_convex_m(x, M, i + 1);

  (*h)[M-1] = sh_convex_M(x);
}

/* Concave
   When hm=1:M = concavem, the Pareto optimal front is purely
   concave, and a region of the hyper-sphere of radius one 
   centred at the origin, where sum_{m=1}^{M} h_m^2 = 1.
 */

double sh_concave_1(double *x, int M) {
  double v = 1;
  int i;

  for(i = M-2; i > -1; i--)
    v *= sin(x[i]*PI/2);

  return correct_to_01(v);
}

/* m in 2:M-1 */
double sh_concave_m(double *x, int M, int m) {
  double v = 1;
  int i;

  for(i = M-m-1; i > -1; i--)
    v *= sin(x[i]*PI/2);

  return correct_to_01(v * cos(x[M-m] * PI/2));
}

double sh_concave_M(double *x) {
  return correct_to_01(cos(x[0] * PI/2));
}

void sh_concave(double **h, double *x, int M) {
  int i;

  (*h)[0] = sh_concave_1(x, M);

  for(i = M - 2; i > 0; i--) 
    (*h)[i] = sh_concave_m(x, M, i + 1);
 
  (*h)[M-1] = sh_concave_M(x);
}
 
/* Mixed convex / concave
   Causes the Pareto optimal front to contain both convex and 
   concave segments, the number of which is controlled by A.
   The overrall shape is controlled by alpha: when alpha > 1
   or when alpha < 1, the overall shape is convex or concave
   respectibely. When alpha = 1, the overall shape is linear.

   (alpha > 0, A in {1,2,...})
 */
double sh_mixed_M(double *x, double A, double alpha) {
  double v;

  v = cos(2*A*PI*x[0] + PI / 2);
  v /= 2*A*PI;
  v = 1 - x[0] - v;
  return correct_to_01(pow(v, alpha));
}

/* Disconnected
   Causes the Pareto optimal front to have disconnected regions,
   the number of which is controlled by A. The overall shape
   is controlled by alpha (when alpha > 1 or when alpha < 1,
   the overall shape is concave or convex respectibely, and
   when alpha = 1, the overall shape is linear), and beta
   influences the location of the disconnected regions
   (larger values push the location of disconnected regions
   towards larger values of x1, and vice versa).

   (alpha, beta > 0, A in {1,2,...})
 */
double sh_disc_M(double *x, double A, double alpha, double beta) {
  double v;

  v = cos(A * pow(x[0], beta) * PI);
  v = pow(x[0], alpha) * pow(v, 2);
  return correct_to_01(1 - v);
}

/* Transformations
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

  v =  min(0, floor(y - B));
  v *= A * (B - y) / B;
 
  w = min(0, floor(C - y));
  w *= (1 - A) * (y - C) / (1 - C);

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

  v = B + (C - B) * (A - (1 - 2 * u) * fabs(floor(0.5 - u) + A));
  return correct_to_01(pow(y, v));
}

/* Shift
   Move the location of optima. In the absence of any shift,
   all distance-related parameters would be extremal parameters,
   with optimal value at zero. Shift transformations can be
   used to set the location of parameter optima (subject to
   skewing by bias transformations), which is useful if
   medial and extremal parameters are to be avoided.
   We recommend that all distance-related parameters be subject
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
  v3 = 1 - v2;
  
  v4 = floor(y - v1) * (1 - C + v1/B) / v1;
  v4 += floor(v2 - y) * (1 - C + v3 / B) / v3 + 1 / B;
  v4 *= fabs(y - A) - B;

  return correct_to_01(1 + v4);
}

/* Shift: Multi-modal
   A in {1,2,...}
   B >= 0,
   (4A + 2) * PI >= 4B
   C in (0,1)

   A controls the number of minima, B controls the magnitud
   of the hill sizes of the multi-modality, and C is the value
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
  v4 = 2 * (v3 + C);
  v5 = 1 + cos((4*A + 2) * PI * (0.5 - v2 / v4));
  v5 += 4 * B * pow(v2 / v4, 2);
  return correct_to_01(v5 / (B + 2));
}

/* Reduction: Weighted Sum
   |w| = |y|
   w1,...,w|y| > 0

   By varying the constants of the weight vector w, EAs can be
   forced to treat parameters differently.
 */
double tr_r_sum(double *y, double *w, int n) {
  double v1, v2;
  int i;
 
  v1 = v2 = 0;

  for(i = 0; i < n; i++) {
    v1 += w[i] * y[i];
    v2 += w[i];
  }

  return correct_to_01(v1 / v2);
}

/* Reduction: Non-separable
   A in {1,...,|y|}
   |y| mod A = 0

   A controls the degree of non-separability (noting that
   r_nonsep(y,1) = r_sum(y,{1,...,1})).
 */
double tr_r_nonsep(double *y, double A, int n) {
  double v1, v2, v3;
  int j, k;

  v2 = 0;
  v3 = ceil(A / 2);

  for(j = 0; j < n; j++) {
    v1 = 0;

    for(k = 0; k <= A - 2; k++) 
      v1 += fabs(y[j] - y[(j+k+1) % n]);
 
    v2 += y[j] + v1;
  }

  return correct_to_01(v2 / (n / A * v3 * (1 + 2 * A - 2 * v3)));
}

void wfg1_t1(double **t, double *z, int N, int K) {
  int i;

  if(*t != z)
    memcpy(*t, z, sizeof(double) * K);

  for(i = K; i < N; i++) 
    (*t)[i] = tr_s_linear(z[i], 0.35);
}

void wfg1_t2(double **t, double *z, int N, int K) {
  int i;

  if(*t != z)
    memcpy(*t, z, sizeof(double) * K);

  for(i = K; i < N; i++) 
    (*t)[i] = tr_b_flat(z[i], 0.8, 0.75, 0.85); 
}

void wfg1_t3(double **t, double *z, int N) {
  int i;

  for(i = 0; i < N; i++) 
    (*t)[i] = tr_b_poly(z[i], 0.02);
}

void wfg1_t4(double **t, double *z, int M, int N, int K) {
  int i, j, idx, head, tail;

  for(i = 1; i <= M - 1; i++) {
    idx = 0;

    head = (i-1) * K / (M - 1);
    tail = i * K / (M - 1);

    for(j = head; j < tail;  j++) {
      w[idx] = 2 * (j + 1); 
      y[idx] = z[j];
      idx++;
    }
    (*t)[i-1] = tr_r_sum(y, w, idx); 
  }

  idx = 0;

  for(j = K; j < N; j++) {
    w[idx] = 2*(j + 1);
    y[idx] = z[j];
    idx++;
  } 

  (*t)[M-1] = tr_r_sum(y, w, idx); 
}

void wfg2_t2(double **t, double *z, int N, int K) {
  int i, j, idx, head, tail, l;

  l = N - K;

  if(*t != z)
    memcpy(*t, z, sizeof(double) * K);

  for(i = K + 1; i <= K + l/2; i++) {
    head = K + 2 * (i - K) - 2;
    tail = K + 2 * (i - K);
    idx = 0;

    for(j = head; j < tail;  j++) {
      y[idx] = z[j];
      idx++;
    }
    (*t)[i-1] = tr_r_nonsep(y, 2, idx); 
  }
} 

void wfg2_t3(double **t, double *z, int M, int N, int K) {
  int i, j, idx, head, tail, l;

  l = N - K;

  for(i = 1; i <= M - 1; i++) {
    idx = 0;

    head = (i-1) * K / (M - 1);
    tail = i * K / (M - 1);

    for(j = head; j < tail;  j++) {
      w[idx] = 1; 
      y[idx] = z[j];
      idx++;
    }
    (*t)[i-1] = tr_r_sum(y, w, idx); 
  }

  idx = 0;

  for(j = K; j < K + l/2; j++) {
    w[idx] = 1;
    y[idx] = z[j];
    idx++;
  } 

  (*t)[M-1] = tr_r_sum(y, w, idx); 
}

void wfg4_t1(double **t, double *z, int N) {
  int i;

  for(i = 0; i < N; i++) 
    (*t)[i] = tr_s_multi(z[i], 30, 10, 0.35); 
}

void wfg4_t2(double **t, double *z, int M, int N, int K) {
  int i, j, idx, head, tail;

  for(i = 1; i <= M - 1; i++) {
    idx = 0;

    head = (i-1) * K / (M - 1);
    tail = i * K / (M - 1);

    for(j = head; j < tail;  j++) {
      w[idx] = 1; 
      y[idx] = z[j];
      idx++;
    }
    (*t)[i-1] = tr_r_sum(y, w, idx); 
  }

  idx = 0;

  for(j = K; j < N; j++) { // modified from wfg2_t3
    w[idx] = 1;
    y[idx] = z[j];
    idx++;
  } 

  (*t)[M-1] = tr_r_sum(y, w, idx); 
}

void wfg5_t1(double **t, double *z, int N) {
  int i;

  for(i = 0; i < N; i++) 
    (*t)[i] = tr_s_decept(z[i], 0.35, 0.001, 0.05);
} 

void wfg6_t2(double **t, double *z, int M, int N, int K) {
  int i, j, idx, head, tail;

  for(i = 1; i <= M-1; i++) {
    head = (i-1) * K / (M - 1);
    tail = i * K / (M - 1);
    idx = 0;

    for(j = head; j < tail;  j++) {
      y[idx] = z[j];
      idx++;
    }
    (*t)[i-1] = tr_r_nonsep(y, K / (M - 1), idx); 
  }

  idx = 0;

  for(j = K; j < N;  j++) {
    y[idx] = z[j];
    idx++;
  }
  (*t)[M-1] = tr_r_nonsep(y, N-K, idx); 
} 

void wfg7_t1(double **t, double *z, int N, int K) {
  int i, j, idx;

  for(i = 0; i < K; i++) { 
    idx = 0;

    for(j = i + 1; j < N;  j++) {
      w[idx] = 1; 
      y[idx] = z[j];
      idx++;
    }
    (*t)[i] = tr_b_param(z[i], tr_r_sum(y, w, idx), 0.98/49.98, 0.02, 50); 
  }

  for(i = K; i < N; i++) 
    (*t)[i] = z[i];
}
 
void wfg8_t1(double **t, double *z, int N, int K) {
  int i, j, idx;

  if(*t != z)
    memcpy(*t, z, sizeof(double) * K);

  for(i = K; i < N; i++) { 
    idx = 0;

    for(j = 0; j <= i - 1;  j++) {
      w[idx] = 1; 
      y[idx] = z[j];
      idx++;
    }
    (*t)[i] = tr_b_param(z[i], tr_r_sum(y, w, idx), 0.98/49.98, 0.02, 50); 
  }
}

void wfg9_t1(double **t, double *z, int N) {
  int i, j, idx;

  for(i = 0; i < N - 1; i++) { 
    idx = 0;

    for(j = i+1; j < N;  j++) {
      w[idx] = 1; 
      y[idx] = z[j];
      idx++;
    }
    (*t)[i] = tr_b_param(z[i], tr_r_sum(y, w, idx), 0.98/49.98, 0.02, 50); 
  }

  (*t)[N-1] = z[N-1];
}

void wfg9_t2(double **t, double *z, int N, int K) {
  int i;

  for(i = 0; i < K; i++) 
    (*t)[i] = tr_s_decept(z[i], 0.35, 0.001, 0.05);
 
  for(i = K; i < N; i++) 
    (*t)[i] = tr_s_multi(z[i], 30, 95, 0.35); 
}

/* x: set of M underlying parameters */
void calc_x(double **x, double *t, double *A, int M) {
  int i;

  for(i = 0; i < M - 1; i++) {
    (*x)[i] = (t[i] - 0.5);
    (*x)[i] *= max(t[M-1], A[i]);
    (*x)[i] += 0.5;
  }

  (*x)[M-1] = t[M-1];
}
 
/* Objective functions */ 
void calc_f(double **f, double *x, double *S, double *h, int M) {
  int i;

  for(i = 0; i < M; i++)
    (*f)[i] = x[M-1] + S[i] * h[i];
}

void print_vec(char *s, double *v, int n) {
  int i;

  printf("%s", s);

  for(i = 0; i < n; i++)
    printf("%f ", v[i]);
  printf("\n");
}

/* No normaliza, en PISA se multiplica y divide vector de parametros por 2 * i
void normalize(double **x, int N) {
  int i;

  for(i = 0; i < N; i++) {
    (*x)[i] = (*x)[i] / (2 * (i + 1));
  }
}*/

void wfg1(double **f, double *z, int M, int N, ...) {
  int K, i;

  va_list arg_list;
  va_start(arg_list, N);
  K = va_arg(arg_list, int);
  va_end(arg_list);

  //normalize(&z, N);
  wfg1_t1(&t, z, N, K);    /* Transformation t1 */
  wfg1_t2(&t, t, N, K);    /* Transformation t2 */
  wfg1_t3(&t, t, N);       /* Transformation t3 */
  wfg1_t4(&t, t, M, N, K); /* Transformation t4 */

  calc_x(&x, t, A, M);     /* Vector x */ 
 
  /* Shape */ 
  h[0] = sh_convex_1(x, M);

  for(i = M - 2; i > 0; i--) 
    h[i] = sh_convex_m(x, M, i + 1);
 
  h[M-1] = sh_mixed_M(x, 5, 1);

  calc_f(f, x, S, h, M);   /* Objective functions */ 
}

void wfg2(double **f, double *z, int M, int N, ...) {
  int i, K;

  va_list arg_list;
  va_start(arg_list, N);
  K = va_arg(arg_list, int);
  va_end(arg_list);

//  normalize(&z, N);
  wfg1_t1(&t, z, N, K);    /* Transformation t1 */
  wfg2_t2(&t, t, N, K);    /* Transformation t2 */
  wfg2_t3(&t, t, M, N, K); /* Transformation t3 */

  calc_x(&x, t, A, M);     /* Vector x */ 

  /* Shape */ 
  h[0] = sh_convex_1(x, M);

  for(i = M - 2; i > 0; i--) 
    h[i] = sh_convex_m(x, M, i + 1);
 
  h[M-1] = sh_disc_M(x, 5, 1, 1);
 
  calc_f(f, x, S, h, M);   /* Objective functions */ 
}

void wfg3(double **f, double *z, int M, int N, ...) {
  int K;

  va_list arg_list;
  va_start(arg_list, N);
  K = va_arg(arg_list, int);
  va_end(arg_list);
  //normalize(&z, N);

  wfg1_t1(&t, z, N, K);    /* Transformation t1 */
  wfg2_t2(&t, t, N, K);    /* Transformation t2 */
  wfg2_t3(&t, t, M, N, K); /* Transformation t3 */
  calc_x(&x, t, A, M);     /* Vector x */ 
  sh_linear(&h, x, M);     /* Shape */ 
  calc_f(f, x, S, h, M);   /* Objective functions */ 
}

void wfg4(double **f, double *z, int M, int N, ...) {
  int K;

  va_list arg_list;
  va_start(arg_list, N);
  K = va_arg(arg_list, int);
  va_end(arg_list);
  //normalize(&z, N);

  wfg4_t1(&t, z, N);       /* Transformation t1 */
  wfg4_t2(&t, t, M, N, K); /* Transformation t2 */
  calc_x(&x, t, A, M);     /* Vector x */ 
  sh_concave(&h, x, M);    /* Shape */ 
  calc_f(f, x, S, h, M);   /* Objective functions */ 
}

void wfg5(double **f, double *z, int M, int N, ...) {
  int K;

  va_list arg_list;
  va_start(arg_list, N);
  K = va_arg(arg_list, int);
  va_end(arg_list);
  //normalize(&z, N);

  wfg5_t1(&t, z, N);       /* Transformation t1 */
  wfg4_t2(&t, t, M, N, K); /* Transformation t2 */
  calc_x(&x, t, A, M);     /* Vector x */ 
  sh_concave(&h, x, M);    /* Shape */ 
  calc_f(f, x, S, h, M);   /* Objective functions */ 
}

void wfg6(double **f, double *z, int M, int N, ...) {
  int K;

  va_list arg_list;
  va_start(arg_list, N);
  K = va_arg(arg_list, int);
  va_end(arg_list);
  //normalize(&z, N);

  wfg1_t1(&t, z, N, K);    /* Transformation t1 */
  wfg6_t2(&t, t, M, N, K); /* Transformation t2 */
  calc_x(&x, t, A, M);     /* Vector x */ 
  sh_concave(&h, x, M);    /* Shape */ 
  calc_f(f, x, S, h, M);   /* Objective functions */ 
}

void wfg7(double **f, double *z, int M, int N, ...) {
  int K;

  va_list arg_list;
  va_start(arg_list, N);
  K = va_arg(arg_list, int);
  va_end(arg_list);
  //normalize(&z, N);

  wfg7_t1(&t, z, N, K);    /* Transformation t1 */
  wfg1_t1(&t, t, N, K);    /* Transformation t2 */
  wfg4_t2(&t, t, M, N, K); /* Transformation t3 */
  calc_x(&x, t, A, M);     /* Vector x */ 
  sh_concave(&h, x, M);    /* Shape */ 
  calc_f(f, x, S, h, M);   /* Objective functions */ 
}

void wfg8(double **f, double *z, int M, int N, ...) {
  int K;

  va_list arg_list;
  va_start(arg_list, N);
  K = va_arg(arg_list, int);
  va_end(arg_list);
  //normalize(&z, N);

  wfg8_t1(&t, z, N, K);    /* Transformation t1 */
  wfg1_t1(&t, t, N, K);    /* Transformation t2 */
  wfg4_t2(&t, t, M, N, K); /* Transformation t3 */
  calc_x(&x, t, A, M);     /* Vector x */ 
  sh_concave(&h, x, M);    /* Shape */ 
  calc_f(f, x, S, h, M);   /* Objective functions */ 
}

void wfg9(double **f, double *z, int M, int N, ...) {
  int K;

  va_list arg_list;
  va_start(arg_list, N);
  K = va_arg(arg_list, int);
  va_end(arg_list);
  //normalize(&z, N);

  wfg9_t1(&t, z, N);       /* Transformation t1 */
  wfg9_t2(&t, t, N, K);    /* Transformation t2 */
  wfg6_t2(&t, t, M, N, K); /* Transformation t3 */
  calc_x(&x, t, A, M);     /* Vector x */ 
  sh_concave(&h, x, M);    /* Shape */ 
  calc_f(f, x, S, h, M);   /* Objective functions */ 
}

void init_WFG(int wfg, int M, int N, int K) {
  double v;
  int i;

  if(! (M >= 2)) {
    printf("Error, M must be greather than 2.\n");
    exit(1);
  }

  if(! (K >= 1)) {
    printf("Error, K must be greather than 1.\n");
    exit(1);
  }

  if(! (K < N)) {
    printf("Error, K must be less than N.\n");
    exit(1);
  }

  if(! (K % (M-1) == 0) ) {
    printf("Error, K must in {M-1, 2(M-1), 3(M-1),...}.\n");
    exit(1);
  }

  zmax = (double *) malloc(sizeof(double) * N); //max values
  t = (double *) malloc(sizeof(double) * N);  // transicion

  x = (double *) malloc(sizeof(double) * M);  // vector subyacente
  h = (double *) malloc(sizeof(double) * M);  // shape 
  S = (double *) malloc(sizeof(double) * M);  // constant
  A = (double *) malloc(sizeof(double) * (M-1)); // constant

  y = (double *) malloc(sizeof(double) * N);  // tmp
  w = (double *) malloc(sizeof(double) * N);

  for(i = 0; i < M; i++)
    S[i] = 2*(i+1);

  A[0] = 1;

  v = (wfg == 3)? 0 : 1; // degenerate

  for(i = M - 2; i > 0; i--)
    A[i] = v; 

  for(i = 0; i < N; i++)
    zmax[i] = 2 * (i + 1);
}

void free_WFG() {
  free(zmax);
  free(t);
  free(x);
  free(h);
  free(S);
  free(A);
  free(y);
  free(w);
}

