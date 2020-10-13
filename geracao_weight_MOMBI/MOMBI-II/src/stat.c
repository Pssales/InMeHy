
#include <math.h>
#include "stat.h"

double EMO_mean(double *data, int size) {
  double v = 0;
  int i;

  for(i = 0; i < size; i++)
    v += data[i];

  return v / size;
}

// mu, mean
double EMO_var(double *data, double mu, int size) {
  double v = 0;
  int i;

  for(i = 0; i < size; i++)
    v += pow(data[i] - mu, 2.0);

  return v / size; 
}

double EMO_std(double *data, double mu, int size) {
  return sqrt(EMO_var(data, mu, size));
}

