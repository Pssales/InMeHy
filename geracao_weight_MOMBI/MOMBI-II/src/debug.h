
#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>

#define EMO_DEBUG_OFF 0
#define EMO_DEBUG_ON  1

typedef struct {
  unsigned int rank;
  unsigned int level;
  FILE *fp;
  unsigned int line; 
  char *name, buf[30];
} EMO_Debug;
 
void EMO_Debug_alloc(EMO_Debug *dbg, unsigned int level, unsigned int rank, const char *file);
void EMO_Debug_free(EMO_Debug *dbg);
void EMO_Debug_rename(EMO_Debug *dbg, const char *file);
void EMO_Debug_printf(EMO_Debug *dbg, const char *format, ...);
void EMO_Debug_printv(EMO_Debug *dbg, double *x, int n, const char *fmt, ...);

#endif

