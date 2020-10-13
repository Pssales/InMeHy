#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "sort.h"

double contribution(double *obj, int p, double *ref, double **sort, int dim, int size) {
  int i, j, k;

  // m n log n
  for(i = 0; i < dim; i++) {
    // ordenar poblacion con respecto a los objetivos
    for(j = 0; j < size; j++) {
      sort[i][0] = (double) i; 
      sort[i][1] = obj[i*dim + j];
    }

    qsort(sort, size, sizeof(sort[0]), (int (*)(const void *, const void *))&EMO_compare_asc);

    for(j = 0; j < size; j++) {
      k = (int) sort[j][0];
      printf("individuo %d:%d ", i, k);
      vprint(stdout, obj + k, dim, "obj");
    }

    // Localiza individuo p
    for(j = 0; j < size; j++) {
      if((int) sort[j][0] == p) {
        printf("vecinos de %d ", p);

        if(j > 1) printf("%d ", sort[j-1][0]);
        if(j < size - 1) printf("%d ", sort[j+1][0]);
        break;
      }
    }
  }
}

