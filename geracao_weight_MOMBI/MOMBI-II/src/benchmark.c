/**************************************************************
 * testf.c   Definition of test functions.                    *
 *                                                            *
 * March 2013                                                 *
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "benchmark.h"
#include "wfg.h"
//#include "knapsack.h"
#include "numeric.h"
#include "vector.h"


//EMO_Knapsack knap;

/* Test problem FON1 (Fonseca)

   Real variables = 2 
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are open
   PFTrue is formed with x* = [-1, 1]
   POS connected, symmetric; PFTrue connected, concave
*/
void fon1(EMO_MOP *mop, double *f, double *x) {
  double s1, s2;

  s1 = - pow(x[0] - 1.0, 2.0) - pow(x[1] + 1.0, 2.0);
  s2 = - pow(x[0] + 1.0, 2.0) - pow(x[1] - 1.0, 2.0);

  f[0] = 1.0 - exp(s1);
  f[1] = 1.0 - exp(s2);

  mop->feval++;
}

void fon1Range(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -1.0;
    mop->xmax[i] = 1.0;
  }
}

/* Test problem FON2, (Fonseca and Fleming, 1995)

   Real variables = n (scalable)
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [-4,4]
   PFTrue is formed with x* = [-1/sqrt(n), 1/sqrt(n)]
                         f2* = 1-exp(-(2-sqrt(-ln(1-f1*)))^2)
   POS connected, symmetric; PFTrue connected, concave
*/
void fon2(EMO_MOP *mop, double *f, double *x) {
  double s, s1, s2;
  int i;

  s1 = s2 = 0.0;

  s = 1.0 / sqrt((double) mop->nvar);

  for (i = 0; i < mop->nvar; i++) {
    s1 += pow(x[i] - s, 2.0);
    s2 += pow(x[i] + s, 2.0);
  }

  f[0] = 1.0 - exp(-s1);
  f[1] = 1.0 - exp(-s2);

  mop->feval++;
}

void fon2Range(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -4.0;
    mop->xmax[i] = 4.0;
  }
}

/* Test problem KUR (Kursawe, 1990)

   Real variables = 3 
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [-5,5]
   POS disconnected, symmetric; PFTrue disconnected
*/
void kur(EMO_MOP *mop, double *f, double *x) {
  double a, e1, e2;
  int i;

  a = x[1] * x[1];
  e1 = - 0.2 * sqrt(x[0] * x[0] + a);
  e2 = - 0.2 * sqrt(a + x[2] * x[2]);

  f[0] = - 10.0 * (exp(e1) + exp(e2));
  f[1] = 0.0;

  for (i = 0; i < 3; i++) 
    f[1] += pow(fabs(x[i]), 0.8) + 5.0 * sin(pow(x[i], 3.0));

  mop->feval++;
}

void kurRange(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -5.0;
    mop->xmax[i] = 5.0;
  }
}

/* Test problem LAU (Laumanns)

   Real variables = 2 
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [-50,50]
   POS disconnected; PFTrue convex
*/
void lau(EMO_MOP *mop, double *f, double *x) {
  f[0] = x[0] * x[0] + x[1] * x[1];
  f[1] = pow(x[0] + 2.0, 2.0) + x[1] * x[1];
  mop->feval++;
}

void lauRange(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -50.0;
    mop->xmax[i] = 50.0;
  }
}

/* Test problem LIS

   Real variables = 2 
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [-5,10]
   POS disconnected; PFTrue disconnected and concave
*/
void lis(EMO_MOP *mop, double *f, double *x) {
  f[0] = pow(x[0] * x[0] + x[1] * x[1], 1.0 / 8.0);
  f[1] = pow(pow(x[0] - 0.5, 2.0) + pow(x[1] - 0.5, 2.0), 1.0 / 4.0);
  mop->feval++;
}

void lisRange(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -5.0;
    mop->xmax[i] = 10.0;
  }
}

/* Test problem MUR (Murata)

   Real variables = 2 
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x1 is in [1,4], x2 is in [1,2]
   POS connected, PFTrue concave
*/
void mur(EMO_MOP *mop, double *f, double *x) {
  f[0] = 2.0 * sqrt(x[0]);
  f[1] = x[0] * (1.0 - x[1]) + 5.0;
  mop->feval++;
}

void murRange(EMO_MOP *mop) {
  mop->xmin[0] = 1.0;
  mop->xmax[0] = 4.0;
  mop->xmin[1] = 1.0;
  mop->xmax[1] = 2.0;
}

/*void mur_sol(double *f, double *x) {
  x[0] = EMO_rndreal(1.0,4.0);  
  x[1] = 1.0;

  mur(f, x);
}*/

/* Test problem POL (Poloni, 2000)

   Real variables = 2
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [-PI,PI]
   PFTrue is difficult to know 
   POS disconnected; PFTrue disconnected and convex
*/
void pol(EMO_MOP *mop, double *f, double *x) {
  double a1, a2, b1, b2;

  a1 = 0.5 * sin(1.0) - 2.0 * cos(1.0) + sin(2.0) - 1.5 * cos(2.0);
  a2 = 1.5 * sin(1.0) - cos(1.0) + 2.0 * sin(2.0) - 0.5 * cos(2.0);
  b1 = 0.5 * sin(x[0]) - 2.0 * cos(x[0]) + sin(x[1]) - 1.5 * cos(x[1]);
  b2 = 1.5 * sin(x[0]) - cos(x[0]) + 2.0 * sin(x[1]) - 0.5 * cos(x[1]);
  f[0] = 1.0 + pow(a1 - b1, 2.0) + pow(a2 - b2, 2.0);
  f[1] = pow(x[0] + 3.0, 2.0) + pow(x[1] + 1.0, 2.0);
  mop->feval++;
}

void polRange(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -PI;
    mop->xmax[i] = PI;
  }
}

/* Test problem QUA (Quagliarella)

   Real variables >= 16
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [-5.12, 5.12]
   POS disconnected, PFTrue concave
*/
void qua(EMO_MOP *mop, double *f, double *x) {
  double a1, a2;
  int i;

  a1 = a2 = 0;

  for(i = 0; i < mop->nvar; i++) {
    a1 += x[i] * x[i] - 10.0 * cos(2.0 * PI * x[i]) + 10.0;
    a2 += pow(x[i] - 1.5, 2.0) - 10.0 * cos(2.0 * PI * (x[i] - 1.5)) + 10.0;
  }

  f[0] = pow(a1 / (double) mop->nvar, 1.0 / 4.0);
  f[1] = pow(a2 / (double) mop->nvar, 1.0 / 4.0);
  mop->feval++;
}

void quaRange(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -5.12;
    mop->xmax[i] = 5.12;
  }
}

/* Test problem REN1 (Rendon)

   Real variables = 2 
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [-3, 3]
   POS connected, PFTrue convex
*/
void ren1(EMO_MOP *mop, double *f, double *x) {
  double a, b;

  a = x[0] * x[0];
  b = x[1] * x[1];
 
  f[0] = 1.0 / (a + b + 1.0);
  f[1] = a + 3.0 * b + 1.0;
  mop->feval++;
}

/*void ren1_sol(double *f, double *x) {
  int i;

  x[0] = EMO_rndreal(1.0,4.0);  
  x[1] = 1;

  mur(f, x);
}*/

/* Test problem REN2 (Rendon)

   Real variables = 2 
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [-3, 3]
   POS connected, PFTrue convex
*/
void ren2(EMO_MOP *mop, double *f, double *x) {
  f[0] = x[0] + x[1] + 1.0;
  f[1] = x[0] * x[0] + 2.0 * x[1] - 1.0;
  mop->feval++;
}

void renRange(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -3.0;
    mop->xmax[i] = 3.0;
  }
}

/* Test problem SCH1 (Schaffer, 1984)

   Real variables = 1 
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [-A,A], A=10..10^5 (more difficulty)
   PFTrue is formed with f1* = [0,4], f2* = (sqrt(f1*) - 2)^2
   POS connected, PFTrue convex
*/
void sch1(EMO_MOP *mop, double *f, double *x) {
  f[0] = x[0] * x[0];
  f[1] = pow(x[0] - 2.0, 2.0);
  mop->feval++;
}

