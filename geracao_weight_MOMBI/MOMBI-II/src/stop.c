
#include <stdlib.h>

#include "stop.h"
#include "debug.h"

#ifdef EMO_MPI
  #include "mpi.h"
#endif

void get_time(struct timeval *t) {
  #ifdef EMO_MPI
  t->tv_sec = (time_t) MPI_Wtime();
  t->tv_usec = 0;
  #else
  gettimeofday(t, NULL);
  #endif
}

void EMO_Stop_alloc(EMO_Stop *stop, EMO_MOP *mop, EMO_Debug *dbg) {
  stop->dbg = dbg;
  stop->type = 0;
  stop->max_feval = 0;
  stop->max_epoch = 0;
  stop->max_fitness = 0;
  timerclear(&stop->max_time);
  stop->feval = &mop->feval;
  *(stop->feval) = 0;
  EMO_Stop_start(stop);

}

void EMO_Stop_free(EMO_Stop *stop) {
  stop->dbg = NULL;
}

void EMO_Stop_start(EMO_Stop *stop) {

  stop->epoch = 0;
  *(stop->feval) = 0;
  stop->fitness = 0;
  stop->interrupt = 0;
  stop->msg = 0;
  timerclear(&stop->total_time);
  get_time(&stop->t1);
}

void EMO_Stop_set_feval(EMO_Stop *stop, unsigned long max_feval) {
  stop->type |= EMO_STOP_EVAL;
  stop->max_feval = max_feval;
}

void EMO_Stop_set_time(EMO_Stop *stop, time_t seconds) {
  stop->type |= EMO_STOP_TIME;
  stop->max_time.tv_sec = seconds;
  stop->max_time.tv_usec = 0;
}

void EMO_Stop_set_fitness(EMO_Stop *stop, double max_fitness) {
  stop->type |= EMO_STOP_FIT;
  stop->max_fitness = max_fitness;
}

void EMO_Stop_set_epoch(EMO_Stop *stop, int epoch) {
  stop->type |= EMO_STOP_EPOCH;
  stop->max_epoch = epoch;
}

void EMO_Stop_update_epoch(EMO_Stop *stop) {
  if(stop->feval == NULL)
    EMO_Debug_printf(stop->dbg, "Error while updating epoch in stopping condition");
  else
    stop->epoch = (int) *(stop->feval) / stop->max_epoch;
}

void EMO_Stop_now(EMO_Stop *stop) {
  stop->interrupt = stop->msg = 1;
  EMO_Debug_printf(stop->dbg, "Program stop now!");
}

int EMO_Stop_end(EMO_Stop *stop) {
  int new_epoch, r = 0;

  get_time(&stop->t2);
  timersub(&stop->t2, &stop->t1, &stop->total_time);

  if(stop->interrupt) {
    EMO_Debug_printf(stop->dbg, "Program stopped: the interrupt flag is activated");
    r = 1;
  }
  else {
    if(((stop->type)&(EMO_STOP_EVAL)) == EMO_STOP_EVAL && *(stop->feval) >= stop->max_feval) {
      stop->interrupt = stop->msg = r = 1;
      EMO_Debug_printf(stop->dbg, "Program stopped: maximum number of objective function evaluations reached (%d)", stop->max_feval);
    }

    if(((stop->type)&(EMO_STOP_TIME)) == EMO_STOP_TIME && stop->max_time.tv_sec != 0 && stop->total_time.tv_sec >= stop->max_time.tv_sec) {
      stop->interrupt = stop->msg = r = 1;
      EMO_Debug_printf(stop->dbg, "Program stopped: maximum execution time reached (%lds.%04ld)", stop->max_time.tv_sec, stop->max_time.tv_usec);
    }

    if(((stop->type)&(EMO_STOP_FIT)) == EMO_STOP_FIT && stop->fitness >= stop->max_fitness) {
      stop->interrupt = stop->msg = r = 1;
      EMO_Debug_printf(stop->dbg, "Program stopped: maximum fitness reached (%f)", stop->max_fitness);
    }

    if(((stop->type)&(EMO_STOP_EPOCH)) == EMO_STOP_EPOCH) {
      new_epoch = (int) *(stop->feval) / stop->max_epoch;

      if(new_epoch != stop->epoch) {
        stop->epoch = new_epoch;
        stop->msg = r = 1;
        EMO_Debug_printf(stop->dbg, "Program stopped: epoch achieved (%ld/%d)", *(stop->feval), stop->max_epoch);
      }
    }
  }

  if(stop->msg) {
    EMO_Debug_printf(stop->dbg, "Real execution time: %lds.%06ld", stop->total_time.tv_sec, stop->total_time.tv_usec);
    EMO_Debug_printf(stop->dbg, "Objective function evaluations: %ld", *(stop->feval));
    EMO_Debug_printf(stop->dbg, "Fitness value: %f", stop->fitness);
    stop->msg = 0;
  }

  return r;
}

