
#include "common.h"
#include "param.h"
#include "mombi2.h"

#ifndef _MOEA_H
#define _MOEA_H

/* Generic MOEA */
typedef void (*EMO_MOEA_falloc)(void *, EMO_Param *, EMO_Population *, EMO_MOP *);
typedef void (*EMO_MOEA_ffree)(void *);
typedef void (*EMO_MOEA_frun)(void *, EMO_Param *, EMO_Population *, EMO_MOP *);
typedef void (*EMO_MOEA_fprun)(void *, EMO_Param *, EMO_Population *, EMO_MOP *);

typedef struct {
  EMO_MOEA_falloc alloc;
  EMO_MOEA_ffree free;
  EMO_MOEA_frun run;
  EMO_MOEA_fprun prun;

  union {
    EMO_MOMBI2 mombi2;
  } alg;

  void *palg;
} EMO_MOEA;


void EMO_MOEA_alloc(EMO_MOEA *moea, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop, const char *str);
void EMO_MOEA_free(EMO_MOEA *moea);
void EMO_MOEA_run(EMO_MOEA *moea, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop);
void EMO_MOEA_prun(EMO_MOEA *moea, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop);

#endif

