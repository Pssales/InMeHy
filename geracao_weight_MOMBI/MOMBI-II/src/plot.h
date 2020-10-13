
#ifndef _PLOT_H
#define _PLOT_H

#include <stdio.h>

#define GNUPLOT_COMMAND "gnuplot -persist 1>gnuplot.out 2>&1"
#define TERM "x11" /* wxt */

#define EMO_PLOT_NONE 0
#define EMO_PLOT_ITERATIVE 1
#define EMO_PLOT_END 2

typedef struct {
  FILE **gp;
  char *term;
  char *fname;
  char *pftrue;
  int row;
  int col;
  int enable;
} EMO_Plot;

void EMO_Plot_alloc(EMO_Plot *p, const char *term, const char *fname, const char *pftrue, int row, int col, int enable);
void EMO_Plot_free(EMO_Plot *p);
void EMO_Plot_run(EMO_Plot *p, double *data, int gen, int enable);

#endif

