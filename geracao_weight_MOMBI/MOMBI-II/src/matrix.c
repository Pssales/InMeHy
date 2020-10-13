#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include "sort.h"

#define EPSM 10e-3  /* Tolerancia de error */
#define NT  1024   /* Numero de hilos por bloque en GPU's */

int debug = 0;

/*  Imprime matriz A[n x m] en un archivo */
void EMO_mprint(FILE *fp, const char *s, double *A, int n, int m) {
  int i, j;

  if(s != NULL) fprintf(fp, "\n%s [%d x %d] =\n", s, n, n);

  for(i = 0; i < n; i++) {
    fprintf(fp, "\n  ");

    for(j = 0; j < m; j++)
      fprintf(fp, "%+.5f ", A[i*m+j]);
  }
  fprintf(fp, "\n");
}

/*  Multiplicacion de matrices A[n x l] = B[n x m] C[m x l]  */
void EMO_matmul(double *a, double *b, double *c, int n, int m, int l) {
  int i, j, k;
  double s;

  //#pragma omp parallel for shared(a,b,c) private(i,j,k,s)
  for(i = 0; i < n; i++) {
    for(j = 0; j < l; j++) {
      s = 0;

      for(k = 0; k < m; k++)
        s += b[i*m + k] * c[k*l + j];

      a[i*l+j] = s;
    }
  }
}

/* Factorizacion LU. Parametros:
 *
 * L       - Matriz triangular inferior con 1's en diagonal
 * U       - Matriz triangular superior
 * A       - Matriz a factorizar [n x n]
 * n       - orden de la matriz
 * nthread - numero de hilos
 */
int lu(double *L, double *U, double *A, int n, int nthread) {
  int task, ntask, ktask, size, h, i, j, k, u, r = 0;

  ntask = 2 * n - 1;  /* Numero total de tareas */
  task = k = 0;
  ktask = n - 1 + nthread;

  /* Tamaño del bloque de renglones que ejecuta cada hilo, 
     se considera caso especial en donde la división no es exacta */
  size = (n % nthread != 0)? n / nthread + 1 : n / nthread;

    h = 1; 

    while(k < size && r == 0) {

      /* Calcula fila a partir del identificador del hilo y bloque actual */
      i = h + nthread * k;

      while(task < ktask) {
        j = task - i;  /* Calcula columna a procesar */

        if(i >= 0 && i < n && j >= 0 && j < n) {
          if(i > j) {  /* Calculo elementos l_{i,j} */
            L[i*n+j] = A[i*n+j];
 
            for(u = 0; u < j; u++)
              L[i*n+j] -= L[i*n+u] * U[u*n+j];
              /*M[i*n+j] -= M[i*n+u] * M[u*n+j];*/

            if(U[j*n+j] == 0) { 
              r++;
              printf("Error. Elemento de la diagonal %d es cero en matriz U.\n", j);
            }
         
            L[i*n+j] /= U[j*n+j];

            if(L != U) U[i*n+j] = 0.0;
          }
          else {        /* Calculo elementos u_{i,j} */
            U[i*n+j] = A[i*n+j];

            for(u = 0; u < i; u++)
              U[i*n+j] -= L[i*n+u] * U[u*n+j];

            if(L != U) L[i*n+j] = (i == j)? 1.0 : 0.0;
          }

          if(debug) printf("Tarea %d, hilo %d: m[%d,%d]\n", task, h, i, j);
        }

        task++;
      }

      task = k++ * nthread;  /* Actualiza bloque e inicializa contador de tareas */

      ktask += nthread;
      ktask = (ktask > ntask)? ntask : ktask; 
    }

  if(r != 0) {
    EMO_mprint(stdout, "Factorizacion A", A, n, n);
    EMO_mprint(stdout, "Factorizacion U", U, n, n);
    return 1;
  }

  return 0;
}

/* Factorizacion LU para matrices tridiagonales */
int luT(double *L, double *U, double *A, int n) {
  int i;

  for(i = 0; i < n; i++) {
    U[i*n+i] = A[i*n+i];
   
    if(L != U) 
      L[i*n+i] = 1.0;

    if(i > 0) 
      U[i*n+i] -= L[i*n+i-1] * U[(i-1)*n+i];
 
    if(i < n - 1) {
      U[i*n+i+1] = A[i*n+i+1];

      if(U[i*n+i] == 0)
        return 1;

      L[(i+1)*n+i] = A[(i+1)*n+i] / U[i*n+i];
    }
  }
  return 0;
}

/* Factorizacion Cholesky. Parametros:
 *
 * L       - Matriz triangular inferior
 * Lt      - Transpuesta matriz L
 * A       - Matriz a factorizar [n x n]
 * n       - orden de la matriz
 * nthread - numero de hilos
 */
