/* Evolutionary operators */
/* evop.c   Crossover, mutation and selection routines */

#include <math.h>
#include "evop.h"
#include "random.h"
#include "dominance.h"

#define _EPS 1.2e-7

/* Routine for real variable SBX crossover */
void EMO_crossSBX(double *child1, double *child2, double *parent1, double *parent2, EMO_Rand *rand, EMO_MOP *mop, double Pc, double eta_c) {
  double alpha, beta, betaq;
  double y1, y2, yl, yu;
  double c1, c2;
  double rnd;
  int i;

  if (EMO_Rand_flip(rand, Pc)) {
    for (i = 0; i < mop->nvar; i++) {

      if (EMO_Rand_flip(rand, 0.5)) {

        /* significant difference between parents */
        if (fabs(parent1[i] - parent2[i]) > _EPS) {

          if (parent1[i] < parent2[i]) {
            y1 = parent1[i];
            y2 = parent2[i];
          }
          else {
            y1 = parent2[i];
            y2 = parent1[i];
          }

          yl = mop->xmin[i];
          yu = mop->xmax[i];
          rnd = EMO_Rand_prob3(rand);
          beta = 1.0 + (2.0*(y1-yl)/(y2-y1));
          alpha = 2.0 - pow(beta,-(eta_c+1.0));

          if (rnd <= (1.0/alpha))
            betaq = pow ((rnd*alpha),(1.0/(eta_c+1.0)));
          else
            betaq = pow ((1.0/(2.0 - rnd*alpha)),(1.0/(eta_c+1.0)));

          c1 = 0.5*((y1+y2)-betaq*(y2-y1));
          beta = 1.0 + (2.0*(yu-y2)/(y2-y1));
          alpha = 2.0 - pow(beta,-(eta_c+1.0));

          if (rnd <= (1.0/alpha))
            betaq = pow ((rnd*alpha),(1.0/(eta_c+1.0)));
          else
            betaq = pow ((1.0/(2.0 - rnd*alpha)),(1.0/(eta_c+1.0)));

          c2 = 0.5*((y1+y2)+betaq*(y2-y1));

          if (c1 < yl) c1 = yl;
          if (c2 < yl) c2 = yl;
          if (c1 > yu) c1 = yu;
          if (c2 > yu) c2 = yu;

          if (EMO_Rand_flip(rand, 0.5)) {
            child1[i] = c2;
            child2[i] = c1;
          }
          else {
            child1[i] = c1;
            child2[i] = c2;
          }
        }
        else {
          child1[i] = parent1[i];
          child2[i] = parent2[i];
        }
      }
      else {
        child1[i] = parent1[i];
        child2[i] = parent2[i];
      }
    }
  }
  else {
    for (i = 0; i < mop->nvar; i++) {
      child1[i] = parent1[i];
      child2[i] = parent2[i];
    }
  }
}


/* Routine for real polynomial mutation of an individual */
void EMO_mutatePolynom(double *child, EMO_Rand *rand, EMO_MOP *mop, double Pm, double eta_m) {
  double rnd, delta1, delta2, mut_pow, deltaq;
  double y, yl, yu, val, xy;
  int j;

  for (j = 0; j < mop->nvar; j++) {
    if (EMO_Rand_flip(rand, Pm)) {
      y = child[j];
      yl = mop->xmin[j];
      yu = mop->xmax[j];
      delta1 = (y-yl)/(yu-yl);
      delta2 = (yu-y)/(yu-yl);
      rnd = EMO_Rand_prob3(rand);
      mut_pow = 1.0/(eta_m+1.0);

      if (rnd <= 0.5) {
        xy = 1.0-delta1;
        val = 2.0*rnd+(1.0-2.0*rnd)*(pow(xy,(eta_m+1.0)));
        deltaq =  pow(val,mut_pow) - 1.0;
      }
      else {
        xy = 1.0-delta2;
        val = 2.0*(1.0-rnd)+2.0*(rnd-0.5)*(pow(xy,(eta_m+1.0)));
        deltaq = 1.0 - (pow(val,mut_pow));
      }

      y = y + deltaq*(yu-yl);

      if (y < yl) y = yl;
      if (y > yu) y = yu;

      child[j] = y;
      //nrealmut+=1;
    }
  }
}

/* Implementacion de cruza por un punto. Argumentos:
 *
 * p1: padre 1
 * p2: padre 2
 * c1: hijo 1
 * c2: hijo 2 */
