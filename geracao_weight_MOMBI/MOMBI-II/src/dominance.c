
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include "dominance.h"
#include "sort.h"

/* Weak dominance relation 
 *
 * A solution x weakly dominates a solution y, iff
 * x[i] <= y[i] for all i in {1,..,n}.
 *
 * Return value: 
 *  1 - if x weakly dominates y
 *  0 - if x and y are mutually non weakly dominated 
 * -1 - if y weakly dominates x
 */
int EMO_Dominance_weak(double *x, double *y, int n) {
  int i, fx, fy;

  fx = fy = 0;

  for(i = 0; i < n; i++) {
    if(x[i] <= y[i]) 
      fx++;
    if(x[i] >= y[i])
      fy++;
  }

  if(fx == n)
    return 1;
  if(fy == n)
    return -1;

  return 0;
}

/* Strict dominance relation 
 *
 * A solution x strictly dominates a solution y, iff
 * x[i] <= y[i] for all i in {1,..,n} and x[j] < y[j] for at least one j in {1,..,n}.
 *
 * Return value: 
 *  1 - if x dominates y
 *  0 - if x and y are mutually non dominated  (INCOMPARABLE)
 * -1 - if y dominates x
 */
int EMO_Dominance_strict(double *x, double *y, int n) {
  int i, fx, fy;

  fx = fy = 0;

  for(i = 0; i < n; i++) {
    if(x[i] < y[i])
      fx++;
    else if(x[i] > y[i])
      fy++;
  }

  if(fx > 0 && fy == 0)
    return 1;

  if(fy > 0 && fx == 0)
    return -1;

  return 0;
}

/* Strong dominance relation
 *
 * A solution x strongly dominates a solution y, iff
 * x[i] < y[i] for all i in {1,..,n}.
 *
 * Return value: 
 *  1 - if x strongly dominates y
 *  0 - if x and y are mutually non strongly dominated 
 * -1 - if y strongly dominates x 
 */
int EMO_Dominance_strong(double *x, double *y, int n) {
  int i, fx, fy;

  fx = fy = 0;

  for(i = 0; i < n; i++) {
    if(x[i] < y[i])
      fx++;
    else if(x[i] > y[i])
      fy++;
  }

  if(fx == n)
    return 1;

  if(fy == n)
    return -1;

  return 0;
}

/* alpha-dominance relation (Ikeda Kokolo, et al., 2001)
 *
 * A solution x strictly dominates a solution y, iff
 * x[i] <= y[i] for all i in {1,..,n} and x[j] < y[j] for at least one j in {1,..,n}.
 *
 * Return value: 
 *  1 - if x dominates y
 *  0 - if x and y are mutually non dominated  (INCOMPARABLE)
 * -1 - if y dominates x
 */
int EMO_Dominance_alpha2(double *x, double *y, double *tmp, double *alpha, int n) {
  int i, j, fx, fy;

  for(i = 0; i < n; i++) {
    tmp[i] = 0;

    for(j = 0; j < n; j++) {
      if(i == j)
        tmp[i] += x[j] - y[j];
      else
        tmp[i] += alpha[i*n + j] * (x[j] - y[j]);
    }
  }

  fx = fy = 0;

  for(i = 0; i < n; i++) {
    if(tmp[i] < 0)
      fx++;
    else if(tmp[i] > 0)
      fy++;
  }

  if(fx > 0 && fy == 0)
    return 1;

  if(fy > 0 && fx == 0)
    return -1;

  return 0;
}

/* Ikeda, Kokolo dominance */
int EMO_Dominance_alpha(double *x, double *y, double *tmp, double *alpha, int n) {
  int i, j, fx, fy;

  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      if(i == j)
        tmp[i] += x[j] - y[j];
      else
        tmp[i] += alpha[i*n + j] * (x[j] - y[j]);
    }
  }

  fx = fy = 0;

  for(i = 0; i < n; i++) {
    if(tmp[i] < 0)
      fx++;
    else if(tmp[i] > 0)
      fy++;
  }

  if(fx > 0 && fy == 0)
    return 1;

  if(fy > 0 && fx == 0)
    return -1;

  return 0;
}

