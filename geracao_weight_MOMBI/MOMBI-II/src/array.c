
#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

void f(double *a, int s) {
  int i;

  for(i = 0; i < s; i++)
    a[i] = (double) i + 1.0;
}

void print(double *a, int s) {
  int i;

  for(i = 0; i < s; i++)
    printf("%f ", a[i]);

  printf("\n");
}


void main() {
  double *a;

  a = (double *) malloc(sizeof(double) * SIZE);

  f(a, SIZE);
  print(a, SIZE);
 
  free(a);
}

