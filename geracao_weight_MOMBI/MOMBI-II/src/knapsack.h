
#ifndef _KNAPSACK_
#define _KNAPSACK_

typedef struct {
  EMO_Rand *rand;
} EMO_Knapsack;

double *_weights;
double *_profits;
int _length;    /* Number of variables  */
int _dimension; /* Number of objectives */
int *_selectOrder;
double *_capacities;
double *_profitSums;

/* temporary array used for sorting items */
static double *_profitWeightRatios;

int cmpItems(const void*  itemPtr1, const void*  itemPtr2);
int mallocKnapsack(EMO_Knapsack *knap, int nvar, int nobj, const char *wfile, const char *pfile);
void freeKnapsack();
void knapsackRange(double *xmin, double *xmax);
void knapsack(double *f, double *x, ...);

#endif

