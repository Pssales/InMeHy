
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "benchmark.h"
#include "io.h"

#define _MAXCHAR 2000

/* 
 * from_file: variable population
 */
void EMO_Population_alloc(EMO_Population *pop, EMO_MOP *mop, int mu, int lambda) {

  pop->size = mu + lambda;
  pop->mu = mu;
  pop->lambda = lambda;

  if((pop->obj = (double *) calloc(sizeof(double), mop->nobj * pop->size)) == NULL) {
    printf("Error, not enough memory.\n");
    exit(1);
  }

  if((pop->odummy = (double *) malloc(sizeof(double) * mop->nobj)) == NULL){
    printf("Error, not enough memory.\n");
    exit(1);
  }

  if(mop->ncon == 0) {
    pop->con = NULL;
  }
  else {
    if((pop->con = (double *) calloc(sizeof(double), mop->ncon * pop->size)) == NULL){
      printf("Error, not enough memory.\n");
      exit(1);
    }

    if((pop->cdummy = (double *) malloc(sizeof(double) * mop->ncon)) == NULL){
      printf("Error, not enough memory.\n");
      exit(1);
    }
  }

  if((pop->vdummy = (double *) malloc(sizeof(double) * mop->nvar)) == NULL){
    printf("Error, not enough memory.\n");
    exit(1);
  }

  //pop->var = NULL;

  if((pop->var = (double *) calloc(sizeof(double), mop->nvar * pop->size)) == NULL) {
    printf("Error, not enough memory.\n");
    exit(1);
  }
}

void EMO_Population_init(EMO_Population *pop, EMO_Rand *rand, EMO_MOP *mop) {
  int i, j, k;

  /* Random initialization */
  printf("Random inicialization of population.\n");

  printf("mu %d, var %d, obj %d\n", pop->mu, mop->nvar, mop->nobj);

  if(mop->coding == EMO_REAL) { 
    for(i = 0; i < pop->mu; i++) {
      k = i * mop->nvar;

      for(j = 0; j < mop->nvar; j++)
        pop->var[k + j] = EMO_Rand_real1(rand, mop->xmin[j], mop->xmax[j]);

      mop->f(mop, &pop->obj[i*mop->nobj], &pop->var[k]);
    }
  }

  if(mop->coding == EMO_BINARY) { 
    for(i = 0; i < pop->mu; i++) {
      k = i * mop->nvar;

      for(j = 0; j < mop->nvar; j++)
        pop->var[k + j] = EMO_Rand_int1(rand, (int) mop->xmin[j], (int) mop->xmax[j]);

      mop->f(mop, &pop->obj[i*mop->nobj], &pop->var[k]);
    }
  }
}

/* Initialization from file */
int EMO_Population_init_from_file(EMO_Population *pop, EMO_MOP *mop, const char *from_file, int start) {
  int i, k, size;

  printf("mu %d, var %d, obj %d\n", pop->mu, mop->nvar, mop->nobj);

  printf("Reading population from file %s.\n", from_file);//, verificar codificacion binaria
  size = pop->mu;

  EMO_File_read(pop->var, &size, &mop->nvar, from_file, start);

  printf("mu %d, size %d, var %d, obj %d\n", pop->mu, size, mop->nvar, mop->nobj);

  for(i = 0; i < size; i++) {
    k = i + start;
    mop->f(mop, pop->obj + k*mop->nobj, pop->var + k*mop->nvar);

    //for(j = 0; j < mop->nvar; j++)
    //  printf("%f ", pop->var[k * mop->nvar + j]);
    //printf(" = %f %f\n", pop->obj[i*mop->nobj], pop->obj[i*mop->nobj+1]);
  }
  return size;
}

void EMO_Population_write(EMO_Population *pop, int *filter, EMO_MOP *mop, const char *prefix, int run) {
  char file[_MAXCHAR];
  int flag;

  flag = strcmp(prefix, "stdout");

  if(flag == 0)
    strcpy(file, "stdout");
  else if(run == 0)
    sprintf(file, "%s.pos", prefix);
  else
    sprintf(file, "%s%.2d.pos", prefix, run);

  EMO_File_write(pop->var, filter, pop->mu, mop->nvar, file, "%e ");

  if(flag != 0) {
    if(run == 0)
      sprintf(file, "%s.pof", prefix);
    else
      sprintf(file, "%s%.2d.pof", prefix, run);
  }

  EMO_File_write(pop->obj, filter, pop->mu, mop->nobj, file, "%e ");

  //EMO_Plot_run(param->plot, pop->obj, mop->feval / pop->mu, 1);

  //param->stop.fitness = 5;
  //EMO_hv2(pop->obj, pop->mu, ref, mop->nobj); // falta ref
  //printf("HV %s: %f\n", str, stop.fitness);

  //fprintf(param->fp, "%d %ld %e %ld.%04ld\n", run, mop->feval, param->stop.fitness, param->stop.total_time.tv_sec, param->stop.total_time.tv_usec);
  //fflush(param->fp);

  //EMO_Debug_printf(&param->dbg, "Total execution time: %lds.%04ld", param->stop.total_time.tv_sec, param->stop.total_time.tv_usec);
  //EMO_Debug_printf(&param->dbg, "Number of objective function evaluations: %d", mop->feval);
  //EMO_Debug_printf(&param->dbg, "Performance indicator of the Pareto front: %f", param->stop.fitness);
}

