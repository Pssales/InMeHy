
#ifndef _MOMBI_
#define _MOMBI_

#include "random.h"
#include "utility.h"
#include "common.h"
#include "debug.h"
#include "param.h"
#include "stop.h"
#include "list.h"
#include "plot.h"

typedef struct {
  EMO_List lst1, lst2; // temporary lists
  double *norm;        // normalized objective functions
  double *rank;        // rank of individuals
  double *l2;          // l2 norm
  double **sort;       // temporary array for sorting population
  int ssize;           // temporal population size (for memory free)
  double *tmp;         // temporary array
  int wsize;           // number of weight vectors
  double *W;           // weight vectors
  double *min;         // ideal point
  double *max;         // nadir point
  double *ideal;       // ideal point
  double *new_min;
  double *new_max;
  double *hist;
  int *update;
  EMO_Utility utl;
  int *filter;         // selected individuals
  int max_hist;        /* parameters of the algorithm */
  double epsilon;
  double alpha;
  char *wfile;
} EMO_MOMBI2;

void EMO_MOMBI2_alloc(EMO_MOMBI2 *alg, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop);
void EMO_MOMBI2_free(EMO_MOMBI2 *alg);
void EMO_MOMBI2_run(EMO_MOMBI2 *alg, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop);
void EMO_MOMBI2_prun(EMO_MOMBI2 *alg, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop);

#endif

