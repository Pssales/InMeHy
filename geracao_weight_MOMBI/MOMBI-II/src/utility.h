/**************************************************************
 * utility.c    Definition of utility functions.              *
 *                                                            *
 * March 2013                                                 *
 *************************************************************/
#ifndef _UTILITY_H
#define _UTILITY_H

typedef struct EMO_Utility {
  char *name;
  int nobj;
  double *tmp; // pbi parameter
  double (*uf)(struct EMO_Utility *u, double *w, double *x);
} EMO_Utility;

typedef double (*EMO_UtilityFunction)(EMO_Utility *u, double *w, double *x);

void EMO_Utility_alloc(EMO_Utility *u, int nobj, const char *str);
void EMO_Utility_free(EMO_Utility *u);
double EMO_Utility_asf(EMO_Utility *u, double *w, double *x);
double EMO_Utility_weighted_sum(EMO_Utility *u, double *w, double *x);
double EMO_Utility_least_squares(EMO_Utility *u, double *w, double *x);
double EMO_Utility_tchebycheff(EMO_Utility *u, double *w, double *x);
double EMO_Utility_aug_tchebycheff(EMO_Utility *u, double *w, double *x);
double EMO_Utility_mod_tchebycheff(EMO_Utility *u, double *w, double *x);
double EMO_Utility_pbi(EMO_Utility *u, double *w, double *x);

#endif

