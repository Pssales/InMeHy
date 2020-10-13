
#include <stdlib.h>
#include <float.h>
#include <limits.h>

#include "numeric.h"
#include "vector.h"
#include "matrix.h"
#include "sort.h"

/* finds the minimum element of an array of doubles */
double EMO_dmin(int *idx, double *arr, int *filter, int size) {
  double min;
  int i, j;

  min = arr[0];
  j = 0;

  if(filter == NULL) {
    for(i = 1; i < size; i++) {
      if(arr[i] <= min) {
        min = arr[i];
        j = i;
      }
    }
  }
  else {
    for(i = 1; i < size; i++) {
      if(filter[i] && arr[i] <= min) {
        min = arr[i];
        j = i;
      }
    }
  }

  if(idx != NULL)
    *idx = j;

  return min;
}

/* finds the maximum element of an array of doubles */
double EMO_dmax(int *idx, double *arr, int *filter, int size) {
  double max;
  int i, j;

  max = -DBL_MAX;
  j = -1;

  if(filter == NULL) {
    for(i = 0; i < size; i++) {
      if(arr[i] >= max) {
        max = arr[i];
        j = i;
      }
    }
  }
  else {
    for(i = 0; i < size; i++) {
      if(filter[i] && arr[i] >= max) {
        max = arr[i];
        j = i;
      }
    }
  }

  if(idx != NULL)
    *idx = j;

  return max;
}

/* finds the minimum element of an array of integers */
int EMO_min(int *idx, int *arr, int *filter, int size) {
  int i, j, min;

  min = INT_MAX;
  j = -1;

  if(filter == NULL) {
    for(i = 0; i < size; i++) {
      if(arr[i] <= min) {
        min = arr[i];
        j = i;
      }
    }
  }
  else {
    for(i = 0; i < size; i++) {
      if(filter[i] && arr[i] <= min) {
        min = arr[i];
        j = i;
      }
    }
  }

  if(idx != NULL)
    *idx = j;

  return min;
}

/* finds the maximum element of an array of integers */
int EMO_max(int *idx, int *arr, int *filter, int size) {
  int i, j, max;

  max = -INT_MAX;
  j = -1;

  if(filter == NULL) {
    for(i = 0; i < size; i++) {
      if(arr[i] >= max) {
        max = arr[i];
        j = i;
      }
    }
  }
  else {
    for(i = 0; i < size; i++) {
      if(filter[i] && arr[i] >= max) {
        max = arr[i];
        j = i;
      }
    }
  }

  if(idx != NULL)
    *idx = j;

  return max;
}

/* determines the maximum bound of a set of vectors */
void EMO_maxBound(double *max, double *data, int *filter, int row, int col) {
  int i, j;

  for(j = 0; j < col; j++)
    max[j] = -DBL_MAX;

  if(filter == NULL) {
    for(i = 0; i < row; i++) {
      for(j = 0; j < col; j++) {

        if(data[i*col + j] > max[j])
          max[j] = data[i*col + j];
      }
    }
  }
  else {
    for(i = 0; i < row; i++) {
      for(j = 0; j < col; j++) {

        if(filter[i] && data[i*col + j] > max[j])
          max[j] = data[i*col + j];
      }
    }
  }
}

/* determines the minimum bound of a set of vectors */
void EMO_minBound(double *min, double *data, int *filter, int row, int col) {
  int i, j;

  for(j = 0; j < col; j++)
    min[j] = DBL_MAX;

  if(filter == NULL) {
    for(i = 0; i < row; i++) {
      for(j = 0; j < col; j++) {

        if(data[i*col + j] < min[j])
          min[j] = data[i*col + j];
      }
    }
  }
  else {
    for(i = 0; i < row; i++) {
      for(j = 0; j < col; j++) {

        if(filter[i] && data[i*col + j] < min[j])
          min[j] = data[i*col + j];
      }
    }
  }
}

/* determines the maximum and minimum bounds of a set of vectors */
void EMO_maxminBound(double *max, double *min, double *data, int *filter, int row, int col) {
  int i, j;

  for(j = 0; j < col; j++) {
    min[j] = DBL_MAX;
    max[j] = -DBL_MAX;
  }

  if(filter == NULL) {
    for(i = 0; i < row; i++) {
      for(j = 0; j < col; j++) {

        if(data[i*col + j] < min[j])
          min[j] = data[i*col + j];
 
        if(data[i*col + j] > max[j])
          max[j] = data[i*col + j];
      }
    }
  }
  else {
    for(i = 0; i < row; i++) {
      for(j = 0; j < col; j++) {

        if(filter[i]) {
          if(data[i*col + j] < min[j])
            min[j] = data[i*col + j];
 
          if(data[i*col + j] > max[j])
            max[j] = data[i*col + j];
        }
      }
    }
  }
}


/* finds those solutions that have a maximum or minimum bounds of a set of vectors */
void EMO_findmaxminBound(int *max, int *min, double *data, int *filter, int row, int col) {
  int i, j;

  i = 0;

  if(filter != NULL)
    while(i < row && filter[i] == 0) i++;

  for(j = 0; j < col; j++) {
    min[j] = i;
    max[j] = i;
  }

  if(filter == NULL) {
    for(i=i+1; i < row; i++) {
      for(j = 0; j < col; j++) {

        if(data[i*col + j] < data[min[j]*col + j] && EMO_find(min, col, i) == 0)
          min[j] = i;
        
 
        if(data[i*col + j] > data[max[j]*col + j] && EMO_find(max, col, i) == 0)
          max[j] = i;
      }
    }
  }
  else {
    for(i=i+1; i < row; i++) {
      for(j = 0; j < col; j++) {

        if(filter[i]) {
          if(data[i*col + j] < data[min[j]*col + j] && EMO_find(min, col, i) == 0)
            min[j] = i;
 
          if(data[i*col + j] > data[max[j]*col + j] && EMO_find(min, col, i) == 0)
            max[j] = i;
        }
      }
    }
  }
}

/* normalizes a set of vectors */
void EMO_normalize(double *norm, double *data, int *filter, int size, double *min, double *max, int dim) {
  int i, j, k;
  double v;

  if(filter == NULL) {
    for(i = 0; i < size; i++) {
      for(j = 0; j < dim; j++) {
        k = i * dim + j;
        v = max[j] - min[j];
        norm[k] = (v == 0.0)? data[k] : (data[k] - min[j]) / v;
      }
    }
  }
  else {
    for(i = 0; i < size; i++) {
      for(j = 0; j < dim; j++) {
        if(filter[i]) {
          k = i * dim + j;
          v = max[j] - min[j];
          norm[k] = (v == 0.0)? data[k] : (data[k] - min[j]) / v;
        }
      }
    }
  }
}

// least squares, linear interpolation
void EMO_lsq(double *a, double *tmp1, double *tmp2, double *y, int n) {
  double s1, s2;
  int i;

  tmp1[0] = n * (n + 1.0) * (2 * n + 1) / 6.0;
  tmp1[1] = tmp1[2] = n * (n + 1.0) / 2.0;
  tmp1[3] = (double) n;

  EMO_minverse(tmp2, tmp1, 2, 1, 0, 0);

  s1 = s2 = 0;

  for(i = 0; i < n; i++) {
    s1 += (double) (i + 1) * y[i];
    s2 += y[i];
  }

  tmp1[0] = s1;
  tmp1[1] = s2;

  EMO_matmul(a, tmp2, tmp1, 2, 2, 1); 
}

int EMO_find(int *data, int size, int elem) {
  int i;

  for(i = 0; i < size; i++)
    if(data[i] == elem)
      return 1;

  return 0;
}