void sch1Range(EMO_MOP *mop) {
  mop->xmin[0] = -10.0;
  mop->xmax[0] = 10.0;
}

/* Test problem SCH2 (Schaffer, 1984)

   Real variables = 1 
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [-5,10]
   PFTrue is formed with x* in {[1,2] union [4,5]}
   POS disconnected; PFTrue convex are disconnected
*/
void sch2(EMO_MOP *mop, double *f, double *x) {

  if (x[0] <= 1.0)
    f[0] = - x[0];
  else if (x[0] <= 3.0)
    f[0] = x[0] - 2.0;
  else if (x[0] <= 4.0)
    f[0] = 4.0 - x[0];
  else f[0] = x[0] - 4.0;

  f[1] = pow(x[0] - 5.0, 2.0);
  mop->feval++;
}

void sch2Range(EMO_MOP *mop) {
  mop->xmin[0] = -5.0;
  mop->xmax[0] = 10.0;
}

/* Test problem VIE1 (Viennet, 1996)

   Real variables = 2 
   Bin variables = 0
   Objectives = 3
   Constraints = 0
   Values of x are in [-2,2]
   POS connected and symmetric, PFTrue curved surface
*/
void vie1(EMO_MOP *mop, double *f, double *x) {
  double a;

  a = x[0] * x[0];

  f[0] = a + pow(x[1] - 1.0, 2.0);
  f[1] = a + pow(x[1] + 1.0, 2.0) + 1.0;
  f[2] = pow(x[0] - 1.0, 2.0) + x[1] * x[1] + 2.0;
  mop->feval++;
}

void vie1Range(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -2.0;
    mop->xmax[i] = 2.0;
  }
}

/* Test problem VIE2 (Viennet, 1996)

   Real variables = 2 
   Bin variables = 0
   Objectives = 3
   Constraints = 0
   Values of x are in [-4,4]
   POS connected, PFTrue disconnected
*/
void vie2(EMO_MOP *mop, double *f, double *x) {
  f[0] = pow(x[0] - 2.0, 2.0) * 0.5 + pow(x[1] + 1.0, 2.0) / 13.0 + 3.0;
  f[1] = pow(x[0] + x[1] - 3.0, 2.0) / 36.0 + pow(-x[0] + x[1] + 2.0, 2.0) * 0.125 - 17.0;
  f[2] = pow(x[0] + 2.0 * x[1] - 1.0, 2.0) / 175.0 + pow(2.0 * x[1] - x[0], 2.0) / 17.0 - 13.0;
  mop->feval++;
}

void vie2Range(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -4.0;
    mop->xmax[i] = 4.0;
  }
}

/* Test problem VIE3 (Viennet, 1996)

   Real variables = 2 
   Bin variables = 0
   Objectives = 3
   Constraints = 0
   Values of x are in [-3,3]
   POS disconnected and unsymmetric, PFTrue connected
*/
void vie3(EMO_MOP *mop, double *f, double *x) {
  double a;

  a = x[0] * x[0] + x[1] * x[1];
 
  f[0] = 0.5 * a + sin(a);
  f[1] = pow(3.0 * x[0] - 2.0 * x[1] + 4.0, 2.0) * 0.125 + pow(x[0] - x[1] + 1.0, 2.0) / 27.0 + 15.0;
  f[2] = 1.0 / (a + 1.0) - 1.1 * exp(-a);
  mop->feval++;
}

void vie3Range(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -3.0;
    mop->xmax[i] = 3.0;
  }
}

/* Test problem DEB1

   Real variables = 2 
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1]
   POS connected; PFTrue connected, concave
*/
void deb1(EMO_MOP *mop, double *f, double *x) {
  double g, h;

  g = 1.0 + x[1] * x[1];
  h = (x[0] <= g) ? 1.0 - pow(x[0] / g, 2.0) : 0.0;

  f[0] = x[0];
  f[1] = g * h;
  mop->feval++;
}

/* Test problem DEB2

   Real variables = 2 
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1]
   POS disconnected, PFTrue disconnected
*/
void deb2(EMO_MOP *mop, double *f, double *x) {
  double g, h;

  g = 1.0 + 10.0 * x[1];
  h = 1.0 - pow(x[0] / g, 2.0) - x[0] / g * sin(12.0 * PI * x[0]);

  f[0] = x[0];
  f[1] = g * h;
  mop->feval++;
}

/* Test problem DEB3

   Real variables = 2 
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1]
   POS connected; PFTrue connected, concave
*/
void deb3(EMO_MOP *mop, double *f, double *x) {
  double g, h;

  f[0] = 1.0 - exp(-4.0 * x[0]) * pow(sin(10.0 * PI * x[0]), 4.0);
 
  g = 1.0 + x[1] * x[1];
  h = (f[0] <= g) ? 1.0 - pow(f[0] / g, 10.0) : 0.0;

  f[1] = g * h;
  mop->feval++;
}

void debRange(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = 0.0;
    mop->xmax[i] = 1.0;
  }
}

/* Test problem OKA1

   Real variables = 2
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x1 are in [6*sin(PI/12), 6*sin(PI/12)+2*PI*cos(PI/12)]
                       [1.5529142706, 7.6220052302]
             x2 are in [-2*PI*sin(PI/12), 6*cos(PI/12)]
                       [-1.6262080214 5.7955549577]
   PFTrue is at f2 = sqrt(2*PI) - sqrt(f1), f1 in [0, 2 * PI]
                x2' = 3 * cos(x1') + 3, x1' in [0, 2*PI]
   Convex Pareto-optimal front
*/

void oka1(EMO_MOP *mop, double *f, double *x) {
  double x0, x1;
 
  x0 = cos(PI/12.0) * x[0] - sin(PI/12.0) * x[1];
  x1 = sin(PI/12.0) * x[0] + cos(PI/12.0) * x[1];

  f[0] = x0;
  f[1] = sqrt(2.0 * PI) - sqrt(fabs(x0)) + 2.0 * pow(fabs(x1 - 3.0 * cos(x0) - 3.0), 1.0/3.0);
  mop->feval++;
}

void oka1Range(EMO_MOP *mop) {
  mop->xmin[0] = 6.0 * sin(PI/12.0);
  mop->xmax[0] = mop->xmin[0] + 2 * PI * cos(PI/12.0);
  mop->xmin[1] = -2.0 * PI * sin(PI/12.0);
  mop->xmax[1] = 6.0 * cos(PI/12.0);
}

/* Test problem OKA2

   Real variables = 3
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x1 are in [-PI,PI], x2,x3 in [-5,5]
   PFTrue is at f2 = 1 - 1/(4*PI^2) * (f1 + PI)^2, f1 in [-PI, PI]
        (x1,x2,x3) = (x1, 5*cos(x1), 5*sin(x1))
   Non-convex Pareto-optimal front
*/
void oka2(EMO_MOP *mop, double *f, double *x) {
  f[0] = x[0];
  f[1] = 1.0 - pow(x[0] + PI, 2.0) / (4.0 * PI * PI);
  f[1] += pow(fabs(x[1] - 5.0 * cos(x[0])), 1.0/3.0);
  f[1] += pow(fabs(x[2] - 5.0 * sin(x[0])), 1.0/3.0);
  mop->feval++;
}

void oka2Range(EMO_MOP *mop) {
  mop->xmin[0] = -PI;
  mop->xmax[0] = PI;
  mop->xmin[1] = -5.0;
  mop->xmax[1] = 5.0;
}

/* Test problem BNH1 (Binh)

   Real variables = 2
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [-5,10]
   POS connected, symmetric, PFTrue connected, convex
*/
void bnh1(EMO_MOP *mop, double *f, double *x) {
  f[0] = x[0] * x[0] + x[1] * x[1];
  f[1] = pow(x[0] - 5.0, 2.0) + pow(x[1] - 5.0, 2.0);
  mop->feval++;
}

