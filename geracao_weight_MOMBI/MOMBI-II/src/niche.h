
#ifndef _NICHE_
#define _NICHE_

#include "list.h"

void EMO_nicheCount(double *niche, double *data, int row, int col, double alpha, double sigma_share);
void EMO_crowdingDistance(double *cd, double **sort, double *data, int row, double *min, double *max, int col);
void EMO_crowdingDistance2(double *cd, double **sort, double *data, EMO_List *front, double *max, double *min, int col);
void EMO_wdist(double *wd, double *data, int size, double *W, int wsize, double *tmp, int dim);


// Metodo de truncado de soluciones basado en SPEA2
typedef struct {
  EMO_List *lnn;          /* array of lists for storing neighbors */
  EMO_List copy;
  double **sort;          /* temporary array for sorting */
  double **dist;
  int *min, *max;         /* individuals that are part of the extreme points */
  int dim, ssize;
} EMO_KNN;

void EMO_KNN_alloc(EMO_KNN *knn, int size, int dim);
void EMO_KNN_free(EMO_KNN *knn);
void EMO_KNN_prune(EMO_KNN *knn, int *filter, int max_size, double *data, int size);

void EMO_knn(EMO_List *l, double **sort, double *W, int size, int dim, int k, int itself);
void EMO_knn2(EMO_List *l, EMO_List *copy, double **dist, double **sort, double *W, int size, int dim);
void EMO_kNN2(EMO_List *lst, double **sort, double *data, int row, int col);

#endif