/*int EMO_Dominance_alpha(double *x, double *y, double *tmp, double *alpha, int n) {
  int i, j, fx, fy;
  double eps = 1e-4;

  for(i = 0; i < n; i++) {
    if(fabs(x[i] - y[i]) < eps)
      tmp[i] = 0;
    else
      tmp[i] = x[i] - y[i];

//    for(j = 0; j < n; j++) {
//      if(i == j)
//        tmp[i] += x[j] - y[j];
//      else
//        tmp[i] += alpha[i*n + j] * (x[j] - y[j]);
//    }
  }

  fx = fy = 0;

  for(i = 0; i < n; i++) {
    if(tmp[i] < 0)
      fx++;
    else if(tmp[i] > 0)
      fy++;
  }

  if(fx > 0 && fy == 0)
    return 1;

  if(fy > 0 && fx == 0)
    return -1;

  return EMO_Dominance_strict(x, y, n);
  //return 0;
}*/


/* Vectors x and y are incomparable with respect to a
 * dominance relation r if they are mutually non r-dominated.
 *
 * Return value:
 * 1 - vectors are incomparable
 * 0 - vectors are comparable
 */
int EMO_Dominance_incomparable(double *x, double *y, int n, EMO_Dominance r) {
  return (r(x,y,n) == 0)? 1 : 0;
}

/* Vectors x ans y are indifferent iff x[i] = y[i] for all i in {1,...,n}.
 *
 * Return value:
 * 1 - vectors are indifferent
 * 0 - vectors are not indifferent
 */
int EMO_Dominance_indifferent(double *x, double *y, int n) {
  int i;

  for(i = 0; i < n; i++)
    if(x[i] != y[i])
      return 0;

  return 1;
}

/* Constraint-domination
 *
 * x constraint-dominates y iff any of the following conditions is true:
 * 
 * - x is feasible and y is not.
 * - x and y are infeasible, but x has a smaller constraint violation
 * - x and y are feasible and x dominates y in objective function space
 *
 * Deb's definition (see book of Multi-Objective Optimization using EAs)
 */
int EMO_Dominance_constraint(double *x, double *y, int n, double *gx, double *gy, int k, EMO_Dominance r) {
  double vx, vy;
  int i;

  vx = vy = 0; /* constraint violation */

  for(i = 0; i < k; i++) {
    if(gx[i] > 0)
      vx += gx[i];

    if(gy[i] > 0) 
      vy += gy[i];
  }

  if(vx == 0) {
    if(vy == 0) 
      return r(x,y,n);
    else 
      return 1;
  }
  else {
    if(vy == 0)
      return -1;
    else
      if(vx < vy)
        return 1;
      else if(vx > vy)
        return -1;
  }

  return 0;
}

/* Constraint-domination
 *
 * x constraint-dominates y iff any of the following conditions is true:
 * 
 * - x is feasible and y is not.
 * - x and y are infeasible and x dominates y in constraint function space
 * - x and y are feasible and x dominates y in objective function space
 *
 * Lampinen and Kukkonen's definition (see GDE3 paper)
 */
int EMO_Dominance_constraint2(double *x, double *y, int n, double *gx, double *gy, int k, EMO_Dominance r) {
  int vx, vy, i;

  vx = vy = 0; /* constraint violation */

  for(i = 0; i < k; i++) {
    if(gx[i] > 0) 
      vx = 1;
    if(gy[i] > 0) 
      vy = 1;
  }

  if(vx == 0) {
    if(vy == 0) 
      return r(x,y,n);
    else 
      return 1;
  }
  else {
    if(vy == 0)
      return -1;
    else
      return r(gx,gy,k);  /* special case r(gx,gy,k) = 0, -> r(x,y,k) */
  }

  return 0;
}

int EMO_Dominance_feasible(double *g, int k) {
  int i;

  for(i = 0; i < k; i++) {
    if(g[i] > 0) 
      return 0;
  }
  return 1;
}

