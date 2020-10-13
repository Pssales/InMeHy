
#ifndef _DOMINANCE_
#define _DOMINANCE_

#include "list.h"

typedef int (*EMO_Dominance)(double *, double *, int);

int EMO_Dominance_weak(double *x, double *y, int n);
int EMO_Dominance_strict(double *x, double *y, int n);
int EMO_Dominance_strong(double *x, double *y, int n);
int EMO_Dominance_alpha(double *x, double *y, double *tmp, double *alpha, int n);

int EMO_Dominance_incomparable(double *x, double *y, int n, EMO_Dominance r);
int EMO_Dominance_indifferent(double *x, double *y, int n);
int EMO_Dominance_constraint(double *x, double *y, int n, double *gx, double *gy, int k, EMO_Dominance r);
int EMO_Dominance_constraint2(double *x, double *y, int n, double *gx, double *gy, int k, EMO_Dominance r);
int EMO_Dominance_feasible(double *g, int k);
int EMO_Dominance_ndset(int *nd, double *data, int row, int col, EMO_Dominance r);

typedef struct {
  int size;         // numero de soluciones a evaluar
  int *n;           // auxiliar, numero de veces que una solucion i ha sido dominada
  int *rank;        // jerarquia de los individuos
  int nfront;       // numero de frentes
  EMO_List *S;      // auxiliar, lista de soluciones que domina el i-esimo individuo
  EMO_List *front;  // Lista de frentes
} EMO_NDSort;

void EMO_NDSort_alloc(EMO_NDSort *nd, int max_size);
void EMO_NDSort_free(EMO_NDSort *nd);
void EMO_NDSort_run(EMO_NDSort *nd, double *obj, int *filter, int size, int nobj);
void EMO_NDSort_run2(EMO_NDSort *nd, double *obj, int *filter, double *tmp, double *alpha, int size, int nobj);

void EMO_Dominance_ranking_sum(double *rank, double **sort, double *data, int size, int nobj);
void EMO_Dominance_ranking_min(double *rank, double **sort, double *data, int size, int nobj);

#endif

