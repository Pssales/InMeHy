/* function.c   Test problem definitions */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "function.h"

#include "wfg.h"
#include <stdlib.h>

void findfunc(char *nom, void (**apf)(double **, double *, int, int,...)) {

  char *name[] = { "ZDT1", "ZDT2", "ZDT3", "ZDT4", "ZDT6",
                  "DTLZ1", "DTLZ2", "DTLZ3","DTLZ4", 
                  "DTLZ5", "DTLZ6", "DTLZ7",
                  "WFG1", "WFG2", "WFG3", "WFG4", "WFG5", "WFG6",
                  "WFG7", "WFG8", "WFG9"
                };

  void (*fdic[])(double **, double *, int, int, ...) = {
                   zdt1, zdt2, zdt3, zdt4, zdt6,
                   dtlz1, dtlz2, dtlz3,dtlz4, dtlz5,
                   dtlz6, dtlz7,
                   wfg1, wfg2, wfg3, wfg4, wfg5, wfg6, 
                   wfg7, wfg8, wfg9
                };

  int i = 0;

  while(name[i] != NULL) {

    if(strcmp(nom, name[i]) == 0) {
      *apf = fdic[i];
      return;
    }
    i++;
  }
  *apf = fdic[0];
}


/* Test problem ZDT1

   Real variables = 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1]
   PFTrue is formed with g(x) = 1
   Convex Pareto-optimal front
 */

void zdt1 (double **f, double *x, int m, int n, ...) {
  double f1, f2, g, h;
  int i;

  f1 = x[0];
  g = 0;

  for(i = 1; i < n; i++) 
    g += x[i];

  g = 9*g/(n-1);
  g += 1;
  h = 1 - sqrt(f1/g);
  f2 = g*h;

  (*f)[0] = f1;
  (*f)[1] = f2;
}


/* Test problem ZDT2

   Real variables = 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1]
   PFTrue is formed with g(x) = 1
   Non-convex Pareto-optimal front
*/
void zdt2(double **f, double *x, int m, int n, ...) {
  double f1, f2, g, h;
  int i;

  f1 = x[0];
  g = 0;

  for(i = 1; i < n; i++)
    g += x[i];

  g = 9*g/(n-1);
  g += 1;
  h = 1 - pow((f1/g),2);
  f2 = g*h;
  (*f)[0] = f1;
  (*f)[1] = f2;
}

/* Test problem ZDT3

   Real variables = 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1]
   PFTrue is formed with g(x) = 1
   Pareto-optimal front disconnected, consisting of
   several noncontiguous convex parts.
*/
void zdt3(double **f, double *x, int m, int n, ...) {
  double f1, f2, g, h;
  int i;

  f1 = x[0];
  g = 0;

  for(i = 1; i < n; i++)
    g += x[i];

  g = 9*g/(n-1);
  g += 1;
  h = 1 - sqrt(f1/g) - (f1/g)*sin(10*PI*f1);
  f2 = g*h;
  (*f)[0] = f1;
  (*f)[1] = f2;
}

/* Test problem ZDT4

   Real variables = 10
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x1 in [0,1], x2,...,xn in [-5,5]
   PFTrue is formed with g(x) = 1, PFlocal g(x) = 1.25
   21^9 local Pareto-optimal fronts (multifrontality)
*/
void zdt4(double **f, double *x, int m, int n, ...) {
  double f1, f2, g, h;
  int i;

  f1 = x[0];
  g = 0;

  for(i = 1; i < n; i++)
    g += x[i]*x[i] - 10*cos(4*PI*x[i]);

  g += 1 + 10*(n-1);
  h = 1 - sqrt(f1/g);
  f2 = g*h;
  (*f)[0] = f1;
  (*f)[1] = f2;
}

/* Test problem ZDT6

   Real variables = 10
   Bin variables = 0
   Objectives = 2
   constraints = 0
   Values of xi in [0,1]
   PFTrue is formed with g(x) = 1 and is nonconvex.
   PTrue are nonuniformly distributed along the PFTrue,
   the density of the solutions is lowest near the PFTrue
   and highest away from the front.
*/
void zdt6(double **f, double *x, int m, int n, ...) {
  double f1, f2, g, h;
  int i;

  f1 = 1 - ( exp(-4*x[0]) ) * pow( (sin(6*PI*x[0])), 6);
  g = 0;

  for(i = 1; i < n; i++)
    g += x[i];

  g = g/9;
  g = pow(g,0.25);
  g = 1 + 9*g;
  h = 1 - pow((f1/g),2);
  f2 = g*h;
  (*f)[0] = f1;
  (*f)[1] = f2;
}