void bnh1Range(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -5.0;
    mop->xmax[i] = 10.0;
  }
}

/* Test problem BNH3 (Binh)

   Real variables = 2
   Bin variables = 0
   Objectives = 3
   Constraints = 0
   Values of x are in [10^-6,10^6]
   PFTrue = [-1e+06, -1e-06, -2e+00]
   POS connected, PFTrue is a point
*/
void bnh3(EMO_MOP *mop, double *f, double *x) {
  f[0] = x[0] - pow(10.0, 6.0);
  f[1] = x[1] - 2.0 * pow(10.0, -6.0);
  f[2] = x[0] * x[1] - 2.0;
  mop->feval++;
}

void bnh3Range(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = 1e-6;
    mop->xmax[i] = 1e6;
  }
}

/* Test problem SDD (Schutze, Dell'Aere, Dellnitz)

   Real variables = (scalable) 
   Bin variables = 0
   Objectives = (scalable)
   Constraints = 0
   POS connected, PFTrue convex
*/
void sdd(EMO_MOP *mop, double *f, double *x) {
  double a, b = 0;
  int i, j;

  for(j = 0; j < mop->nobj; j++) {
    f[j] = 0;
    a = -1.0;

    for(i = 0; i < mop->nvar; i++) {
      switch(j) {
        case 0: a = 1.0;
                break;
        case 1: a = -1.0;
                break;
        default: if(i % (j-1) == 0) 
                   a *= (-1.0);
                 break;
      }

      if(j == i) 
        b = a;
      else
        f[j] += pow(x[i] - a, 2.0);
    }
    f[j] += pow(x[j] - b, 4.0);
  }
  mop->feval++;
}

/* Test problem STZ1 (Schutze)

   Real variables = 2 
   Bin variables = 0
   Objectives = 2 
   Constraints = 0
   POS connected, PFTrue convex
*/
void stz1(EMO_MOP *mop, double *f, double *x) {
  f[0] = pow(x[0] - 1.0, 2.0) + pow(x[1], 2.0);
  f[1] = pow(x[0], 2.0) + pow(x[1] - 1.0, 2.0);
  mop->feval++;
}

/* Test problem STZ2 (Schutze)

   Real variables = (scalable) 
   Bin variables = 0
   Objectives = 2 
   Constraints = 0
   POS connected, PFTrue convex
*/
void stz2(EMO_MOP *mop, double *f, double *x) {
  int i;

  f[0] = pow(x[0] - 1.0, 4.0);

  for(i = 1; i < mop->nvar; i++)
    f[0] += pow(x[i] - 1.0, 2.0);

  f[1] = 0;

  for(i = 0; i < mop->nvar; i++)
    f[1] += pow(x[i] + 1.0, 2.0);
  mop->feval++;
}

/* Test problem STZ3 (Schutze)

   Real variables = 2 
   Bin variables = 0
   Objectives = 2 
   Constraints = 0
   POS connected, PFTrue mixed (convex, concave)
*/
void stz3(EMO_MOP *mop, double *f, double *x) {
  double a, b, c, lambda= 0.85;

  a = sqrt(1.0 + pow(x[0] + x[1], 2.0));
  b = sqrt(1.0 + pow(x[0] - x[1], 2.0));
  c = lambda * exp(-pow(x[0] - x[1], 2.0));

  f[0] = 0.5 * (a + b + x[0] - x[1]) + c;
  f[1] = 0.5 * (a + b - x[0] + x[1]) + c;
  mop->feval++;
}

// verificar
void stzRange(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = -10.0;
    mop->xmax[i] = 10.0;
  }
}

/* Test problem ZDT1 (Zitzler-Deb-Thiele, 2000)

   Real variables >= 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1]
   PFTrue is formed with g(x) = 1
   Convex Pareto-optimal front
*/
void zdt1 (EMO_MOP *mop, double *f, double *x) {
  double g, h;
  int i;

  f[0] = x[0];
  g = 0;

  for(i = 1; i < mop->nvar; i++) 
    g += x[i];

  g = 1.0 + 9.0 * g / (mop->nvar - 1.0);
  h = 1.0 - sqrt(x[0] / g);
  f[1] = g * h;
  mop->feval++;
}

/* Test problem ZDT2 (Zitzler-Deb-Thiele, 2000)

   Real variables >= 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1]
   PFTrue is formed with g(x) = 1
   Non-convex Pareto-optimal front
*/
void zdt2(EMO_MOP *mop, double *f, double *x) {
  double g, h;
  int i;

  f[0] = x[0];
  g = 0;

  for(i = 1; i < mop->nvar; i++) 
    g += x[i];

  g = 1.0 + 9.0 * g / (mop->nvar - 1.0);
  h = 1.0 - pow(x[0] / g, 2.0);
  f[1] = g * h;
  mop->feval++;
}

/* Test problem ZDT3 (Zitzler-Deb-Thiele, 2000)

   Real variables >= 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1]
   PFTrue is formed with g(x) = 1
   Pareto-optimal front disconnected, consisting of
   several non-contiguous convex parts.
*/
void zdt3(EMO_MOP *mop, double *f, double *x) {
  double g, h;
  int i;

  f[0] = x[0];
  g = 0;

  for(i = 1; i < mop->nvar; i++)
    g += x[i];

  g = 1.0 + 9.0 * g / (mop->nvar - 1.0);
  h = 1 - sqrt(x[0] / g) - (x[0] / g) * sin(10.0 * PI * x[0]);
  f[1] = g * h;
  mop->feval++;
}

void zdtRange(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = 0.0;
    mop->xmax[i] = 1.0;
  }
}

/* Test problem ZDT4 (Zitzler-Deb-Thiele, 2000)

   Real variables = 10
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x1 in [0,1]; x2,...,xn in [-5,5]
   PFTrue is formed with g(x) = 1, PFlocal g(x) = 1.25
   21^9 local Pareto-optimal solutions (multi-frontality)
*/
void zdt4(EMO_MOP *mop, double *f, double *x) {
  double g, h;
  int i;

  f[0] = x[0];
  g = 0;

  for(i = 1; i < mop->nvar; i++)
    g += x[i] * x[i] - 10.0 * cos(4.0 * PI * x[i]);

  g += 1.0 + 10.0 * ((double) mop->nvar - 1.0);
  h = 1.0 - sqrt(x[0] / g);
  // ethz: h = 1.0 - pow(x[0] / g, 2.0);
  f[1] = g * h;
  mop->feval++;
}

void zdt4Range(EMO_MOP *mop) {
  int i; //, n;
printf("zdt4Range\n");
  mop->xmin[0] = 0.0;
  mop->xmax[0] = 1.0;

  for(i = 1; i < mop->nvar; i++) {
    mop->xmin[i] = -5.0;
    mop->xmax[i] = 5.0;
  }


/*  n = mop->nvar - 1;

  for(i = n - 1; i > -1; i--) {
    mop->xmin[i] = 0.0;
    mop->xmax[i] = 1.0;
  }

  mop->xmin[n] = -5.0;
  mop->xmax[n] = 5.0;*/
}

/* Test problem ZDT5 (Zitzler-Deb-Thiele, 2000)

   Real variables = 0 
   Bin variables = 11
   Objectives = 2
   Constraints = 0
   Values of x1 is represented by a 30-bit substring,
             x2-11 are represented by 5 bits each
   PFTrue is formed with g(x) = 10, x2-x10 are all 1s.
   Convex, deceptive and multi-frontal problem.
*/
void zdt5(EMO_MOP *mop, double *f, double *x) {
/*void zdt5 (double *xreal, double *xbin, int **gene, double *obj, double *constr, int n, int k) {

  int i, j;
  int u[11];
  int v[11];
  double f1, f2, g, h;

  for(i = 0; i < 11; i++)
    u[i] = 0.0;

  for(j = 0; j < 30; j++)
    if(gene[0][j] == 1)
      u[0]++;

  for(i = 1; i < 11; i++)
    for(j = 0; j < 4; j++)
      if(gene[i][j] == 1)
        u[i]++;

  f1 = 1.0 + u[0];

  for(i = 1; i < 11; i++)
    if(u[i] < 5)
      v[i] = 2.0 + u[i];
    else
      v[i] = 1.0;

  g = 0.0;

  for (i = 1; i < 11; i++)
    g += v[i];

  h = 1.0 / f1;
  f2 = g * h;
  f[0] = f1;
  f[1] = f2;
  mop->feval++;
*/
}

