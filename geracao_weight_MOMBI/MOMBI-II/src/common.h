
#ifndef _COMMON_H
#define _COMMON_H

#include "random.h"
#include "list.h"

#define EMO_BINARY 0
#define EMO_REAL 1


typedef struct EMO_MOP {
  int nvar;            /* number of decision variables  */
  int nobj;            /* number of objective functions */
  int ncon;            /* number of constraints */

  int npos;            /* number of position related parameters (WFG test problems) */

  double *xmin, *xmax; /* valid ranges of decision variables */

  void (*f)(struct EMO_MOP *mop, double *, double *);  /* pointer to the evaluation function */

  char *name;          /* name of the problem */
  int coding;          /* representation: real or binary */
  int L;               /* chromosome length (binary representation) */
  unsigned long feval; /* number of objective function evaluations */

                       /* Auxiliary variables */
  double *t;  /* theta for DTLZ5, DTLZ6 and transition vector for WFG test problems */
  double *x;           /* underlying parameters */
  double *h;           /* shape */
  double *S;           /* constant */
  double *A;           /* constant */
  double *y;           /* temporary vector */
  double *w;           /* weight vector */
} EMO_MOP;

typedef void (*EMO_MOP_evaluate)(EMO_MOP *mop, double *, double *);

typedef struct {
  double *var;
  double *obj;
  double *con;
  int mu;     /* parent population size */
  int lambda; /* offspring population size */
  int size;  /* total population size */
  double *vdummy, *odummy, *cdummy; // Internal use
} EMO_Population;

void EMO_Population_alloc(EMO_Population *pop, EMO_MOP *mop, int mu, int lambda);
void EMO_Population_init(EMO_Population *pop, EMO_Rand *rand, EMO_MOP *mop);
int EMO_Population_init_from_file(EMO_Population *pop, EMO_MOP *mop, const char *file, int start);
void EMO_Population_write(EMO_Population *pop, int *filter, EMO_MOP *mop, const char *prefix, int run);
void EMO_Population_free(EMO_Population *pop);
void EMO_Population_copy(EMO_Population *pop, int *fit1, double *fit2, EMO_MOP *mop, int i, int j);
void EMO_Population_swap(EMO_Population *pop, int *fit1, double *fit2, EMO_MOP *mop, int i, int j);

void EMO_Population_survive(EMO_Population *pop, int *fit1, double *fit2, EMO_MOP *mop, EMO_List *missing, EMO_List *available, int *filter);

#endif

