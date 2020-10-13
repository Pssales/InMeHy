
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef EMO_MPI
  #include "mpi.h"
#endif

#include "benchmark.h"
#include "param.h"
#include "io.h"

#define MAX_CHAR 1000

void _EMO_Param_to_upper(char *str) {
  int i, n;

  n = strlen(str);

  for(i=0; i < n; i++) {
    str[i] = toupper(str[i]);
  }

  str[n] = '\0';
}

void EMO_Param_alloc(EMO_Param *param, int max_param) {

  param->parser = (EMO_Parser *) malloc(sizeof(EMO_Parser));
  EMO_Parser_alloc(param->parser, NULL, max_param);
 
  param->mop = NULL;
  param->dbg = NULL;
  param->rand = NULL;
  param->stop = NULL;
  param->plot = NULL;
  param->fp = NULL;

  param->prefix = NULL;
  param->flog = NULL;
  param->fsum = NULL;
  param->algorithm = NULL;
  param->subalgorithm = NULL;
}

void EMO_Param_alloc_from_file(EMO_Param *param, EMO_MOP *mop, char *algorithm, char *file, char *problem) {
  double fitness = 0;
  int v, flag, proc;
  char *str;

  param->mop = mop;
  param->parser = (EMO_Parser *) malloc(sizeof(EMO_Parser));
  param->dbg = (EMO_Debug *) malloc(sizeof(EMO_Debug));
  param->rand = (EMO_Rand *) malloc(sizeof(EMO_Rand));
  param->stop = (EMO_Stop *) malloc(sizeof(EMO_Stop));

  param->prefix = (char *) malloc(sizeof(char) * MAX_CHAR);
  param->flog = (char *) malloc(sizeof(char) * MAX_CHAR);
  param->fsum = (char *) malloc(sizeof(char) * MAX_CHAR);
  param->algorithm = (char *) malloc(sizeof(char) * MAX_CHAR);
  param->subalgorithm = (char *) malloc(sizeof(char) * MAX_CHAR);
  str = (char *) malloc(sizeof(char) * MAX_CHAR);

  _EMO_Param_to_upper(algorithm);

  #ifdef EMO_MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &proc);
    sprintf(param->algorithm, "p%s", algorithm);
  #else
    proc = 0;
    strcpy(param->algorithm, algorithm);
  #endif

  sprintf(param->flog, "%s_%s_%d.log", param->algorithm, problem, proc); // random number

  printf("log tmp %s\n", param->flog);
  EMO_Debug_alloc(param->dbg, EMO_DEBUG_ON, proc, param->flog);

  strcpy(param->subalgorithm, algorithm); // required by IBEA

  EMO_Parser_alloc_from_file(param->parser, param->dbg, file);

  if(strcmp(problem, "DEFAULT") != 0 && strcmp(problem, "default") != 0)
    EMO_Benchmark_alloc(mop, param, problem);

  if(!EMO_Parser_get_char(param->parser, str, "output")) {
    printf("Error, output is not defined in the configuration file.\n");
    exit(1);
  }

  #ifdef EMO_MPI
    EMO_Debug_printf(param->dbg, "EMO_MPI is defined");
    sprintf(param->flog, "%s/%s_%d_%s_%.2dD.log", str, param->algorithm, proc, mop->name, mop->nobj);
    sprintf(param->prefix, "%s/%s_%d_%s_%.2dD_R", str, param->algorithm, proc, mop->name, mop->nobj);
    sprintf(param->fsum, "%s/%s_%d_%s_%.2dD.sum", str, param->algorithm, proc, mop->name, mop->nobj);
  #else
    EMO_Debug_printf(param->dbg, "EMO_MPI is not defined");
    sprintf(param->flog, "%s/%s_%s_%.2dD.log", str, param->algorithm, mop->name, mop->nobj);
    sprintf(param->prefix, "%s/%s_%s_%.2dD_R", str, param->algorithm, mop->name, mop->nobj);
    sprintf(param->fsum, "%s/%s_%s_%.2dD.sum", str, param->algorithm, mop->name, mop->nobj);
  #endif

  EMO_Debug_rename(param->dbg, param->flog);

  if(!EMO_Parser_get_char(param->parser, str, "seed")) {
    printf("Error, seed is not defined in the configuration file.\n");
    exit(1);
  }

  EMO_Rand_alloc_from_file(param->rand, param->dbg, str, proc);
  EMO_Stop_alloc(param->stop, mop, param->dbg);
  param->fp = fopen(param->fsum, "w");
  fprintf(param->fp, "#Run|Feval|Fitness|Time (s)\n");

  flag = 0;

  if(EMO_Parser_get_int(param->parser, &v, "feval")) {
    EMO_Stop_set_feval(param->stop, v);
    flag = 1;
  }

  if(EMO_Parser_get_int(param->parser, &v, "time")) { // 10 * 60
    EMO_Stop_set_time(param->stop, v);
    flag = 1;
  }

  if(EMO_Parser_get_double(param->parser, &fitness, "fitness")) {
    EMO_Stop_set_fitness(param->stop, fitness);
    flag = 1;
  }

  if(flag == 0) {
    printf("Error, missing stopping condition in configuration file\n");
    exit(1);
  }

  if(!EMO_Parser_get_int(param->parser, &param->mu, "psize")) {
    printf("Error, mu is not defined in the configuration file.\n");
    exit(1);
  }

  if(!EMO_Parser_get_int(param->parser, &v, "plot")) {
    printf("Error, plot is not defined in the configuration file.\n");
    exit(1);
  }

  if(!EMO_Parser_get_char(param->parser, str, "term")) {
    printf("Error, plot is not defined in the configuration file.\n");
    exit(1);
  }

  #ifdef EMO_MPI
    param->plot = NULL;
  #else
    param->plot = (EMO_Plot *) malloc(sizeof(EMO_Plot));
    EMO_Plot_alloc(param->plot, str, mop->name, "", param->mu, mop->nobj, v);
  #endif

  if(!EMO_Parser_get_int(param->parser, &v, "debug")) {
    printf("Error, debug is not defined in the configuration file.\n");
    exit(1);
  }

  param->dbg->level = (v == EMO_DEBUG_OFF)? v : EMO_DEBUG_ON;

  free(str);
}