/* Retrives the Pareto set (non-dominated solutions), strong Pareto set and weak Pareto set */
int EMO_Dominance_ndset(int *nd, double *data, int row, int col, EMO_Dominance r) {
  int i, j, res, cont = 0;

  memset(nd, 0, sizeof(int) * row);

  if(r == EMO_Dominance_strong) {
    for(i = 0; i < row; i++) {
      for(j = 0; j < row; j++) {
 
        if(i == j)
          continue;

        res = EMO_Dominance_strict(data + j*col, data + i*col, col);
        if(res == 1) break;

        res = EMO_Dominance_weak(data + i*col, data + j*col, col);
        if(res == 1) break;
      }

      if(j == row) {
        nd[i] = 1;
        cont++;
      }
      else {
        nd[i] = 0;
      }

      //nd[i] = (j == row)? 1 : 0;
    }
  }
  else {
    if(r == EMO_Dominance_weak)
      r = EMO_Dominance_strong;

    for(i = 0; i < row; i++) {
      for(j = 0; j < row; j++) {
 
        if(i == j)
          continue;

        res = r(data + j*col, data + i*col, col);

        if(res == 1)
          break;

      }

      if(j == row) {
        nd[i] = 1;
        cont++;
      }
      else {
        nd[i] = 0;
      }

      //nd[i] = (j == row)? 1 : 0;
    }
  }
  return cont;
}

void EMO_NDSort_alloc(EMO_NDSort *nd, int max_size) {
  int i;

  max_size++;

  nd->n = (int *) malloc(sizeof(int) * max_size);
  if(nd->n == NULL) 
    printf("not enough memory\n");

  nd->rank = (int *) malloc(sizeof(int) * max_size);
  if(nd->rank == NULL)
    printf("not enogh memory\n");

  nd->S = (EMO_List *) malloc(sizeof(EMO_List) * max_size);
  if(nd->S == NULL)
    printf("not enogh memory\n");

  nd->front = (EMO_List *) malloc(sizeof(EMO_List) * max_size);
  if(nd->front == NULL)
    printf("not enogh memory\n");

  for(i = 0; i < max_size; i++) {
    EMO_initList(&(nd->S[i]), 10);
    EMO_initList(&(nd->front[i]), 10);
  }

  nd->size = max_size;
}

void EMO_NDSort_free(EMO_NDSort *nd) {
  int i;

  for(i = 0; i < nd->size; i++) {
    EMO_freeList(&(nd->S[i]));
    EMO_freeList(&(nd->front[i]));
  }

  free(nd->n);
  free(nd->rank);
  free(nd->S);
  free(nd->front);
}

/* Routine to perform non-dominated sorting */
void EMO_NDSort_run(EMO_NDSort *nd, double *obj, int *filter, int size, int nobj) {
  int i, j, v, f;

  EMO_clear(&nd->front[0]);

  if(filter == NULL) {
    for(i = 0; i < size; i++) {
      nd->n[i] = 0;

      for(j = 0; j < size; j++) {
        if(i == j) continue;

        //v = dominance(pop->ind[i], pop->ind[j], m);
        v = EMO_Dominance_strict(obj + i*nobj, obj + j*nobj, nobj);
        if(v == 1) EMO_queue(&nd->S[i] , j);
        if(v == -1) nd->n[i]++;
      }

      if(nd->n[i] == 0) {
        nd->rank[i] = 0;
        EMO_queue(&nd->front[0], i);
      }
      //pop->ind[i].d = pop->ind[i].n;
    }
  }
  else {
    for(i = 0; i < size; i++) {

      if(!filter[i]) continue;

      nd->n[i] = 0;

      for(j = 0; j < size; j++) {
        if(i == j || !filter[i]) continue;

        //v = dominance(pop->ind[i], pop->ind[j], m);
        v = EMO_Dominance_strict(obj + i*nobj, obj + j*nobj, nobj);
        if(v == 1) EMO_queue(&nd->S[i] , j);
        if(v == -1) nd->n[i]++;
      }

      if(nd->n[i] == 0) {
        nd->rank[i] = 0;
        EMO_queue(&nd->front[0], i);
      }
      //pop->ind[i].d = pop->ind[i].n;
    }
  }
  f = 0;

  while(nd->front[f].length != 0) {
    EMO_clear(&nd->front[f+1]);

    for(i = 0; i < nd->front[f].length; i++) {
      v = EMO_element(nd->front[f], i);

      while(nd->S[v].length != 0) {
        j = EMO_dequeue(&nd->S[v]);
        nd->n[j]--;

        if(nd->n[j] == 0) {
          nd->rank[j] = f + 1;
          EMO_queue(&nd->front[f+1], j);
        }
      }
    }
    f++;
    //lprint(front[f]);
  }

  nd->nfront = f;
}