void EMO_Population_free(EMO_Population *pop) {
  free(pop->var);
  free(pop->vdummy);
  free(pop->obj);
  free(pop->odummy);

  if(pop->con != NULL) {
    free(pop->con);
    free(pop->cdummy);
  }
}

/* Copy individual j to the current position of individual i */
void EMO_Population_copy(EMO_Population *pop, int *fit1, double *fit2, EMO_MOP *mop, int i, int j) {  //destino i, origen j
   int x, y;

  if(i == j) return;

  // Copy variables
  x = i * mop->nvar;
  y = j * mop->nvar;

  memcpy(pop->var + x, pop->var + y, sizeof(double) * mop->nvar);

  // Copy objectives
  x = i * mop->nobj;
  y = j * mop->nobj;
  memcpy(pop->obj + x, pop->obj + y, sizeof(double) * mop->nobj);

  // Copy constraints
  if(mop->ncon != 0) {
    x = i * mop->ncon;
    y = j * mop->ncon;
    memcpy(pop->con + x, pop->con + y, sizeof(double) * mop->ncon);
  }
  
  if(fit1 != NULL) 
    fit1[i] = fit1[j];

  if(fit2 != NULL) 
    fit2[i] = fit2[j];
}


/* Interchange individual i by j in the population */
void EMO_Population_swap(EMO_Population *pop, int *fit1, double *fit2, EMO_MOP *mop, int i, int j) {
  double dummy;
  int x, y;

  if(i == j) return;

  // Copy variables
  x = i * mop->nvar;
  y = j * mop->nvar;

  memcpy(pop->vdummy,  pop->var + x, sizeof(double) * mop->nvar);
  memcpy(pop->var + x, pop->var + y, sizeof(double) * mop->nvar);
  memcpy(pop->var + y, pop->vdummy,  sizeof(double) * mop->nvar);

  // Copy objectives
  x = i * mop->nobj;
  y = j * mop->nobj;

  memcpy(pop->odummy,  pop->obj + x, sizeof(double) * mop->nobj);
  memcpy(pop->obj + x, pop->obj + y, sizeof(double) * mop->nobj);
  memcpy(pop->obj + y, pop->odummy,  sizeof(double) * mop->nobj);

  // Copy constraints
  if(mop->ncon != 0) {
    x = i * mop->ncon;
    y = j * mop->ncon;

    memcpy(pop->cdummy,  pop->con + x, sizeof(double) * mop->ncon);
    memcpy(pop->con + x, pop->con + y, sizeof(double) * mop->ncon);
    memcpy(pop->con + y, pop->cdummy,  sizeof(double) * mop->ncon);
  }

  if(fit1 != NULL) {
    x = fit1[i];
    fit1[i] = fit1[j];
    fit1[j] = x;
  }

  if(fit2 != NULL) {
    dummy = fit2[i];
    fit2[i] = fit2[j];
    fit2[j] = dummy;
  }
}

/*void EMO_serializeIndividual(double *buff, EMO_Population *pop, EMO_MOP mop, int i) {
  double *pt;

  pt = buff;
  memcpy(pt, pop->var + i * mop.nvar, sizeof(double) * mop.nvar);

  pt += mop.nvar;
  memcpy(pt, pop->obj + i * mop.nobj, sizeof(double) * mop.nobj);

  // Copy constraints
  if(mop.ncon != 0) {
    pt += mop.nobj;
    memcpy(pt, pop->con + i * mop.ncon, sizeof(double) * mop.ncon);
  }
}

void EMO_unserializeIndividual(double *buff, EMO_Population *pop, EMO_MOP mop, int i) {
  double *pt;

  pt = buff;
  memcpy(pop->var + i * mop.nvar, pt, sizeof(double) * mop.nvar);

  pt += mop.nvar;
  memcpy(pop->obj + i * mop.nobj, pt, sizeof(double) * mop.nobj);

  // Copy constraints
  if(mop.ncon != 0) {
    pt += mop.nobj;
    memcpy(pop->con + i * mop.ncon, pt, sizeof(double) * mop.ncon);
  }
}*/


/* Keeps the selected mu individuals, indicated by filter, at the first elements of the mixed population */
void EMO_Population_survive(EMO_Population *pop, int *fit1, double *fit2, EMO_MOP *mop, EMO_List *missing, EMO_List *available, int *filter) {
  int i, j;

  EMO_clear(missing);
  EMO_clear(available);

  for(i = 0; i < pop->size; i++) {
    if(i < pop->mu) { 
      if(filter[i] == 0)
        EMO_queue(available, i);
    }
    else if(filter[i] == 1) {
      EMO_queue(missing, i);
    }
  }

  while(missing->length != 0) {
    i = EMO_dequeue(missing);
    j = EMO_dequeue(available);
    EMO_Population_swap(pop, fit1, fit2, mop, i, j);
  }

  if(missing->length != 0 || available->length != 0) {
    printf("Error in survivalPopulation, missing %d, available %d\n", missing->length, available->length);
    exit(1);
  }
}

#undef _MAXCHAR

