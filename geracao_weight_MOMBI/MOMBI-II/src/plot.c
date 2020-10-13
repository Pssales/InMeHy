
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "plot.h"
#include "io.h"

void EMO_Plot_alloc(EMO_Plot *p, const char *term, const char *fname, const char *pftrue, int row, int col, int enable) {
  int i, t;

  p->enable = enable;

  if(!p->enable) return;

  p->term = (char *) malloc(sizeof(char) * 20);
  p->fname = (char *) malloc(sizeof(char) * 200);
  p->pftrue = (char *) malloc(sizeof(char) * 200);
  p->row = row;
  p->col = col;

  strcpy(p->term, term);
  strcpy(p->fname, fname);
  strcpy(p->pftrue, pftrue);

  t = 2;  /* Number of plots */
  p->gp = (FILE **) malloc(sizeof(FILE *) * t);

  for(i = 0; i < t; i++)
    p->gp[i] = popen(GNUPLOT_COMMAND, "w");
} 

void EMO_Plot_free(EMO_Plot *p) {
  int i, t;

  if(p == NULL || !p->enable) return;

  free(p->term);
  free(p->fname);
  free(p->pftrue);

  t = 2;

  for(i = 0; i < t; i++)
    fclose(p->gp[i]);

  free(p->gp);
}

/* Plots the current population */
void EMO_Plot_run(EMO_Plot *p, double *data, int gen, int enable) {

  if(p == NULL || p->enable == EMO_PLOT_NONE || (p->enable == EMO_PLOT_END && !enable)) return;

  if(gen % 5 != 0 && !enable)  /* Skips */
    return; 

  EMO_File_write(data, NULL, p->row, p->col, "plot.out", "%f ");
 
  /* Parallel coordinates */
  if(1) {
    fprintf(p->gp[0], "set title '%s_%dD - Generation #%d'\nunset key\nset xlabel 'Objective'\nset ylabel 'Value'\n", p->fname, p->col, gen);
    fprintf(p->gp[0], "plot [1:%d] \"<awk '$1 !~ /#/ { for(i = 1; i <= NF; i++) print i,$i; print z}' ./plot.out\" u 1:2:xtic(1) w linesp linecolor 5\n", p->col);
    fflush(p->gp[0]);
  }

  if(p->col == 2) {
    fprintf(p->gp[1], "set term %s\nset title '%s_%dD - Generation #%d'\nunset key\nset xlabel 'f1'\nset ylabel 'f2'\nplot ", p->term, p->fname, p->col, gen);

      if(strlen(p->pftrue) != 0) {/* Plots Pareto front true */
        if(p->pftrue[0] == '\'') /* from file */
          fprintf(p->gp[1], "%s w points pointtype 2 linecolor 5, ", p->pftrue);
        else                  /* from expresion */
          fprintf(p->gp[1], "%s with lines linestyle 1 linecolor 3, ", p->pftrue);
      }

      fprintf(p->gp[1], "'plot.out' w points pointtype 6 pointsize 1 linecolor 4\n");
      fflush(p->gp[1]);
  }
  else if(p->col == 3) {
      fprintf(p->gp[1], "set term %s\nset title '%s_%dD - Generation #%d'\nunset key\nset xlabel 'f1'\nset ylabel 'f2'\nset zlabel 'f3'\nset view 30,30\nsplot ", p->term, p->fname, p->col, gen);

      if(strlen(p->pftrue) != 0) { /* Plots Pareto front true */
        if(p->pftrue[0] == '\'') { /* from file */
          fprintf(p->gp[1], "%s w points pointtype 2 linecolor 5, ", p->pftrue);
        }
        else {                /* from parametric expresion */
          fprintf(p->gp[1], "set parametric\nsplot %s w l linecolor 5, ", p->pftrue);
        }
      } 
      fprintf(p->gp[1], "'plot.out' w points pointtype 6 pointsize 1 linecolor 4\n");
      fflush(p->gp[1]);
  }
}