/* Test problem ZDT6 (Zitzler-Deb-Thiele, 2000)

   Real variables >= 10
   Bin variables = 0
   Objectives = 2
   constraints = 0
   Values of xi in [0,1]
   PFTrue is formed with g(x) = 1 and is non-convex.
   PTrue are non-uniformly distributed along the PFTrue,
   the density of the solutions is lowest near the PFTrue
   and highest away from the front.
*/
void zdt6(EMO_MOP *mop, double *f, double *x) {
  double g, h;
  int i;

  f[0] = 1.0 - exp(-4.0 * x[0]) * pow(sin(6.0 * PI * x[0]), 6.0);
  g = 0;

  for(i = 1; i < mop->nvar; i++)
    g += x[i];

  //g = 1.0 + 9.0 * pow(g / 9.0, 0.25);
  g = 1.0 + 9.0 * pow(g / ((double) mop->nvar - 1.0), 0.25);
  h = 1.0 - pow(f[0] / g, 2.0);
  f[1] = g * h;
  mop->feval++;
}

/* Test problem UF1

   Real variables >= 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1] x [-1,1]^(n-1)
   PFTrue is at f2 = 1 - sqrt(f1), f1 in [0,1]
                xj = sin(6*PI*x1 + j*PI/n), j = 2,...,n 0 <= x1 <= 1
   Convex Pareto-optimal front
*/
void uf1(EMO_MOP *mop, double *f, double *x) {
  int j, count1, count2;
  double sum1, sum2, yj;

  sum1   = sum2   = 0.0;
  count1 = count2 = 0;

  for(j = 2; j <= mop->nvar; j++) {
    yj = x[j-1] - sin(6.0 * PI * x[0] + j * PI / (double) mop->nvar);
    yj = yj * yj;

    if(j % 2 == 0) {
      sum2 += yj;
      count2++;
    } 
    else {
      sum1 += yj;
      count1++;
    }
  }

  f[0] = x[0] + 2.0 * sum1 / (double) count1;
  f[1] = 1.0 - sqrt(x[0]) + 2.0 * sum2 / (double) count2;
  mop->feval++;
}

/* Test problem UF2

   Real variables >= 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1]x[-1,1]^(n-1)
   PFTrue is at f2 = 1 - sqrt(f1), f1 in [0, 1]
   xj = {(0.3*x1^2*cos(24*PI*x[0]+4*j*PI/n)+0.6*x[0])*cos(6*PI*x[0]+j*PI/n) j in J1
         (0.3*x1^2*cos(24*PI*x[0]+4*j*PI/n)+0.6*x[0])*sin(6*PI*x[0]+j*PI/n) j in J2
        0 <= x <= 1
   Convex Pareto-optimal front
*/
void uf2(EMO_MOP *mop, double *f, double *x) {
  int j, count1, count2;
  double sum1, sum2, yj;

  sum1   = sum2   = 0.0;
  count1 = count2 = 0;

  for(j = 2; j <= mop->nvar; j++) {
    if(j % 2 == 0) {
      yj = x[j-1]-0.3*x[0]*(x[0]*cos(24.0*PI*x[0]+4.0*j*PI/(double) mop->nvar)+2.0)*sin(6.0*PI*x[0]+j*PI/(double) mop->nvar);
      sum2 += yj*yj;
      count2++;
    }
    else {
      yj = x[j-1]-0.3*x[0]*(x[0]*cos(24.0*PI*x[0]+4.0*j*PI/(double) mop->nvar)+2.0)*cos(6.0*PI*x[0]+j*PI/(double) mop->nvar);
      sum1 += yj*yj;
      count1++;
    }
  }

  f[0] = x[0] + 2.0 * sum1 / (double)count1;
  f[1] = 1.0 - sqrt(x[0]) + 2.0 * sum2 / (double)count2;
  mop->feval++;
}


/* Test problem UF3

   Real variables >= 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1]^n
   PFTrue is at f2 = 1 - sqrt(f1), f1 in [0, 1]
   xj = x1^(0.5*(1+3*(j-2) / (n-2))) j = 2,...,n 0 <= x1 <= 1
   Convex Pareto-optimal front
*/
void uf3(EMO_MOP *mop, double *f, double *x) {
  int j, count1, count2;
  double sum1, sum2, prod1, prod2, yj, pj;

  sum1   = sum2   = 0.0;
  count1 = count2 = 0;
  prod1  = prod2  = 1.0;

  for(j = 2; j <= mop->nvar; j++) {
    yj = x[j-1] - pow(x[0], 0.5 * (1.0 + 3.0 * (j - 2.0) / ((double) mop->nvar - 2.0)));
    pj = cos(20.0 * yj * PI / sqrt((double) j));

    if (j % 2 == 0) {
      sum2  += yj*yj;
      prod2 *= pj;
      count2++;
    } 
    else {
      sum1  += yj*yj;
      prod1 *= pj;
      count1++;
    }
  }

  f[0] = x[0] + 2.0 * (4.0 * sum1 - 2.0 * prod1 + 2.0) / ((double) count1);
  f[1] = 1.0 - sqrt(x[0]) + 2.0 * (4.0 * sum2 - 2.0 * prod2 + 2.0) / ((double) count2);
  mop->feval++;
}

void uf3Range(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = 0.0;
    mop->xmax[i] = 1.0;
  }
}

/* Test problem UF4

   Real variables >= 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1] x [-2,2]^(n-1)
   PFTrue is at f2 = 1 - f1^2, f1 in [0, 1]
                xj = sin(6*PI*x[0] + j * PI/n), j = 2,...,n 0 <= x1 <= 1
   Concave Pareto-optimal front
*/
void uf4(EMO_MOP *mop, double *f, double *x) {
  int j, count1, count2;
  double sum1, sum2, yj, hj;

  sum1   = sum2   = 0.0;
  count1 = count2 = 0;

  for(j = 2; j <= mop->nvar; j++) {
    yj = x[j-1] - sin(6.0 * PI * x[0] + j * PI / (double) mop->nvar);
    hj = fabs(yj) / (1.0 + exp(2.0 * fabs(yj)));

    if (j % 2 == 0) {
      sum2 += hj;
      count2++;
    } 
    else {
      sum1 += hj;
      count1++;
    }
  }

  f[0] = x[0] + 2.0 * sum1 / (double)count1;
  f[1] = 1.0 - x[0] * x[0] + 2.0 * sum2 / (double)count2;
  mop->feval++;
}

void uf4Range(EMO_MOP *mop) {
  int i;

  mop->xmin[0] = 0.0;
  mop->xmax[0] = 1.0;

  for(i = mop->nvar - 1; i > 0; i--) {
    mop->xmin[i] = -2.0;
    mop->xmax[i] = 2.0;
  }
}
 
/* Test problem UF5

   Real variables >= 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1] x [-1,1]^(n-1)
   PF has 2N+1 Pareto Optimal solutions: (i/(2N),1-i/(2N))
   Disconnected and linear Pareto-optimal front
*/
void uf5(EMO_MOP *mop, double *f, double *x) {
  int j, count1, count2;
  double sum1, sum2, yj, hj, N, E;

  sum1   = sum2   = 0.0;
  count1 = count2 = 0;
  N = 10.0; E = 0.1;

  for(j = 2; j <= mop->nvar; j++) {
    yj = x[j-1] - sin(6.0 * PI * x[0] + j * PI / (double) mop->nvar);
    hj = 2.0 * yj * yj - cos(4.0 * PI * yj) + 1.0;

    if (j % 2 == 0) {
      sum2  += hj;
      count2++;
    } 
    else {
      sum1  += hj;
      count1++;
    }
  }

  hj = (0.5 / N + E) * fabs(sin(2.0 * N * PI * x[0]));
  f[0] = x[0] + hj + 2.0 * sum1 / (double)count1;
  f[1] = 1.0 - x[0] + hj + 2.0 * sum2 / (double)count2;
  mop->feval++;
}