void EMO_one_point_crossover(double *child1, double *child2, double *parent1, double *parent2, EMO_Rand *rand, EMO_MOP *mop, double Pc) {

  int i, cp = -1;

  if(EMO_Rand_flip(rand, Pc)) {
    cp = EMO_Rand_int1(rand, 0, mop->L-1);

    /* Intercambia encabezados del cromosoma */
    for(i = 0; i < cp; i++) {
      child1[i] = parent2[i];
      child2[i] = parent1[i];
    }

    /* Copia colas del cromosoma */
    for(i = cp; i < mop->L; i++) {
      child1[i] = parent1[i];
      child2[i] = parent2[i];
    }
  }
  else {
    for(i = 0; i < mop->L; i++) {
      child1[i] = parent1[i];
      child2[i] = parent2[i];
    }
  }
}


/* flip one bit at random position */
void EMO_one_bit_mutation(double *child, EMO_Rand *rand, EMO_MOP *mop) {
  int cp;

  cp = EMO_Rand_int1(rand, 0, mop->L-1);
  child[cp] = (child[cp] == 0)? 1 : 0;
}


void tournament_selection_dominance(int *ind, EMO_Rand *rand, int *seedp, double *obj, int n, int nobj) {
  int p1, p2, i, j = 0, r;

 EMO_Rand_shuffle(rand, seedp, n);

  for(i = 0; i < n; i+=2) {
    p1 = seedp[i];
    p2 = seedp[i+1];

    r = EMO_Dominance_strict(obj + p1 * nobj, obj + p2 * nobj, nobj);

    if(r == 1)
       ind[j++] = p1;
    else if(r == -1)
      ind[j++] = p2;
    else {
      if (EMO_Rand_flip(rand, 0.5))
        ind[j++] = p1;
      else
        ind[j++] = p2;
    }
  }
}


void tournament_selection_rank(int *ind, EMO_Rand *rand, int *seedp, int *rank, int n) {
  int p1, p2, i, j = 0;

 EMO_Rand_shuffle(rand, seedp, n);

  for(i = 0; i < n; i+=2) {
    p1 = seedp[i];
    p2 = seedp[i+1];

    if(rank[p1] < rank[p2])
       ind[j++] = p1;
    else if(rank[p2] < rank[p1])
      ind[j++] = p2;
    else {
      if (EMO_Rand_flip(rand, 0.5))
        ind[j++] = p1;
      else
        ind[j++] = p2;
    }
  }
}

void tournament_selection_rank_crowding(int *ind, EMO_Rand *rand, int *seedp, int *rank, double *cd, int n){
 int p1, p2, i, j = 0;

 EMO_Rand_shuffle(rand, seedp, n);

  for(i = 0; i < n; i+=2) {
    p1 = seedp[i];
    p2 = seedp[i+1];

    if(rank[p1] < rank[p2])
       ind[j++] = p1;
    else if(rank[p2] < rank[p1])
      ind[j++] = p2;
    else {
       if ( cd[p1] > cd [p2] )
          ind[j++]=p1;
       else
	  ind[j++]=p2; 
    }
  }
}

void tournament_selection_fitness(int *ind, EMO_Rand *rand, int *seedp, double *fit, int n) {
  int p1, p2, i, j = 0;

  EMO_Rand_shuffle(rand, seedp, n);

  for(i = 0; i < n; i+=2) {
    p1 = seedp[i];
    p2 = seedp[i+1];

    if(fit[p1] > fit[p2]) {
       ind[j++] = p1;
    }
    else if(fit[p2] > fit[p1]) {
      ind[j++] = p2;
    }
    else {
      if (EMO_Rand_flip(rand, 0.5))
        ind[j++] = p1;
      else
        ind[j++] = p2;
    }
  }
}

void tournament_selection_r2(int *ind, EMO_Rand *rand, int *seedp, double *r2, int n, int m) {
  int p1, p2, i, j = 0;
  int x, y;

  EMO_Rand_shuffle(rand, seedp, n);

  for(i = 0; i < n; i+=2) {
    p1 = seedp[i];
    p2 = seedp[i+1];
    x = p1 * m + p2;
    y = p2 * m + p1;

    if(r2[x] < r2[y]) {
       ind[j++] = p1;
    }
    else if(r2[y] < r2[x]) {
      ind[j++] = p2;
    }
    else {
      if (EMO_Rand_flip(rand, 0.5))
        ind[j++] = p1;
      else
        ind[j++] = p2;
    }
  }
}

int tournament_selection_fitness2(double *fit, EMO_Rand *rand, int size) {
  int p1, p2;

  p1 = EMO_Rand_int1(rand, 0, size);
  while ((p2 = EMO_Rand_int1(rand, 0, size)) == p1);

  if(fit[p1] > fit[p2])
    return p1;

  if(fit[p2] > fit[p1])
    return p2;
 
  if(EMO_Rand_flip(rand, 0.5))
    return p1;
  else
    return p2;
}

#undef _EPS