void EMO_Param_save(EMO_Param *param, EMO_Population *pop, EMO_MOP *mop, int run) {
  int d;

  EMO_Plot_run(param->plot, pop->obj, mop->feval / pop->mu, 1);

  d = param->dbg->level;
  param->dbg->level = EMO_DEBUG_ON;

  param->stop->fitness = 0;

  fprintf(param->fp, "%d %ld %e %ld.%04ld\n", run, mop->feval, param->stop->fitness, param->stop->total_time.tv_sec, param->stop->total_time.tv_usec);
  fflush(param->fp);

  EMO_Debug_printf(param->dbg, "Total execution time: %lds.%04ld", param->stop->total_time.tv_sec, param->stop->total_time.tv_usec);
  EMO_Debug_printf(param->dbg, "Number of objective function evaluations: %d", mop->feval);
  EMO_Debug_printf(param->dbg, "Performance indicator of the Pareto front: %f", param->stop->fitness);

  param->dbg->level = d;
}

void EMO_Param_set(EMO_Param *param, const char *name, const char *value) {
  EMO_Parser_set(param->parser, name, value);
}

void EMO_Param_free(EMO_Param *param) {

  EMO_Parser_free(param->parser);
  free(param->parser);

  if(param->prefix != NULL) free(param->prefix);
  if(param->flog != NULL) free(param->flog);
  if(param->fsum != NULL) free(param->fsum);
  if(param->algorithm != NULL) free(param->algorithm);
  if(param->subalgorithm != NULL) free(param->subalgorithm);
  if(param->fp != NULL) fclose(param->fp);

  if(param->dbg != NULL) {
    EMO_Debug_free(param->dbg);
    free(param->dbg);
  }

  if(param->dbg != NULL) {
    EMO_Stop_free(param->stop);
    free(param->stop);
  }

  if(param->rand != NULL) {
    EMO_Rand_free(param->rand);
    free(param->rand);
  }

  #ifndef EMO_MPI
    if(param->plot != NULL) {
      EMO_Plot_free(param->plot);
      free(param->plot);
    }
  #endif

  if(param->mop != NULL)
    EMO_Benchmark_free(param->mop);
}

#undef MAX_CHAR