/* Test problem UF6

   Real variables >= 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1] x [-1,1]^(n-1)
   PFTrue is at f2 = 1 - f1, f1 in Union_{i=1}^{N} [(2i-1)/2N, 2i/2N]
   Disconnected Pareto-optimal front
*/
void uf6(EMO_MOP *mop, double *f, double *x) {
  int j, count1, count2;
  double sum1, sum2, prod1, prod2, yj, hj, pj, N, E;

  N = 2.0; E = 0.1;
  sum1   = sum2   = 0.0;
  count1 = count2 = 0;
  prod1  = prod2  = 1.0;

  for(j = 2; j <= mop->nvar; j++) {

    yj = x[j-1] - sin(6.0 * PI * x[0] + j * PI / (double) mop->nvar);
    pj = cos(20.0 * yj * PI / sqrt((double) j));

    if (j % 2 == 0) {
      sum2  += yj * yj;
      prod2 *= pj;
      count2++;
    }
    else {
      sum1  += yj * yj;
      prod1 *= pj;
      count1++;
    }
  }

  hj = 2.0 * (0.5 / N + E) * sin(2.0 * N * PI * x[0]);

  if(hj < 0.0) hj = 0.0;

  f[0] = x[0] + hj + 2.0 * (4.0 * sum1 - 2.0 * prod1 + 2.0) / (double)count1;
  f[1] = 1.0 - x[0] + hj + 2.0 * (4.0 * sum2 - 2.0 * prod2 + 2.0) / (double)count2;
  mop->feval++;
}

/* Test problem UF7

   Real variables >= 30
   Bin variables = 0
   Objectives = 2
   Constraints = 0
   Values of x are in [0,1] x [-1,1]^(n-1)
   PFTrue is at f2 = 1 - f1, f1 in [0,1]
                xj = sin(6*PI*x[0] + j*PI/n), j = 2,...,n, x1 in [0,1]
   Linear Pareto-optimal front
*/
void uf7(EMO_MOP *mop, double *f, double *x) {
  int j, count1, count2;
  double sum1, sum2, yj;

  sum1   = sum2   = 0.0;
  count1 = count2 = 0;

  for(j = 2; j <= mop->nvar; j++) {
    yj = x[j-1] - sin(6.0 * PI * x[0] + j * PI / (double) mop->nvar);

    if (j % 2 == 0) {
      sum2  += yj * yj;
      count2++;
    } 
    else {
      sum1  += yj * yj;
      count1++;
    }
  }

  yj = pow(x[0], 0.2);

  f[0] = yj + 2.0 * sum1 / (double) count1;
  f[1] = 1.0 - yj + 2.0 * sum2 / (double) count2;
  mop->feval++;
}

void ufaRange(EMO_MOP *mop) {
  int i;

  mop->xmin[0] = 0.0;
  mop->xmax[0] = 1.0;

  for(i = mop->nvar - 1; i > 0; i--) {
    mop->xmin[i] = -1.0;
    mop->xmax[i] = 1.0;
  }
}

/* Test problem UF8

   Real variables >= 30
   Bin variables = 0
   Objectives = 3
   Constraints = 0
   Values of x are in [0,1]^2 x [-2,2]^(n-2)
   PFTrue is at f1^2 + f2^2 + f3^2 = 1, 0 <= f1,f2,f3 <= 1
   xj = 2*x2*sin(2*PI*x1 + j*PI/n), j = 3,...,n
   Concave Pareto-optimal front
*/
void uf8(EMO_MOP *mop, double *f, double *x) {
  int j, count1, count2, count3;
  double sum1, sum2, sum3, yj;

  sum1   = sum2   = sum3   = 0.0;
  count1 = count2 = count3 = 0;

  for(j = 3; j <= mop->nvar; j++) {
    yj = x[j-1] - 2.0 * x[1] * sin(2.0 * PI * x[0] + j * PI / (double) mop->nvar);

    if(j % 3 == 1) {
      sum1  += yj * yj;
      count1++;
    }
    else if(j % 3 == 2) {
      sum2  += yj * yj;
      count2++;
    }
    else {
      sum3  += yj * yj;
      count3++;
    }
  }

  f[0] = cos(0.5*PI*x[0]) * cos(0.5*PI*x[1]) + 2.0 * sum1 / (double)count1;
  f[1] = cos(0.5*PI*x[0]) * sin(0.5*PI*x[1]) + 2.0 * sum2 / (double)count2;
  f[2] = sin(0.5*PI*x[0]) + 2.0 * sum3 / (double)count3;
  mop->feval++;
}

/* Test problem UF9

   Real variables >= 30
   Bin variables = 0
   Objectives = 3
   Constraints = 0
   Values of x are in [0,1]^2 x [-2,2]^(n-2)
   PFTrue has two parts. The first part is
      0 <= f3 <= 1, 0 <= f1 <= 1/4 (1 - f3), f2 = 1 - f1 - f3
   and the second one is
      0 <= f3 <= 1, 3/4 (1 - f3) <= f1 <= 1, f2 = 1 - f1 - f3
   The PS also has two disconnected parts:
      x1 in [0,0.25] Union [0.75,1], 0 <= x2 <= 1,
      xj = 2*x2*sin(2*PI*x1 + j*PI/n), j = 3,...,n
   Disconnected Pareto-optimal front
*/
void uf9(EMO_MOP *mop, double *f, double *x) {
  int j, count1, count2, count3;
  double sum1, sum2, sum3, yj, E;

  E = 0.1;
  sum1   = sum2   = sum3   = 0.0;
  count1 = count2 = count3 = 0;

  for(j = 3; j <= mop->nvar; j++) {
    yj = x[j-1] - 2.0 * x[1] * sin(2.0 * PI * x[0] + j * PI / (double) mop->nvar);

    if(j % 3 == 1) {
      sum1  += yj * yj;
      count1++;
    } 
    else if(j % 3 == 2) {
      sum2  += yj * yj;
      count2++;
    } 
    else {
      sum3  += yj * yj;
      count3++;
    }
  }

  yj = (1.0 + E) * (1.0 - 4.0 * (2.0 * x[0] - 1.0) * (2.0 * x[0] - 1.0));

  if(yj < 0.0) yj = 0.0;

  f[0] = 0.5 * (yj + 2 * x[0]) * x[1] + 2.0 * sum1 / (double)count1;
  f[1] = 0.5 * (yj - 2 * x[0] + 2.0) * x[1] + 2.0 * sum2 / (double)count2;
  f[2] = 1.0 - x[1] + 2.0 * sum3 / (double)count3;
  mop->feval++;
}

/* Test problem UF10

   Real variables >= 30
   Bin variables = 0
   Objectives = 3
   Constraints = 0
   Values of x are in [0,1]^2 x [-2,2]^(n-2)
   PFTrue is f1^2 + f2^2 + f3^2 = 1, 0 <= f1,f2,f3 <= 1
             xj = 2*x2*sin(2*PI*x1 + j*PI/n), j = 3,...,n
   Concave Pareto-optimal front
*/
void uf10(EMO_MOP *mop, double *f, double *x) {
  int j, count1, count2, count3;
  double sum1, sum2, sum3, yj, hj;

  sum1   = sum2   = sum3   = 0.0;
  count1 = count2 = count3 = 0;

  for(j = 3; j <= mop->nvar; j++) {
    yj = x[j-1] - 2.0 * x[1] * sin(2.0 * PI * x[0] + j * PI / (double) mop->nvar);
    hj = 4.0 * yj * yj - cos(8.0 * PI * yj) + 1.0;

    if(j % 3 == 1) {
      sum1  += hj;
      count1++;
    }
    else if(j % 3 == 2) {
      sum2  += hj;
      count2++;
    }
    else {
      sum3  += hj;
      count3++;
    }
  }

  f[0] = cos(0.5*PI*x[0]) * cos(0.5*PI*x[1]) + 2.0 * sum1 / (double)count1;
  f[1] = cos(0.5*PI*x[0]) * sin(0.5*PI*x[1]) + 2.0 * sum2 / (double)count2;
  f[2] = sin(0.5*PI*x[0]) + 2.0 * sum3 / (double) count3;
  mop->feval++;
}

