
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "dominance.h"
#include "indicator.h"
#include "numeric.h"
#include "mombi2.h"
#include "vector.h"
#include "evop.h"
#include "stat.h"
#include "sort.h"
#include "io.h"
#include "hv.h"

#define _MAXCHAR 2000

/* Load specific parameters for the algorithm */
void EMO_MOMBI2_load_param(EMO_MOMBI2 *alg, EMO_Param *param, int nvar) {

  if(!EMO_Parser_get_int(param->parser, &param->mu, "psize")) {
    printf("Error, psize is not defined in the configuration file.\n");
    exit(1);
  }

  if(!EMO_Parser_get_double(param->parser, &param->Pc, "pc")) {
    printf("Error, pc is not defined in the configuration file.\n");
    exit(1);
  }

  if(!EMO_Parser_get_double(param->parser, &param->Pm, "pm")) {
    printf("Error, pm is not defined in the configuration file.\n");
    exit(1);
  }

  if(!EMO_Parser_get_double(param->parser, &param->Nc, "nc")) {
    printf("Error, nc is not defined in the configuration file.\n");
    exit(1);
  }

  if(!EMO_Parser_get_double(param->parser, &param->Nm, "nm")) {
    printf("Error, nm is not defined in the configuration file.\n");
    exit(1);
  }

  param->Pm = (param->Pm == -1)? 1.0 / (double) nvar : param->Pm ;
  EMO_Debug_printf(param->dbg, "pm updated %f", param->Pm);

  if(!EMO_Parser_get_char(param->parser, alg->wfile, "wfile")) {
    printf("Error, wfile is not defined in the configuration file.\n");
    exit(1);
  } 

  if(!EMO_Parser_get_int(param->parser, &alg->max_hist, "max_hist")) {
    printf("Error, max_hist is not defined in the configuration file.\n");
    exit(1);
  }

  if(!EMO_Parser_get_double(param->parser, &alg->epsilon, "epsilon")) {
    printf("Error, epsilon is not defined in the configuration file.\n");
    exit(1);
  }

  if(!EMO_Parser_get_double(param->parser, &alg->alpha, "alpha")) {
    printf("Error, alpha is not defined in the configuration file.\n");
    exit(1);
  }
}

/* Update reference points */
void update_refpoint(EMO_MOMBI2 *alg, EMO_Param *param, double *data, int size, int nobj) {
  static int gen = 0;
  double mu, v;
  int i, j, k;

  EMO_minBound(alg->new_min, data, NULL, 2*size, nobj);
 
  for(i = 0; i < nobj; i++) 
    if(alg->new_min[i] < alg->ideal[i])
      alg->ideal[i] = alg->min[i] = alg->new_min[i];

  EMO_maxBound(alg->new_max, data, NULL, size, nobj);

  for(i = 0; i < nobj; i++) {
    k = gen % (alg->max_hist);
    alg->hist[i * alg->max_hist + k] = alg->new_max[i];
  }
 
  if(gen >= alg->max_hist - 1)  {

    for(i = 0; i < nobj; i++) { 
      mu = EMO_mean(alg->hist + i * alg->max_hist, alg->max_hist);
      v = EMO_var(alg->hist + i * alg->max_hist, mu, alg->max_hist);

      if(v > alg->alpha) {
        v = EMO_dmax(NULL, alg->new_max, NULL, nobj);
 
        for(j = 0; j < nobj; j++)
          alg->max[j] = v;
 
        break;
      }
      else {

        if(fabs(alg->max[i] - alg->min[i]) < alg->epsilon) {
          v = EMO_dmax(NULL, alg->max, NULL, nobj);
          alg->max[i] = v;
          alg->update[i] = alg->max_hist;
        }
        else if(alg->new_max[i] > alg->max[i]) {
          alg->max[i] = alg->new_max[i] + fabs(alg->new_max[i] - alg->max[i]);
          alg->update[i] = alg->max_hist;
        }
        else if(v == 0 && (alg->new_max[i] <= mu || (alg->new_max[i] - mu) > alg->epsilon) && alg->update[i] == 0) {
          v = EMO_dmax(NULL, alg->hist + i * alg->max_hist, NULL, alg->max_hist);
          alg->max[i] = (alg->max[i] + v) / 2.0;
          alg->update[i] = alg->max_hist;
        }
      }

      if(alg->update[i] > 0) alg->update[i]--;
    }
  }
  gen++;
}