/* Test problem DTLZ1

   Real variables = 12
   Bin variables = 0
   Objectives = 2
   constraints = 0
   Values of xi in [0,1]
   PFTrue is formed with g(x) = 1 and is nonconvex.
   PTrue are nonuniformly distributed along the PFTrue,
   the density of the solutions is lowest near the PFTrue
   and highest away from the front.
*/
void dtlz1(double **f, double *x, int m, int n, ...) {
  double g;
  int i, j, t, c;

  g = 0;
  c = n - m + 1;

  for(i = n - c; i < n; i++)
    g += pow(x[i] - 0.5, 2) - cos(20*PI*(x[i] - 0.5));

  g = 100*(c + g);

  for(i = 0; i < m; i++) {
    (*f)[i] = 0.5*(1 + g);

    t = m - i - 1; 

    for(j = 0; j < t; j++) {
      (*f)[i] *= x[j];
    }

    if(t < m - 1) {
      (*f)[i] *= (1 - x[t]);
    }
  }
}

void dtlz7(double **f, double *x, int m, int n, ...) {
  double g, h;
  int i, c;

  g = h = 0;
  c = n - m + 1;

  for(i = 0; i < m - 1; i++) 
    (*f)[i] = x[i];

  for(i = n - c; i < n; i++)
    g += x[i];

  g = 1 + 9 * g / c;

  for(i = 0; i < m - 1; i++)
    h += x[i] / (1 + g) * (1 + sin(3*PI*x[i]));

  h = m - h;

  (*f)[m-1] = (1 + g) * h;
}

void dtlz2(double **f, double *x, int m, int n, ...) {
  double g;
  int i, j, t, c;

  g = 0;
  c = n - m + 1;

  for(i = n - c; i < n; i++)
    g += pow(x[i] - 0.5, 2);


  for(i = 0; i < m; i++) {
    (*f)[i] = (1 + g);

    t = m - i - 1; 

    for(j = 0; j < t; j++) {
      (*f)[i] *= cos(x[j]*PI*0.5);
    }

    if(t < m - 1) {
      (*f)[i] *= sin(x[t]*PI*0.5);
    }
  }
}

void dtlz3(double **f, double *x, int m, int n, ...) {
  double g;
  int i, j, t, c;

  g = 0;
  c = n - m + 1;

  for(i = n - c; i < n; i++)
    g += pow(x[i] - 0.5, 2) - cos(20*PI*(x[i] - 0.5));

  g = 100*(c + g);

  for(i = 0; i < m; i++) {
    (*f)[i] = (1 + g);

    t = m - i - 1; 

    for(j = 0; j < t; j++) {
      (*f)[i] *= cos(x[j]*PI*0.5);
    }

    if(t < m - 1) {
      (*f)[i] *= sin(x[t]*PI*0.5);
    }
  }
}

void dtlz4(double **f, double *x, int m, int n, ...) {
  double g, alpha = 100;
  int i, j, t, c;

  g = 0;
  c = n - m + 1;

  for(i = n - c; i < n; i++)
    g += pow(x[i] - 0.5, 2);


  for(i = 0; i < m; i++) {
    (*f)[i] = (1 + g);

    t = m - i - 1; 

    for(j = 0; j < t; j++) {
      (*f)[i] *= cos(pow(x[j], alpha)*PI*0.5);
    }

    if(t < m - 1) {
      (*f)[i] *= sin(pow(x[t], alpha)*PI*0.5);
    }
  }
}

void dtlz5(double **f, double *x, int m, int n, ...) {
  double g, *theta, p;
  int i, j, t, c;

  theta = (double *) malloc(sizeof(double) * (m-1));
  g = 0;
  c = n - m + 1;

  for(i = n - c; i < n; i++)
    g += pow(x[i] - 0.5, 2);

  theta[0] = x[0] * PI * 0.5;
  p = PI / (4 * (1 + g));

  for(i = 1; i < m-1; i++)
    theta[i] = p * (1 + 2 * g * x[i]);

  for(i = 0; i < m; i++) {
    (*f)[i] = (1 + g);

    t = m - i - 1; 

    for(j = 0; j < t; j++) {
      (*f)[i] *= cos(theta[j]);
    }

    if(t < m - 1) {
      (*f)[i] *= sin(theta[t]);
    }
  }

  free(theta);
}

void dtlz6(double **f, double *x, int m, int n, ...) {
  double g, *theta, p;
  int i, j, t, c;

  theta = (double *) malloc(sizeof(double) * (m-1));
  g = 0;
  c = n - m + 1;

  for(i = n - c; i < n; i++)
    g += pow(x[i], 0.1);

  theta[0] = x[0] * PI * 0.5;
  p = PI / (4 * (1 + g));

  for(i = 1; i < m-1; i++)
    theta[i] = p * (1 + 2 * g * x[i]);

  for(i = 0; i < m; i++) {
    (*f)[i] = (1 + g);

    t = m - i - 1; 

    for(j = 0; j < t; j++) {
      (*f)[i] *= cos(theta[j]);
    }

    if(t < m - 1) {
      (*f)[i] *= sin(theta[t]);
    }
  }

  free(theta);
}