void ufbRange(EMO_MOP *mop) {
  int i;

  mop->xmin[0] = mop->xmin[1] = 0.0;
  mop->xmax[0] = mop->xmax[1] = 1.0;

  for(i = mop->nvar - 1; i > 1; i--) {
    mop->xmin[i] = -2.0;
    mop->xmax[i] = 2.0;
  }
}

/* Test problem DTLZ1 (Deb-Thiele-Laumanns-Zitzler)

   Real variables = mop->nobj + k - 1, k = 5 
   Bin variables = 0
   Objectives = (scalable) 
   constraints = 0
   Values of xi in [0,1]
   PFTrue is linear, separable, multimodal
   POS is at x_{M}^{*} = 0, sum_{m=1}^{M} fm = 0.5
*/
void dtlz1(EMO_MOP *mop, double *f, double *x) {
  double g, v;
  int i, j, t, c;

  g = 0.0;
  c = mop->nvar - mop->nobj + 1;

  for(i = mop->nvar - c; i < mop->nvar; i++)
    g += pow(x[i] - 0.5, 2.0) - cos(20.0 * PI * (x[i] - 0.5));

  g = 100.0 * (c + g);
  v = 0.5 * (1.0 + g);

  for(i = 0; i < mop->nobj; i++) {
    f[i] = v;
    t = mop->nobj - i - 1; 

    for(j = 0; j < t; j++) 
      f[i] *= x[j];

    if(t < mop->nobj - 1) 
      f[i] *= (1.0 - x[t]);
  }
  mop->feval++;
}

/* Generate a random solution of the true Pareto front */
/*void dtlz1_sol(double *f, double *x){
  int i;

  for(i = mop->nobj - 2; i > -1; i--) 
    x[i] = EMO_rndreal(0.0,1.0);  

  for(i = mop->nobj - 1; i < mop->nvar; i++) 
    x[i] = 0.5;

  dtlz1(f, x);
}*/

/* Test problem DTLZ2 (Deb-Thiele-Laumanns-Zitzler)

   Real variables = NOBJ + k - 1, k = 10 
   Bin variables = 0
   Objectives = (scalable) 
   constraints = 0
   Values of xi in [0,1]
   PFTrue is concave
   POS is at xi = 0.5 for all xi \in x_{M}, sum_{m=1}^{M} fm^2 = 1 
*/
void dtlz2(EMO_MOP *mop, double *f, double *x) {
  double g, v;
  int i, j, t, c;

  g = 0.0;
  c = mop->nvar - mop->nobj + 1;

  for(i = mop->nvar - c; i < mop->nvar; i++)
    g += pow(x[i] - 0.5, 2.0);

  v = 1.0 + g;

  for(i = 0; i < mop->nobj; i++) {
    f[i] = v;
    t = mop->nobj - i - 1;

    for(j = 0; j < t; j++)
      f[i] *= cos(x[j] * PI * 0.5);

    if(t < mop->nobj - 1) 
      f[i] *= sin(x[t] * PI * 0.5);
  }
  mop->feval++;
}

/* Generate a random solution of the true Pareto front */
/*void dtlz2_sol(double *f, double *x){
  int i;

  for(i = mop->nobj - 2; i > -1; i--) 
    x[i] = EMO_rndreal(0.0,1.0);  

  for(i = mop->nobj - 1; i < mop->nvar; i++) 
    x[i] = 0.5;

  dtlz2(f, x);
}*/

/* Test problem DTLZ3 (Deb-Thiele-Laumanns-Zitzler)

   Real variables = NOBJ + k - 1, k = 10 
   Bin variables = 0
   Objectives = (scalable) 
   constraints = 0
   Values of xi in [0,1]
   PFTrue is concave and multimodal
   POS is at xi = 0.5 for all xi \in x_{M}, g* = 0, sum_{m=1}^{M} fm^2 = 1 
*/
void dtlz3(EMO_MOP *mop, double *f, double *x) {
  double g, v;
  int i, j, t, c;

  g = 0.0;
  c = mop->nvar - mop->nobj + 1;

  for(i = mop->nvar - c; i < mop->nvar; i++)
    g += pow(x[i] - 0.5, 2.0) - cos(20.0 * PI * (x[i] - 0.5));

  g = 100.0 * (c + g);
  v = 1.0 + g;

  for(i = 0; i < mop->nobj; i++) {
    f[i] = v;
    t = mop->nobj - i - 1; 

    for(j = 0; j < t; j++)
      f[i] *= cos(x[j] * PI * 0.5);

    if(t < mop->nobj - 1)
      f[i] *= sin(x[t] * PI * 0.5);
  }
  mop->feval++;
}

/* Generate a random solution of the true Pareto front */
/*void dtlz3_sol(double *f, double *x){
  int i;

  for(i = mop->nobj - 2; i > -1; i--) 
    x[i] = EMO_rndreal(0.0,1.0);  

  for(i = mop->nobj - 1; i < mop->nvar; i++) 
    x[i] = 0.5;

  dtlz3(f, x);
}*/

/* Test problem DTLZ4 (Deb-Thiele-Laumanns-Zitzler)

   Real variables = NOBJ + k - 1, k = 10 
   Bin variables = 0
   Objectives = (scalable) 
   constraints = 0
   Values of xi in [0,1]
   PFTrue is concave, separable, unimodal
   POS is at xi = 0.5 for all xi \in x_{M}, g* = 0, sum_{m=1}^{M} fm^2 = 1 
*/
void dtlz4(EMO_MOP *mop, double *f, double *x) {
  static double alpha = 100.0;
  double g, v;
  int i, j, t, c;

  g = 0.0;
  c = mop->nvar - mop->nobj + 1;

  for(i = mop->nvar - c; i < mop->nvar; i++)
    g += pow(x[i] - 0.5, 2.0);

  v = 1.0 + g;

  for(i = 0; i < mop->nobj; i++) {
    f[i] = v;
    t = mop->nobj - i - 1; 

    for(j = 0; j < t; j++) 
      f[i] *= cos(pow(x[j], alpha) * PI * 0.5);

    if(t < mop->nobj - 1) 
      f[i] *= sin(pow(x[t], alpha) * PI * 0.5);
  }
  mop->feval++;
}

/* Generate a random solution of the true Pareto front */
/*void dtlz4_sol(double *f, double *x){
  int i;

  for(i = mop->nobj - 2; i > -1; i--)
    if(EMO_flip(0.25))
      x[i] = EMO_rndreal(0.94,1.0);
    else
      x[i] = EMO_rndreal(0.98,1.0);

  for(i = mop->nobj - 1; i < mop->nvar; i++) 
    x[i] = 0.5;

  dtlz4(f, x);
}*/

/* Test problem DTLZ5 (Deb-Thiele-Laumanns-Zitzler)

   Real variables = NOBJ + k - 1, k = 10 
   Bin variables = 0
   Objectives = (scalable) 
   constraints = 0
   Values of xi in [0,1]
   PFTrue is unimodal, m < 4 degenerate
   POS is at xi = 0.5 for all xi \in x_{M}, g* = 0, sum_{m=1}^{M} fm^2 = 1 
*/
void dtlz5(EMO_MOP *mop, double *f, double *x) {
  double g, p, v;
  int i, j, t, c;

  g = 0.0;
  c = mop->nvar - mop->nobj + 1;

  for(i = mop->nvar - c; i < mop->nvar; i++)
    g += pow(x[i] - 0.5, 2.0);

  mop->t[0] = x[0] * PI * 0.5;
  p = PI / (4.0 * (1.0 + g));

  for(i = 1; i < mop->nobj-1; i++)
    mop->t[i] = p * (1.0 + 2.0 * g * x[i]);

  v = 1.0 + g;

  for(i = 0; i < mop->nobj; i++) {
    f[i] = v;
    t = mop->nobj - i - 1; 

    for(j = 0; j < t; j++) 
      f[i] *= cos(mop->t[j]);

    if(t < mop->nobj - 1) 
      f[i] *= sin(mop->t[t]);
  }
  mop->feval++;
}