int cholesky(double *L, double *Lt, double *A, int n, int nthread) {
  int task, ntask, ktask, size, h, i, j, k, u, r = 0;
  double sum;

  ntask = 2 * n - 1;  /* Numero total de tareas */
  task = k = 0;
  ktask = n - 1 + nthread;

  /* Tamaño del bloque de renglones que ejecuta cada hilo, 
     se considera caso especial en donde la división no es exacta */
  size = (n % nthread != 0)? n / nthread + 1 : n / nthread;

    h = 1;

    while(k < size && r == 0) {

      /* Calcula fila a partir del identificador del hilo y bloque actual */
      i = h + nthread * k;

      while(task < ktask) {
        j = task - i;  /* Calcula columna a procesar */

        if(i >= 0 && i < n && j >= 0 && j <= i) {
          sum = 0.0;

          if(i == j) {  /* Calculo elementos de la diagonal */
            for(u = i - 1; u >= 0; u--)
              sum += L[i*n+u] * L[i*n+u];

            sum = A[i*n+i] - sum;

            if(sum <= 0) {
              r++;
            }

            L[i*n+i] = Lt[i*n+i] = sqrt(sum);
          }
          else {        /* Calculo elementos de la matriz triangular inferior */

            for(u = j - 1; u >= 0; u--)
              sum += L[i*n+u] * L[j*n+u];

            L[i*n+j] = Lt[j*n+i] = (A[j*n+i] - sum) / L[j*n+j];
            if(L != Lt) L[j*n+i] = Lt[i*n+j] = 0.0;
          }

          if(debug) printf("Tarea %d, hilo %d: m[%d,%d]\n", task, h, i, j);
        }

        task++;
      }

      task = k++ * nthread;  /* Actualiza bloque e inicializa contador de tareas */

      /* Maximo de tareas por bloque */ /*ktask = n-1 + (k+1) * nthread;*/
      ktask += nthread;
      ktask = (ktask > ntask)? ntask : ktask; 
    }

  if(r != 0) {
    printf("Error, matriz no es positiva definida.\n");
    return 1;
  }
  return 0;
}

/* Factorizacion de Cholesky para matrices tridiagonales */
int choleskyT(double *L, double *Lt, double *A, int n) {
  int i;

  for(i = 0; i < n; i++) {

    L[i*n+i] = A[i*n+i];

    if(i > 0)
      L[i*n+i] = sqrt(L[i*n+i] - L[i*n+i-1] * L[i*n+i-1]);
    else
      L[i*n+i] = sqrt(L[i*n+i]);

    if(L != Lt)
      Lt[i*n+i] = L[i*n+i];

    if(L[i*n+i] == 0)
      return 1;

    if(i < n - 1) 
      L[(i+1)*n+i] = Lt[i*n+i+1] = A[(i+1)*n+i] / L[i*n+i];
  }
  return 0;
}

/* Sustitucion hacia adelante.
   Resuelve para el vector y \in R^n del sistema L y = b. */
int forwardsubstitution(double *y, double *L, double *b, int n) {
  double sum;
  int i, j;

  for(i = 0; i < n; i++) {
    sum = 0.0;

    for(j = i-1; j >= 0; j--) 
      sum += L[i*n+j] * y[j];

    if(L[i*n+i] == 0) { printf("error\n");
      return 1; }

    y[i] = (b[i] - sum) / L[i*n+i];
  }
  return 0;
}

/* Sustitucion hacia atras.
   Resuelve para el vector x \in R^n del sistema U x = y. */
int backsubstitution(double *x, double *U, double *y, int n) {
  double sum;
  int i, j;

  for(i = n-1; i >= 0; i--) {
    sum = 0.0;

    for(j = i+1; j < n; j++)
      sum += U[i*n+j] * x[j];

    if(U[i*n+i] == 0) { printf("error\n");
      return 1; }

    x[i] = (y[i] - sum) / U[i*n+i];
  }
  return 0;
}

/* Obtiene la matriz inversa de de una matriz A, paraleliza con OpenMP */
int EMO_minverse(double *B, double *A, int n, int nthread, int metodo, int tridiag) {
  double *L, *U, *b, *y, *a;
  int i, j, r = 0;
 
  L = (double *) calloc(sizeof(double), n * n);
  U = (double *) calloc(sizeof(double), n * n);

  if(metodo == 0) {
    if(debug) printf("\nFactorizacion por el metodo LU\n");

    if(tridiag)
      r = luT(L, U, A, n);
    else 
      r = lu(L, U, A, n, nthread);
  }
  else {
    if(debug) printf("\nFactorizacion por el metodo de Cholesky LU = L * L^T\n");

    if(tridiag)
      r = choleskyT(L, U, A, n);
    else
      r = cholesky(L, U, A, n, nthread);
  }

  if(r == 1) {
    printf("Error, matriz no invertible.\n");
    EMO_mprint(stdout, "A", A, n, n);
    free(L);
    free(U);
    return 1;
  }

  if(debug) EMO_mprint(stdout, "Factorizacion L", L, n, n);
  if(debug) EMO_mprint(stdout, "Factorizacion U", U, n, n);

    b = (double *) calloc(sizeof(double), n);
    y = (double *) malloc(sizeof(double) * n);
    a = (double *) malloc(sizeof(double) * n);

    for(i = 0; i < n; i++) {

      if(r == 0) {
        b[i] = 1.0;

        if(forwardsubstitution(y, L, b, n) == 1) { printf("-------error\n");
          r++;
        }

        if(backsubstitution(a, U, y, n) == 1) { printf("-------error\n");
          r++;
        }

        for(j = 0; j < n; j++) 
          B[j*n+i] = a[j];

        b[i] = 0.0;
      }
    }
    free(b);
    free(y);
    free(a);

  free(L);
  free(U);

  if(r != 0) {
    printf("Error, matriz no invertible.\n");
    EMO_mprint(stdout, "A", A, n, n);
    return 1;
  }

  return 0;
}

#undef EPSM
#undef NT

