
#ifndef _PARAM_
#define _PARAM_

#include "common.h"
#include "parser.h"
#include "random.h"
#include "stop.h"
#include "plot.h"

typedef struct {
  EMO_Parser *parser;
  EMO_Debug *dbg;
  EMO_Rand *rand;
  EMO_Stop *stop;
  EMO_Plot *plot;
  EMO_MOP *mop;  // for freeing resources

  char *algorithm;
  char *subalgorithm;
  FILE *fp; // summary
  int mu;  // population size
  double Pc;
  double Pm;
  double Nc;
  double Nm;

  /* Parallel MOEAs */
  EMO_List *immigrant; /* Pointer list of immigrant individuals */
  EMO_List *emigrant;  /* Pointer list of emigrant individuals */
  int nimmigrant;
  int nemigrant;
  int epoch;

  char *prefix;
  char *flog;
  char *fsum;
} EMO_Param;

void EMO_Param_alloc(EMO_Param *param, int max_param);
void EMO_Param_alloc_from_file(EMO_Param *param, EMO_MOP *mop, char *algorithm, char *file, char *problem);
// guardar parametros no poblacion
void EMO_Param_save(EMO_Param *param, EMO_Population *pop, EMO_MOP *mop, int run);
void EMO_Param_free(EMO_Param *param);
void EMO_Param_set(EMO_Param *param, const char *name, const char *value);

#endif