/*void dtlz5_sol(double *f, double *x){
  int i;

  for(i = mop->nobj - 2; i > -1; i--)
    x[i] = EMO_rndreal(0.0,1.0);

  for(i = mop->nobj - 1; i < mop->nvar; i++)
    x[i] = 0.5;

  dtlz5(f, x);
}*/


/* Test problem DTLZ6 (Deb-Thiele-Laumanns-Zitzler)

   Real variables = NOBJ + k - 1, k = 10 
   Bin variables = 0
   Objectives = (scalable) 
   constraints = 0
   Values of xi in [0,1]
   PFTrue is unimodal, bias, many-to-one-mapping, m < 4 degenerate
   POS is at xi = 0 for all xi \in x_{M}, sum_{m=1}^{M} fm^2 = 1 
*/
void dtlz6(EMO_MOP *mop, double *f, double *x) {
  double g, p, v;
  int i, j, t, c;

  g = 0.0;
  c = mop->nvar - mop->nobj + 1;

  for(i = mop->nvar - c; i < mop->nvar; i++)
    g += pow(x[i], 0.1);

  mop->t[0] = x[0] * PI * 0.5;
  p = PI / (4.0 * (1.0 + g));

  for(i = 1; i < mop->nobj-1; i++)
    mop->t[i] = p * (1.0 + 2.0 * g * x[i]);

  v = 1.0 + g;

  for(i = 0; i < mop->nobj; i++) {
    f[i] = v;
    t = mop->nobj - i - 1; 

    for(j = 0; j < t; j++) 
      f[i] *= cos(mop->t[j]);

    if(t < mop->nobj - 1) 
      f[i] *= sin(mop->t[t]);
  }
  mop->feval++;
}

/*void dtlz6_sol(double *f, double *x){
  int i;

  for(i = mop->nobj - 2; i > -1; i--)
    x[i] = EMO_rndreal(0.0,1.0);

  for(i = mop->nobj - 1; i < mop->nvar; i++)
    x[i] = 0.0;

  dtlz6(f, x);
}*/

/* Test problem DTLZ7 (Deb-Thiele-Laumanns-Zitzler)

   Real variables = NOBJ + k - 1, k = 20 
   Bin variables = 0
   Objectives = (scalable) 
   constraints = 0
   Values of xi in [0,1]
   PFTrue is disconnected
   POS is at xM = 0, sum_{m=1}^{M} fm^2 = 1 
*/
void dtlz7(EMO_MOP *mop, double *f, double *x) {
  double g, h;
  int i, c;

  g = h = 0.0;
  c = mop->nvar - mop->nobj + 1;

  for(i = 0; i < mop->nobj - 1; i++) 
    f[i] = x[i];

  for(i = mop->nvar - c; i < mop->nvar; i++)
    g += x[i];

  g = 1.0 + 9.0 * g / (double) c;

  for(i = 0; i < mop->nobj - 1; i++)
    h += x[i] / (1.0 + g) * (1.0 + sin(3.0 * PI * x[i]));

  h = mop->nobj - h;

  f[mop->nobj-1] = (1.0 + g) * h;
  mop->feval++;
}

/*void dtlz7_sol(double *f, double *x){
  int i;

  for(i = mop->nobj - 2; i > -1; i--)
    x[i] = EMO_rndreal(0.0,1.0);

  for(i = mop->nobj - 1; i < mop->nvar; i++)
    x[i] = 0.0;

  dtlz7(f, x);
}*/

void dtlzRange(EMO_MOP *mop) {
  int i;

  for(i = mop->nvar - 1; i > -1; i--) {
    mop->xmin[i] = 0.0;
    mop->xmax[i] = 1.0;
  }
}

/* Looks for the name of the function in a dictionary

   apf: pointer to the test function
   nom: name of the function
   m:   number of objectives
   n:   number of variables
   k:   parameter k (only for WFG test suite)
   npos: position related parameters
   Returns the status of the search
 */

