/**************************************************************
 * utility.c    Definition of utility functions.              *
 *                                                            *
 * March 2013                                                 *
 *************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

#include "utility.h"

void EMO_Utility_alloc(EMO_Utility *u, int nobj, const char *str) {

  const char *name[] = { "asf",
                         "weighted_sum",
                         "least_square",
                         "tchebycheff",
                         "aug_tchebycheff",
                         "mod_tchebycheff",
                         "pbi", NULL };

  const EMO_UtilityFunction fdic[] = { EMO_Utility_asf,
                                       EMO_Utility_weighted_sum,
                                       EMO_Utility_least_squares,
                                       EMO_Utility_tchebycheff,
                                       EMO_Utility_aug_tchebycheff,
                                       EMO_Utility_mod_tchebycheff,
                                       EMO_Utility_pbi };

  int i, n;

  u->nobj = nobj;

  /* Name of function is converted to lower case */
  n = strlen(str);
  u->name = (char *) malloc(sizeof(char) * (n+1));

  for(i=0; i < n; i++)
    u->name[i] = tolower(str[i]);

  u->name[n] = '\0';

  /* Find the function's name in dictionary */
  i = 0;

  while(name[i] != NULL) {
    if(strcmp(u->name, name[i]) == 0) {
      u->uf = fdic[i];

      if(strncmp(u->name, "pbi", 3) == 0) {
        if((u->tmp = (double *) malloc(sizeof(double) * nobj)) == NULL) {
          printf("Not enough memory in utility.c\n");
          free(u->name);
          exit(1);
        }
      }
      return;
    }
    i++;
  }

  printf("Error, unknown function %s.\n", u->name);
  free(u->name);
  exit(1);
}

void EMO_Utility_free(EMO_Utility *u) {
  if(strncmp(u->name, "pbi", 3) == 0)
    free(u->tmp);
 
  free(u->name);
}


/* Achievement scalarizing function */
double EMO_Utility_asf(EMO_Utility *u, double *w, double *x) {
  double y, v, vmax = 0;
  int i;

  for(i = 0; i < u->nobj; i++) {

    y = w[i] ? w[i] : 1e-2; //1e-3;dtlz //1e-4;   // 1e-2, wfg4, diferente no funciona
    v = fabs(x[i]) / y;

    if(v > vmax)
      vmax = v;
  }
  return vmax;
}

double EMO_Utility_weighted_sum(EMO_Utility *u, double *w, double *x) {
  double t = 0;
  int i;

  for(i = 0; i < u->nobj; i++) 
    t += w[i] * x[i];
  return t;
}

double EMO_Utility_least_squares(EMO_Utility *u, double *w, double *x) {
  double t = 0;
  int i;

  for(i = 0; i < u->nobj; i++) {
    t += w[i] * x[i] * x[i];
  }
  return sqrt(t);
}

double EMO_Utility_tchebycheff(EMO_Utility *u, double *w, double *x) {
  double y, v, vmax = -DBL_MAX;
  int i;

  for(i = 0; i < u->nobj; i++) {
    y = w[i] ? w[i] : 1e-6; 
    v = fabs(x[i]) * y;

    if(v > vmax)
      vmax = v;
  }
  return vmax;
}

double EMO_Utility_aug_tchebycheff(EMO_Utility *u, double *w, double *x) {
  double t, s, vmax;
  int i;

  s = vmax = 0;

  for(i = 0; i < u->nobj; i++) {
    t = w[i] * x[i];  
    s += x[i]; 
 
    if(t > vmax)
      vmax = t;
  }

  return vmax + 0.01 * s;
}

double EMO_Utility_mod_tchebycheff(EMO_Utility *u, double *w, double *x) {
  double t, s, vmax;
  int i, j;

  vmax = 0;
  s = 0;

  for(j = 0; j < u->nobj; j++)
    s += x[j];

  for(i = 0; i < u->nobj; i++) {
    t = w[i] * (x[i] + 0.01 * s);

    if(t > vmax)
      vmax = t;
  }

  return vmax;
}

double EMO_Utility_pbi(EMO_Utility *u, double *w, double *x) {
  double norm = 0, d1 = 0, d2 = 0;
  int i;
 
  // Normalize the weight vector (line segment)
  for(i = 0; i < u->nobj; i++)
    norm  += w[i] * w[i]; 
  norm = sqrt(norm);
 
  for(i = 0; i < u->nobj; i++)
    d1 += x[i] * w[i] / norm;
 
  d1 = fabs(d1);
 
  for(i = 0; i < u->nobj; i++)
    u->tmp[i] = x[i] - d1 * w[i] / norm;
 
  d2 = 0;
 
  // Normalize b
  for(i = 0; i < u->nobj; i++)
    d2  += u->tmp[i] * u->tmp[i]; 

  d2 = sqrt(d2);
 
  return d1 + 5.0 * d2;
}