/* Routine to perform non-dominated sorting */
/*void EMO_NDSort_run2(EMO_NDSort *nd, double *obj, int *filter, double *tmp, double *alpha, int size, int nobj) {
  int i, j, v, f;

  EMO_clear(&nd->front[0]);

  if(filter == NULL) {
    for(i = 0; i < size; i++) {
      nd->n[i] = 0;

      for(j = 0; j < size; j++) {
        if(i == j) continue;

        //v = dominance(pop->ind[i], pop->ind[j], m);
        v = EMO_Dominance_alpha(obj + i*nobj, obj + j*nobj, tmp, alpha, nobj);
        if(v == 1) EMO_queue(&nd->S[i] , j);
        if(v == -1) nd->n[i]++;
      }

      if(nd->n[i] == 0) {
        nd->rank[i] = 0;
        EMO_queue(&nd->front[0], i);
      }
      //pop->ind[i].d = pop->ind[i].n;
    }
  }
  else {
    for(i = 0; i < size; i++) {

      if(!filter[i]) continue;

      nd->n[i] = 0;

      for(j = 0; j < size; j++) {
        if(i == j || !filter[i]) continue;

        //v = dominance(pop->ind[i], pop->ind[j], m);
        v = EMO_Dominance_alpha(obj + i*nobj, obj + j*nobj, tmp, alpha, nobj);
        if(v == 1) EMO_queue(&nd->S[i] , j);
        if(v == -1) nd->n[i]++;
      }

      if(nd->n[i] == 0) {
        nd->rank[i] = 0;
        EMO_queue(&nd->front[0], i);
      }
      //pop->ind[i].d = pop->ind[i].n;
    }
  }
  f = 0;

  while(nd->front[f].length != 0) {
    EMO_clear(&nd->front[f+1]);

    for(i = 0; i < nd->front[f].length; i++) {
      v = EMO_element(nd->front[f], i);

      while(nd->S[v].length != 0) {
        j = EMO_dequeue(&nd->S[v]);
        nd->n[j]--;

        if(nd->n[j] == 0) {
          nd->rank[j] = f + 1;
          EMO_queue(&nd->front[f+1], j);
        }
      }
    }
    f++;
    //lprint(front[f]);
  }

  nd->nfront = f;
}*/

// agg: sum, min
void EMO_rankingDominance_sum(double *rank, double **sort, double *data, int size, int nobj) {
  int i, j;

  memset(rank, 0, sizeof(double) * size);

  for(i = 0; i < nobj; i++) {
    for(j = 0; j < size; j++) {
      sort[j][0] = (double) j;
      sort[j][1] = data[j*nobj + i];
    }

    qsort(sort, size, sizeof(sort[0]), (int (*)(const void *, const void *))&EMO_compare_asc);
    
    for(j = 1; j <= size; j++)
      rank[ (int) sort[j-1][0] ] += (double) j;
  }
}

void EMO_rankingDominance_min(double *rank, double **sort, double *data, int size, int nobj) {
  int i, j, k;
  double v;

  for(j = 0; j < size; j++)
    rank[j] = DBL_MAX;

  for(i = 0; i < nobj; i++) {
    for(j = 0; j < size; j++) {
      sort[j][0] = (double) j;
      sort[j][1] = data[j*nobj + i];
    }

    qsort(sort, size, sizeof(sort[0]), (int (*)(const void *, const void *))&EMO_compare_asc);
    
    for(j = 1; j <= size; j++) {
      k = (int) sort[j-1][0];
      v = (double) j;

      if(v < rank[k])
        rank[k] = v;
    }
  }
}