void EMO_Benchmark_alloc(EMO_MOP *mop, EMO_Param *param, const char *problem) {  
//void EMO_MOP_alloc_test(EMO_MOP *mop, EMO_Debug *dbg, const char *str, int nvar, int nobj, int ncon, int npos, const char *wfile, const char *pfile) {
  typedef void (*Range)(EMO_MOP *);
  int i, n, nvar, nobj, ncon, npos;

  /* Dictionary of test functions */
  const char *name[] = { "FON1", "FON2", "KUR", "LAU", "LIS", "MUR",
                         "POL", "QUA", "REN1", "REN2", "SCH1", "SCH2",
                         "VIE1", "VIE2", "VIE3", "DEB1", "DEB2", "DEB3",
                         "OKA1", "OKA2", "BNH1", "BNH3",
                         "SDD", "STZ1", "STZ2", "STZ3",
                         "ZDT1", "ZDT2", "ZDT3", "ZDT4", "ZDT5", "ZDT6",
                         "UF1", "UF2", "UF3", "UF4", "UF5",
                         "UF6", "UF7", "UF8", "UF9", "UF10",
                         "DTLZ1", "DTLZ2", "DTLZ3", "DTLZ4", "DTLZ5", "DTLZ6", "DTLZ7",
                         "WFG1", "WFG2", "WFG3", "WFG4", "WFG5", "WFG6",
                         "WFG7", "WFG8", "WFG9", 
                         //"KNAPSACK",
                          NULL};

  const EMO_MOP_evaluate fdic[] = {
                   fon1, fon2, kur, lau, lis, mur,
                   pol, qua, ren1, ren2, sch1, sch2,
                   vie1, vie2, vie3, deb1, deb2, deb3,
                   oka1, oka2, bnh1, bnh3,
                   sdd, stz1, stz2, stz3,
                   zdt1, zdt2, zdt3, zdt4, zdt5, zdt6,
                   uf1, uf2, uf3, uf4, uf5, 
                   uf6, uf7, uf8, uf9, uf10,
                   dtlz1, dtlz2, dtlz3, dtlz4, dtlz5, dtlz6, dtlz7,
                   wfg1, wfg2, wfg3, wfg4, wfg5, wfg6, 
                   wfg7, wfg8, wfg9}; //,
                   //knapsack };

  const Range range_dic[] = { /* Functions that specify the range of variables */
                   fon1Range, fon2Range, kurRange, lauRange, lisRange, murRange,
                   polRange, quaRange, renRange, renRange, sch1Range, sch2Range,
                   vie1Range, vie2Range, vie3Range, debRange, debRange, debRange,
                   oka1Range, oka2Range, bnh1Range, bnh3Range,
                   stzRange, stzRange, stzRange, stzRange,
                   zdtRange, zdtRange, zdtRange, zdt4Range, zdtRange, zdtRange,
                   ufaRange, ufaRange, uf3Range, uf4Range, ufaRange, 
                   ufaRange, ufaRange, ufbRange, ufbRange, ufbRange,
                   dtlzRange, dtlzRange, dtlzRange, dtlzRange, dtlzRange, dtlzRange, dtlzRange,
                   wfgRange, wfgRange, wfgRange, wfgRange, wfgRange, wfgRange, 
                   wfgRange, wfgRange, wfgRange };//,
                   //knapsackRange }; 

  const int obj[] = {  /* Number of objectives for each test function, 0 means scalable */
                   2, 2, 2, 2, 2, 2,
                   2, 2, 2, 2, 2, 2,
                   3, 3, 3, 2, 2, 2,
                   2, 2, 2, 3,
                   0, 2, 2, 2,
                   2, 2, 2, 2, 2, 2,       // zdt
                   2, 2, 2, 2, 2, 
                   2, 2, 3, 3, 3,
                   0, 0, 0, 0, 0, 0, 0,    // dtlz
                   0, 0, 0, 0, 0, 0,       // wfg
                   0, 0, 0,
                   0 };

  const int var[] = {  /* Number of variables: 0 means scalable, -1 means fixed */
                   2, 0, 3, 2, 2, 2,
                   2, 0, 2, 2, 1, 1,
                   2, 2, 2, 2, 2, 2,
                   2, 3, 2, 2,
                   0, 2, 0, 2,
                   0, 0, 0, 0, 0, 0,            // zdt
                   0, 0, 0, 0, 0, 
                   0, 0, 0, 0, 0,
                   -1, -1, -1, -1, -1, -1, -1,  // dtlz
                   0, 0, 0, 0, 0, 0,            // wfg
                   0, 0, 0,
                   0 };

  const int default_var[] = {          /* Default number of variables  */
                   0, 10, 0, 0, 0, 0,  /* (only for scalable problems): 0 means not applicable */
                   0, 16, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0,
                   10, 0, 10, 0,
                   30, 30, 30, 10, 11, 10,     // zdt
                   30, 30, 30, 30, 30, 
                   30, 30, 30, 30, 30,
                   5, 10, 10, 10, 10, 10, 20,  /* dtlz, k values for DTLZ, NVAR = NOBJ + k - 1 */
                   24, 24, 24, 24, 24, 24,     // wfg
                   24, 24, 24,
                   500 };


  /* Name of function is converted to uppercase */
  n = strlen(problem);
  mop->name = (char *) malloc(sizeof(char) * (n+1));

  for(i=0; i < n; i++)
    mop->name[i] = toupper(problem[i]);
  
  mop->name[n] = '\0';

  nvar = nobj = ncon = npos = 0;

  if(strncmp(problem, "WFG", 3) == 0) {
    if(!EMO_Parser_get_int(param->parser, &nvar, "nvar_wfg")) {
      printf("Error, nvar_wfg is not defined in the configuration file.\n");
      exit(1);
    }
  }
  else {
    if(!EMO_Parser_get_int(param->parser, &nvar, "nvar")) {
      printf("Error, nvar is not defined in the configuration file.\n");
      exit(1);
    }
  }

  if(!EMO_Parser_get_int(param->parser, &nobj, "nobj")) {
    printf("Error, nobj is not defined in the configuration file.\n");
    exit(1);
  }

  if(!EMO_Parser_get_int(param->parser, &npos, "npos")) {
    printf("Error, npos is not defined in the configuration file.\n");
    exit(1);
  }

  /*if(nvar == NULL || nobj == NULL || *nvar < 0 || *nobj < 0) {*/
  if(nvar < 0 || nobj < 0) {
    printf("Error, invalid reference to the number of variables or objectives.\n");
    free(mop->name);
    exit(1);
  }

/*  if(strncmp(mop->name, "WFG", 3) == 0) {
    if(npos == 0) {
      printf("Error, you should specify the position related parameters in {nobj-1, 2(nobj-1), 3(nobj-1),...}.\n");
      free(mop->name);
      return NULL;
    }
  }*/

  /* Find the function's name in dictionary */
  i = 0;

  while(name[i] != NULL) {

    if(strcmp(mop->name, name[i]) == 0) {
      mop->f = fdic[i];

      /* Default objective values */
      if(nobj == 0 || obj[i] > 0) {
        nobj = (obj[i] <= 0)? 3 : obj[i]; /* Scalable number of objectives */

        if(nobj != 0 && obj[i] > 0) /* Verify that is scalable w.r.t. the number of objectives */
          EMO_Debug_printf(param->dbg, "Benchmark: warning, function %s is not scalable with respect to the number of objectives, using the default value.", mop->name);
      }

      /* Default variable values */
      if(nvar == 0 || var[i] > 0) {
        nvar = (var[i] <= 0)? default_var[i] : var[i]; /* Scalable number of variables */

        if(strncmp(mop->name, "DTLZ", 4) == 0) /* Fixed number of variables in DTLZ test suite */
          nvar += nobj - 1;

        if(strncmp(mop->name, "WFG", 3) == 0) /* Fixed number of variables in WFG test suite */
          nvar = 24;

        if(nvar != 0 && var[i] > 0)  /* Verify that is scalable w.r.t. the number of variables */
          EMO_Debug_printf(param->dbg, "Benchmark: warning, function %s is not scalable with respect to the number of variables, using the default value: %d.", mop->name, nvar);
      }

      if(nvar != 0 && var[i] <= 0) {
        if(strncmp(mop->name, "DTLZ", 4) == 0) { /* Fixed number of variables in DTLZ test suite */
          nvar = default_var[i] + nobj - 1; 
          EMO_Debug_printf(param->dbg, "Benchmark: warning, fixed number of variables (%d) in %s test problem.", nvar, mop->name);
        }

        /* Number of position-related parameters in WFG test suite */
        if(strncmp(mop->name, "WFG", 3) == 0 && npos == 0) {
          npos = (nobj == 2)? 4 : 2 * nobj - 2;
          EMO_Debug_printf(param->dbg, "Benchmark: warning, number of position parameters in %s is not specified, using the default value %d.", mop->name, npos);
        }
      }

      /* Memory allocation of special functions */
      if(strcmp(mop->name, "DTLZ5") == 0 || strcmp(mop->name, "DTLZ6") == 0) {
        if((mop->t = (double *) malloc(sizeof(double) * (nobj-1))) == NULL) {
          printf("Not enough memory in benchmark.c\n");
          free(mop->name);
          exit(1);
        }
      }

      //if(strncmp(mop->name, "KNAPSACK", 8) == 0 || strncmp(mop->name, "ZDT5", 4) == 0)
      if(strncmp(mop->name, "ZDT5", 4) == 0)
        mop->coding = EMO_BINARY;
      else
        mop->coding = EMO_REAL;


      mop->nobj = nobj;
      mop->nvar = nvar;
      mop->ncon = ncon;  
      mop->npos = npos;  /* DTLZ k */

      mop->xmin = (double *) calloc(sizeof(double), nvar);
      mop->xmax = (double *) calloc(sizeof(double), nvar);
      range_dic[i](mop);
      mop->feval = 0;

      if(strncmp(mop->name, "WFG", 3) == 0) {
        if(mallocWFG(mop)) {
          free(mop->name);
          exit(1);
        }
      }

      EMO_Debug_printf(param->dbg, "Benchmark:test function %s", mop->name);
      EMO_Debug_printf(param->dbg, "Benchmark:variables: %d", nvar);
      EMO_Debug_printf(param->dbg, "Benchmark:objective functions: %d", nobj);
      EMO_Debug_printf(param->dbg, "Benchmark:constraints: %d", ncon);
      EMO_Debug_printf(param->dbg, "Benchmark:position parameters: %d", npos);
      EMO_Debug_printf(param->dbg, "Benchmark:encoding: %d", mop->coding);
      EMO_Debug_printv(param->dbg, mop->xmin, mop->nvar, "Benchmark:lower bound");
      EMO_Debug_printv(param->dbg, mop->xmax, mop->nvar, "Benchmark:upper bound");

      /*free(mop->name); */
      return;
    }
    i++;
  }

  printf("Error, unknown function %s.\n", mop->name);

  free(mop->name);
  exit(1);
  /*return NULL;*/
}


/* Free memory */
void EMO_Benchmark_free(EMO_MOP *mop) {
 
  if(strncmp(mop->name, "WFG", 3) == 0) 
    freeWFG(mop);

  if(strncmp(mop->name, "DTLZ5", 5) == 0 || strncmp(mop->name, "DTLZ6", 5) == 0) 
    free(mop->t);

  //if(strncmp(mop->name, "KNAPSACK", 8) == 0) 
  //  freeKnapsack(&knap);

  free(mop->xmin);
  free(mop->xmax);
  free(mop->name);
}