void EMO_MOMBI2_alloc(EMO_MOMBI2 *alg, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop) {
  int i;


  if((alg->wfile = (char *) malloc(sizeof(char) * _MAXCHAR)) == NULL) {
    printf("Error, not enough memory in MOMBI2.\n");
    exit(1);
  }

  EMO_MOMBI2_load_param(alg, param, mop->nvar);
 
  if(param->mu % 2 != 0) {
    printf("Error, population size must be even.\n");
    exit(1);
  }

  EMO_Population_alloc(pop, mop, param->mu, param->mu);
  EMO_initList(&alg->lst1, pop->mu);
  EMO_initList(&alg->lst2, pop->mu);

  if((alg->norm = (double *) malloc(sizeof(double) * pop->size * mop->nobj)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

  if((alg->rank = (double *) malloc(sizeof(double) * pop->size)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

  if((alg->l2 = (double *) malloc(sizeof(double) * pop->size)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

  if((alg->sort = (double **) malloc(sizeof(double *) * pop->size)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

  for(i = 0; i < pop->size; i++) {
    if((alg->sort[i] = (double *) malloc(sizeof(double) * 3)) == NULL) {
      printf("Error, not enough memory in mombi2.\n");
      exit(1);
    }
  }  

  alg->ssize = pop->size;

  if((alg->tmp = (double *) malloc(sizeof(double) * mop->nobj)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

  alg->wsize = 0;
  alg->W = EMO_File_read(NULL, &alg->wsize, &mop->nobj, alg->wfile, 0);

  if((alg->min = (double *) malloc(sizeof(double) * mop->nobj)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

  if((alg->max = (double *) malloc(sizeof(double) * mop->nobj)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

  EMO_maxminBound(alg->max, alg->min, pop->obj, NULL, pop->mu, mop->nobj);

  if((alg->ideal = (double *) malloc(sizeof(double) * mop->nobj)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

  if((alg->new_min = (double *) malloc(sizeof(double) * mop->nobj)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

  if((alg->new_max = (double *) malloc(sizeof(double) * mop->nobj)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

  if((alg->hist = (double *) malloc(sizeof(double) * alg->max_hist * mop->nobj)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

  if((alg->update = (int *) malloc(sizeof(int) * mop->nobj)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

  for(i = 0; i < mop->nobj; i++) {
    alg->update[i] = 0;
  }

  EMO_Utility_alloc(&alg->utl, mop->nobj, "asf");

  if((alg->filter = (int *) calloc(sizeof(int), pop->size)) == NULL) {
    printf("Error, not enough memory in mombi2.\n");
    exit(1);
  }

}

void EMO_MOMBI2_free(EMO_MOMBI2 *alg) {
  int i;

  free(alg->wfile);
  EMO_freeList(&alg->lst1);
  EMO_freeList(&alg->lst2);
  free(alg->norm);
  free(alg->rank);
  free(alg->l2);

  for(i = alg->ssize - 1; i > -1; i--)
    free(alg->sort[i]);

  free(alg->sort);

  free(alg->tmp);
  free(alg->W);
  free(alg->min);
  free(alg->max);
  free(alg->ideal);
  free(alg->new_min);
  free(alg->new_max);
  free(alg->hist);
  free(alg->update);

  EMO_Utility_free(&alg->utl);
  free(alg->filter);
}

void EMO_MOMBI2_run(EMO_MOMBI2 *alg, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop) {
  int p1, p2, v1, v2, o1, o2, i, j;

  EMO_minBound(alg->ideal, pop->obj, NULL, pop->mu, mop->nobj);
  EMO_maxBound(alg->max, pop->obj, NULL, pop->mu, mop->nobj);

  for(i = 0; i < mop->nobj; i++) {
    j = i * alg->max_hist;
    alg->hist[j] = alg->max[i];
  }
  
  EMO_Debug_printf(param->dbg, "run MOMBI2");

  while(!EMO_Stop_end(param->stop)) {
    /* Offspring generation */
    for(i = 0; i < pop->lambda; i+=2) {
      // Tournament selection
      /* Randomly select two parents */
      p1 = EMO_Rand_int1(param->rand, 0, pop->mu-1);
      while ((p2 = EMO_Rand_int1(param->rand, 0, pop->mu-1)) == p1);

      p1 *= mop->nvar;
      p2 *= mop->nvar;
      
      v1 = o1 = pop->mu + i;
      v2 = o2 = pop->mu + i + 1;
      v1 *= mop->nvar;
      v2 *= mop->nvar;
      o1 *= mop->nobj;
      o2 *= mop->nobj;

      /* Generate an offspring by variation operators */
      EMO_crossSBX(pop->var+v1, pop->var+v2, pop->var+p1, pop->var+p2, param->rand, mop, param->Pc, param->Nc);
      EMO_mutatePolynom(pop->var+v1, param->rand, mop, param->Pm, param->Nm);
      EMO_mutatePolynom(pop->var+v2, param->rand, mop, param->Pm, param->Nm);
      mop->f(mop, pop->obj+o1, pop->var+v1);
      mop->f(mop, pop->obj+o2, pop->var+v2);
    }

    update_refpoint(alg, param, pop->obj, pop->mu, mop->nobj);
    EMO_normalize(alg->norm, pop->obj, NULL, pop->size, alg->min, alg->max, mop->nobj);

    EMO_r2_ranking(alg->rank, alg->sort, alg->l2, alg->tmp, alg->norm, pop->size, alg->W, alg->wsize, &alg->utl);

    for(i = 0; i < pop->size; i++) {
      alg->sort[i][0] = (double) i; 
      alg->sort[i][1] = alg->rank[i];
    }

    qsort(alg->sort, pop->size, sizeof(alg->sort[0]), (int (*)(const void *, const void *))&EMO_compare_asc);
    memset(alg->filter, 0, sizeof(int) * pop->size);

    for(i = 0; i < pop->mu; i++) {
      j = (int) alg->sort[i][0];
      alg->filter[j] = 1; 
    }

    /* Reduce population */
    EMO_Population_survive(pop, NULL, alg->rank, mop, &alg->lst1, &alg->lst2, alg->filter);
    EMO_Plot_run(param->plot, pop->obj, mop->feval / pop->mu, 0);
  } 
}

void EMO_MOMBI2_prun(EMO_MOMBI2 *alg, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop) {
  int j, k, n;

    /* Run algorithm */
    EMO_MOMBI2_run(alg, param, pop, mop);

    for(j = 0; j < pop->mu; j++) {
      alg->sort[j][0] = (double) j; 
      alg->sort[j][1] = alg->rank[j];
    }

    qsort(alg->sort, pop->mu, sizeof(alg->sort[0]), (int (*)(const void *, const void *))&EMO_compare_asc);

     /* Select discarded individuals for keeping immigrants */
    EMO_clear(param->immigrant);

    for(j = 0; j < param->nimmigrant; j++) {
      k = (int) alg->sort[pop->mu-1-j][0];
      EMO_queue(param->immigrant, k);
    }

    /* Select emigrants */
    EMO_clear(param->emigrant);
    n = param->nemigrant / 2;

    for(j = 0; j < n; j++) {
      k = (int) alg->sort[j][0];
      EMO_queue(param->emigrant, k);

      k = (int) alg->sort[pop->mu-1-j][0];
      EMO_queue(param->emigrant, k);
    }
}

#undef _MAXCHAR

