
#ifndef _stop_H
#define _stop_H

#include <sys/time.h>

#include "debug.h"
#include "common.h"

#define EMO_STOP_EVAL  1
#define EMO_STOP_TIME  1<<1
#define EMO_STOP_FIT   1<<2
#define EMO_STOP_EPOCH 1<<3 

#define EMO_STOP_EVAL_TIME EMO_STOP_EVAL|EMO_STOP_TIME
#define EMO_STOP_EVAL_FIT  EMO_STOP_EVAL|EMO_STOP_FIT
#define EMO_STOP_TIME_FIT  EMO_STOP_TIME|EMO_STOP_FIT
#define EMO_STOP_ALL       EMO_STOP_EVAL|EMO_STOP_TIME|EMO_STOP_FIT

typedef struct {
  int type;               // define the type of stop, by evals, time, fitness, epoch or a combination
  unsigned long *feval, max_feval;             // evaluations
  unsigned long epoch, max_epoch;              // epoch (for pMOEAs)
  struct timeval t1, t2, total_time, max_time; // execution time
  double fitness, max_fitness;                 // indicator value: hv, igd, delta_p
  int interrupt;          // program interruption
  int msg;                // print summary
  EMO_Debug *dbg;         // associated log
} EMO_Stop;

void EMO_Stop_alloc(EMO_Stop *stop, EMO_MOP *mop, EMO_Debug *dbg);
void EMO_Stop_free(EMO_Stop *stop);
void EMO_Stop_start(EMO_Stop *stop);
void EMO_Stop_set_feval(EMO_Stop *stop, unsigned long max_feval);
void EMO_Stop_set_time(EMO_Stop *stop, time_t seconds);
void EMO_Stop_set_fitness(EMO_Stop *stop, double max_fitness);
void EMO_Stop_set_epoch(EMO_Stop *stop, int epoch);
void EMO_Stop_update_epoch(EMO_Stop *stop);
void EMO_Stop_now(EMO_Stop *stop);
int EMO_Stop_end(EMO_Stop *stop);

#endif

