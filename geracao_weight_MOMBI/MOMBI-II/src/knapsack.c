// from PISA: variator_user.c

#include <stdlib.h>
#include <stdio.h>

#include "random.h"
#include "knapsack.h"
#include "io.h"


/* function used for sorting items in decreasing order */
int cmpItems(const void*  itemPtr1, const void*  itemPtr2) {
  if(_profitWeightRatios[*((int*) itemPtr1)] >
    _profitWeightRatios[*((int*) itemPtr2)])  return -1;
  if(_profitWeightRatios[*((int*) itemPtr2)] > 
    _profitWeightRatios[*((int*) itemPtr1)])  return 1;
  return 0;
}

int mallocKnapsack(EMO_Knapsack *knap, int nvar, int nobj, const char *wfile, const char *pfile) {
  double weightSum, max, temp;
  int i, j;

  _length = nvar;
  _dimension = nobj;

  if((_weights = (double *) malloc(_length * _dimension * sizeof(double))) == NULL) {
    printf("Not enough memory in knapsack.c\n");
    return 1;
  }

  if((_profits =  (double *) malloc(_length * _dimension * sizeof(double))) == NULL) {
    printf("Not enough memory in knapsack.c\n");
    return 1;
   }

  if((_profitSums = (double *) malloc(_dimension * sizeof(double))) == NULL) {
    printf("Not enough memory in knapsack.c\n");
    return 1;
   }

  if((_capacities = (double *) malloc(_dimension * sizeof(double))) == NULL) {
    printf("Not enough memory in knapsack.c\n");
    return 1;
   }

  if((_selectOrder = (int *) malloc(_length * sizeof(int))) == NULL) {
    printf("Not enough memory in knapsack.c\n");
    return 1;
  }

  if(wfile == NULL) {
    for(i = 0; i < _dimension; i++)
      for(j = 0; j < _length; j++)
        // Profits and weights are random integers in the interval [10, 100]
        _weights[i * _length + j] = (double) EMO_Rand_int1(knap->rand, 10, 100);
  }
  else {
    _weights = EMO_File_read(NULL, &_dimension, &_length, wfile, 0);
  }

  if(pfile == NULL) {
    for(i = 0; i < _dimension; i++)
      for(j = 0; j < _length; j++)
        _profits[i * _length + j] = (double) EMO_Rand_int1(knap->rand, 10, 100);
  }
  else {
    _profits = EMO_File_read(NULL, &_dimension, &_length, pfile, 0);
  }
 
  for(i = 0; i < _dimension; i++) {
    weightSum = 0;

    for(j = 0; j < _length; j++) {
      weightSum += _weights[i * _length + j];
    }
    _capacities[i] = 0.5 * weightSum;
  }

 /* determine item order regarding max{profit[i]/weight[i]}
   (per item over all knapsacks) using quicksort
  */
  _profitWeightRatios = (double *) malloc(_length * sizeof(double));

  for(i = 0; i < _length; i++) {
    max = _profits[i] / _weights[i];

    for(j = 1; j < _dimension; j++) {
      temp = _profits[j * _length + i] / _weights[j * _length + i];

      if (temp > max)
        max = temp;
    }
    _profitWeightRatios[i] = max;
    _selectOrder[i] = i;
  }

  qsort(_selectOrder, _length, sizeof(int), cmpItems);
  free(_profitWeightRatios);
     
  /* calculate total of profits per objective */
  for(i = 0; i < _dimension; i++) {
    _profitSums[i] = 0;

    for(j = 0; j < _length; j++)
      _profitSums[i] += _profits[i * _length + j];
  }
  return 0;
}

void freeKnapsack(EMO_Knapsack *knap) {
  free(_weights);
  free(_profits);
  free(_profitSums);
  free(_capacities);
  free(_selectOrder);
}

void knapsackRange(double *xmin, double *xmax) {
  int i;

  for(i = 0; i < _length; i++) {
    xmin[i] = 0.0;
    xmax[i] = 1.0;
  }
}

/* Determines the objective value based on KNAPSACK.
   In order to maximize the profits this function is minimized.
   PISA always minimizes. */
/* knapsack problem
   calculate profits - abuse 'objectives' array for storing the used
   capacity per knapsack
*/
void knapsack(double *f, double *x, ...) {
  int i, j, k, index;
    
  for(i = 0; i < _dimension; i++)
    f[i] = 0;

  for(i = 0; i < _length; i++) {
    index = _selectOrder[i];

    if(x[index] == 1) {
      /* check whether item fits into knapsacks */
      for(j = 0; j < _dimension; j++)
        if(_weights[j * _length + index] + f[j] > _capacities[j]) 
          break;

      if(j == _dimension) { /* put item into knapsacks */
        for(k = 0; k < _dimension; k++)
          f[k] += _weights[k * _length + index];
      }
      else /* item too big -> do not examine remaining items */
        break;
    }
  }

  /* store profits actually achieved in 'f' array */
  for(j = 0; j < _dimension; j++)
    f[j] = 0;
    
  while (i > 0) {
    i--;
    index = _selectOrder[i];

    if(x[index] == 1)
      for(j = 0; j < _dimension; j++)
        f[j] += _profits[j * _length + index];
  }

  /* for minimization */
  for(j = 0; j < _dimension; j++)
    f[j] = _profitSums[j] - f[j];

}

