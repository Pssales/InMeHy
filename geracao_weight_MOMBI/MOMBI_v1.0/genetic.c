#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#include "parameter.h"
#include "genetic.h"
#include "function.h"
#include "randKnuth.h"
#include "wfg.h"
#include "error.h"

void memory_allocation() {
  int i, n, t;

  if(Psize % 2 != 0) {
    printf("Error, population size must be even: %d.\n", Psize);
    exit(1);
  }

  n = Psize * 2;

  initList(&available, Psize);
  initList(&missing, Psize);

  fname =  (char *) malloc(sizeof(char) * MAX_CHAR);
  wfile =  (char *) malloc(sizeof(char) * MAX_CHAR);
  str = (char *) malloc(sizeof(char) * MAX_CHAR);
  pftrue = (char *) malloc(sizeof(char) * MAX_CHAR);

  wrank  = (int *) malloc(sizeof(int) * Wsize);

  pop = (Individual *) malloc(sizeof(Individual) * n);
 
  for(i = 0; i < n; i++) {
    pop[i].x = (double *) malloc(sizeof(double) * N);
    pop[i].f = (double *) malloc(sizeof(double) * M);
  }

  W = (double **) malloc(sizeof(double *) * Wsize);
  wmax = (double **) malloc(sizeof(double *) * Wsize);

  for(i = 0; i < Wsize; i++) {
    W[i] = (double *) malloc(sizeof(double) * M);
    wmax[i] = (double *) malloc(sizeof(double) * n);
  }

  range = (double **) malloc(sizeof(double *) * 2);

  for(i = 0; i < 2; i++)
    range[i] = (double *) malloc(sizeof(double) * N);

  Ref = (double *) malloc(sizeof(double) * M);

  f_min = (double *) malloc(sizeof(double) * M);
  f_max = (double *) malloc(sizeof(double) * M);

  seedp = (int *) malloc(sizeof(int)*Psize);
  for(i = 0; i < Psize; i++)  /* Initializes permutation array */
    seedp[i] = i;

  parent1 = (int *) malloc(sizeof(int)*Psize/2);
  parent2 = (int *) malloc(sizeof(int)*Psize/2);

  if(plot) {
    t = (M < 4)? 2 : 1;

    if( (gp = (FILE **) malloc(sizeof(FILE *) * t)) == NULL)
      error(ENOMEM, "gnuplot", "genetic.c");

    for(i = 0; i < t; i++) {
      if( (gp[i] = popen(GNUPLOT_COMMAND, "w")) == NULL)
        error(ENOFILE, "gnuplot[i]", "genetic.c");
    }
  }

  if( (sort = (double **) malloc(sizeof(double *) * n)) == NULL) {
    printf("error al asignar memoria en sort\n");
    exit(1);
  }

  for(i = 0; i < n; i++) {
    if( (sort[i] = (double *) malloc(sizeof(double) * 2)) == NULL) {
      printf("error al asignar memoria en sort 2\n");
      exit(1);
    }
  }
}

void free_memory() {
  int i, n = 2*Psize, t;

  freeList(&available);
  freeList(&missing);
  free(wrank);

  free(fname);
  free(wfile);
  free(str);
  free(pftrue);

  for(i = 0; i < n; i++) {
    free(pop[i].x);
    free(pop[i].f);  
  }
  free(pop);

  for(i = 0; i < Wsize; i++) {
    free(W[i]);
    free(wmax[i]);
  }
  free(wmax);
  free(W);

  for(i = 0; i < 2; i++)
    free(range[i]);

  free(range);
  free(Ref);
  free(f_min);
  free(f_max);
  free(seedp);
  free(parent1);
  free(parent2);

 if(plot) {
    t = (M < 4)? 2:1;

    for(i = 0; i < t; i++)
      fclose(gp[i]);

    free(gp);
  }

  for(i = 0; i < n; i++)
    free(sort[i]);

  free(sort);
}

void initialize_pop() {
  int i, j;

  for(i=0; i < Psize; i++) {
    for(j=0; j < N; j++) 
      pop[i].x[j] = rndreal(range[0][j], range[1][j]);

    pop[i].rank = INT_MAX;
    pop[i].tch = DBL_MAX;
    eval(&pop[i]); 
  }
}

/* Reference points */
void scale(int psize) {
  int i, k; 

  for(k = 0; k < M; k++) {
    f_min[k] =  DBL_MAX;
    f_max[k] = -DBL_MAX;
  }
 
  for(i = 0; i < psize; i++) {
    for(k = 0; k < M; k++) {

      if(pop[i].f[k] < f_min[k])
        f_min[k] = pop[i].f[k];
      else if(pop[i].f[k] > f_max[k])
        f_max[k] = pop[i].f[k];
    }
  }
}

void initW(char *nom, double ***W, int Wsize, int Dim) {
  FILE *fp;
  int i, j;

  if((fp = fopen(nom, "r")) == NULL) {
    printf("Error al abrir archivo de pesos %s.\n", nom);
    exit(1);
  }

  for(i=0; i < Wsize; i++) {
    for(j=0; j < Dim; j++) {
      if(fscanf(fp, "%lf", &(*W)[i][j]) == EOF) {
        printf("se produjo error al leer archivo de pesos\n");
        exit(1);
      }

      if((*W)[i][j] == 0)
        (*W)[i][j] = 1e-4;
    }
  }
  fclose(fp);
}

/* Fisher-Yates shuffle algorithm for generating 
   number permutations. Used in tournament selection */
void shuffle(int n, int **idx) {
  int i, r, swap;

  for(i = n - 1; i > 0; i--) {
    r = rnd(0, i);
    swap = (*idx)[i];
    (*idx)[i] = (*idx)[r];
    (*idx)[r] = swap;
  }
}

/* Tournament selection */
void seleccion_torneo(int **ind) {
  int p1, p2, i, j = 0;

  shuffle(Psize, &seedp);

  for(i = 0; i < Psize; i+=2) {
    p1 = seedp[i];
    p2 = seedp[i+1];

    if(pop[p1].rank < pop[p2].rank)
       (*ind)[j++] = p1;
    else if(pop[p2].rank < pop[p1].rank)
      (*ind)[j++] = p2;
    else {
      if (rndreal(0,1) <= 0.5)
        (*ind)[j++] = p1;
      else
        (*ind)[j++] = p2;
    }
  }
}


double tchebycheff(double *w, double *x, double *ideal, double *nadir, int dim){
  double t, max = 0;
  int i;

  for(i = 0; i < dim; i++) {
    t = w[i] * fabs(x[i] - ideal[i]) / fabs(nadir[i] - ideal[i]);  
    if(t > max)
      max = t;
  }
  return max;
}

void ranking(Individual **pop, int Size) {
  int i, j, k;

  for(i = 0; i < Size; i++) {
    (*pop)[i].rank = INT_MAX;
    (*pop)[i].tch = DBL_MAX;
  }

  for(i = 0; i < Wsize; i++) {
    // Contribution of the individuals to one weight vector
    for(j = 0; j < Size; j++) {

      sort[j][0] = tchebycheff(W[i], (*pop)[j].f, f_min, f_max, M);
      sort[j][1] = j;

      if(sort[j][0] < (*pop)[j].tch)
        (*pop)[j].tch = sort[j][0];
    }

    // Sort individuals
    qsort(sort, Size, sizeof(sort[0]), (int (*)(const void *, const void *))&compare_individual);

    // Hierarchization 
    for(j = 1; j <= Size; j++) {
      k = (int) sort[j-1][1]; 

      if(j < (*pop)[k].rank) {  // It improves the individual's rank
        (*pop)[k].rank = j;
      }
    }
  }
}

void reducePopulation(Individual **pop, List *available, List *missing, double ***sort, int size, int mu, int n, int m) {
  int i, j;

  for(i = 0; i < size; i++) {
    (*sort)[i][0] = i; 
    (*sort)[i][1] = (*pop)[i].rank;
  }

  // Sorts individuals with respect to their rank, best utility function obtained and the L_1 norm
  qsort(*sort, size, sizeof((*sort)[0]), (int (*)(const void *, const void *))&compareNumber);

  clear(available);

  for(i = 0; i < size; i++) {
    j = (int) (*sort)[i][0]; 

    if(i < mu) {
      if(j >= mu)
        queue(missing, j); 
    }
    else {
      if(j < mu)
        add(available, j, 0);
    }
  } 

  while(missing->length != 0) {
    dequeue(missing, &i);
    dequeue(available, &j);
    copy(&(*pop)[j], (*pop)[i], n, m); 
  }
}

/* Routine for real variable SBX crossover */
void realcross (Individual *child1, Individual *child2, int *ncross, Individual parent1, Individual parent2, double **range, int n, double pc, double eta_c) {
  int i;
  double rand;
  double y1, y2, yl, yu;
  double c1, c2;
  double alpha, beta, betaq;

  if (rndreal(0, 1) <= pc) {
    (*ncross)++;

    for (i=0; i < n; i++) {

      if (rndreal(0, 1) <= 0.5 ) {

        /* significant difference between parents */
        if (fabs(parent1.x[i]-parent2.x[i]) > 1e-14) {

          if (parent1.x[i] < parent2.x[i]) {
            y1 = parent1.x[i];
            y2 = parent2.x[i];
          }
          else {
            y1 = parent2.x[i];
            y2 = parent1.x[i];
          }
          yl = range[0][i];
          yu = range[1][i];
          rand = rndreal(0, 1);
          beta = 1.0 + (2.0*(y1-yl)/(y2-y1));
          alpha = 2.0 - pow(beta,-(eta_c+1.0));

          if (rand <= (1.0/alpha))
            betaq = pow ((rand*alpha),(1.0/(eta_c+1.0)));
          else
            betaq = pow ((1.0/(2.0 - rand*alpha)),(1.0/(eta_c+1.0)));

          c1 = 0.5*((y1+y2)-betaq*(y2-y1));
          beta = 1.0 + (2.0*(yu-y2)/(y2-y1));
          alpha = 2.0 - pow(beta,-(eta_c+1.0));

          if (rand <= (1.0/alpha))
            betaq = pow ((rand*alpha),(1.0/(eta_c+1.0)));
          else
             betaq = pow ((1.0/(2.0 - rand*alpha)),(1.0/(eta_c+1.0)));

          c2 = 0.5*((y1+y2)+betaq*(y2-y1));

          if (c1<yl) c1=yl;

          if (c2<yl) c2=yl;

          if (c1>yu) c1=yu;

          if (c2>yu) c2=yu;

          if (rndreal(0, 1)<=0.5) {
            child1->x[i] = c2;
            child2->x[i] = c1;
          }
          else {
            child1->x[i] = c1;
            child2->x[i] = c2;
          }
        }
        else {
          child1->x[i] = parent1.x[i];
          child2->x[i] = parent2.x[i];
        }
      }
      else {
        child1->x[i] = parent1.x[i];
        child2->x[i] = parent2.x[i];
      }
    }
  }
  else {
    for (i=0; i < n; i++) {
      child1->x[i] = parent1.x[i];
      child2->x[i] = parent2.x[i];
    }
  }
}

/* Routine for real polynomial mutation of an individual */
void real_mutate (Individual *ind, int *nmut, int n, double **range, double pm, double eta_m) {
  int j, flag = 0;
  double rnd, delta1, delta2, mut_pow, deltaq;
  double y, yl, yu, val, xy;

  for (j=0; j < n; j++) {
    if (rndreal(0, 1) <= pm) {

      y = ind->x[j];
      yl = range[0][j];
      yu = range[1][j];
      delta1 = (y-yl)/(yu-yl);
      delta2 = (yu-y)/(yu-yl);
      rnd = rndreal(0, 1);
      mut_pow = 1.0/(eta_m+1.0);

      if (rnd <= 0.5) {
        xy = 1.0-delta1;
        val = 2.0*rnd+(1.0-2.0*rnd)*(pow(xy,(eta_m+1.0)));
        deltaq =  pow(val,mut_pow) - 1.0;
      }
      else {
        xy = 1.0-delta2;
        val = 2.0*(1.0-rnd)+2.0*(rnd-0.5)*(pow(xy,(eta_m+1.0)));
        deltaq = 1.0 - (pow(val,mut_pow));
      }

      y = y + deltaq*(yu-yl);

      if (y<yl) y = yl;
      if (y>yu) y = yu;

      ind->x[j] = y;
      flag = 1;
    }
  }
  if(flag) (*nmut)++;
}

void eval(Individual *ind) {
  f(&ind->f, ind->x, M, N, K);
  feval++;
}

/* Copies the information of an individual */
void copy(Individual *dest, Individual src, int n, int m) {

  memcpy(dest->x, src.x, sizeof(double) * n);
  memcpy(dest->f, src.f, sizeof(double) * m);

  dest->tch = src.tch;
  dest->rank = src.rank;
}

void save(char *nom, int type, Individual *pop, int fil, int col) {
  FILE *fp;
  int i, j;

  fp = fopen(nom, "w");

  if(type == POS) {

    fprintf(fp, "# %d %d\n", fil, col);

    for(i = 0; i < fil; i++) {
        for(j = 0; j < col; j++)
          fprintf(fp, "%e ", pop[i].x[j]);
        fprintf(fp, "\n");
    }
  }
  else if(type == POF) {
    fprintf(fp, "# %d %d\n", fil, col);

    for(i = 0; i < fil; i++) {
        for(j = 0; j < col; j++)
          fprintf(fp, "%e ", pop[i].f[j]);
 
        fprintf(fp, "\n");
    }
  }

  fclose(fp);
}


/* Function to display the current population */
void graphic (FILE **gp, Individual *pop, int psize, int dim, char *pftrue, char *fname, int gen) {

  if(gen % 5 != 0 && gen != Gmax)  /* Skips */
    return; 

  save("plot.out", POF, pop, psize, dim);

  /* Parallel coordinates */
  if(dim > 3 || gen == Gmax) {
    sprintf(str, "set title '%s_%dD - Generation #%d'\nunset key\nset xlabel 'Objective'\nset ylabel 'Value'\n", fname, dim, gen);
    sprintf(str, "%splot [1:%d] \"<awk '$1 !~ /#/ { for(i = 1; i <= NF; i++) print i,$i; print z}' 'plot.out'\" u 1:2:xtic(1) w linesp linecolor 5", str, dim);
    fprintf(gp[0], "%s\n", str);
    fflush(gp[0]);
  }

  if(dim == 2) {
      sprintf(str, "set term %s\nset title '%s_%dD - Generation #%d'\nunset key\nset xlabel 'f1'\nset ylabel 'f2'\nplot ", TERM, fname, dim, gen);

      if(strlen(pftrue) != 0) {/* Plots Pareto front true */
        sprintf(str, "%s'%s' w points pointtype 2 linecolor 5, ", str, pftrue);
      }

      sprintf(str, "%s'plot.out' w points pointtype 6 pointsize 1 linecolor 4\n", str);

      fprintf(gp[1], "%s", str);
      fflush(gp[1]);
  }
  else if(dim == 3) {
      sprintf(str, "set term %s\nset title '%s_%dD - Generation #%d'\nunset key\nset xlabel 'f1'\nset ylabel 'f2'\nset zlabel 'f3'\nset view 30,30\nsplot ", TERM, fname, dim, gen);

      if(strlen(pftrue) != 0) { /* Plots Pareto front true */
        sprintf(str, "%s'%s' w points pointtype 2 linecolor 5, ", str, pftrue);
      } 
      sprintf(str, "%s'plot.out' w points pointtype 6 pointsize 1 linecolor 4\n", str);
      fprintf(gp[1], "%s", str);
      fflush(gp[1]);
  }
}


double norm(double *a, int n) {
  int i;
  double na = 0;

  for(i=0; i < n; i++) 
    na += fabs(a[i]);

  return na;
}

int compare_individual(const void **a, const void **b) {
  double *v1, *v2, n1, n2;
  int i, j;

  v1 = (double *) *a;
  v2 = (double *) *b;

  if(*v1 < *v2) return -1;
  else if(*v1 > *v2) return 1;
 
  i = (int) v1[1];
  j = (int) v2[1];
  n1 = norm(pop[i].f, M);
  n2 = norm(pop[j].f, M);

  if(n1 < n2) 
    return -1; 
  else if(n1 > n2) 
    return 1; 
  return 0;
}

int compareNumber(const void **a, const void **b) {
  double *v1, *v2, n1, n2;
  int i, j;

  v1 = (double *) *a;
  v2 = (double *) *b;

  if(v1[1] < v2[1]) return -1;
  else if(v1[1] > v2[1]) return 1;

  i = (int) v1[0];
  j = (int) v2[0];
 
  if(pop[i].tch < pop[j].tch)
    return -1;
  else if(pop[j].tch > pop[i].tch)
    return 1;

  n1 = norm(pop[i].f, M);
  n2 = norm(pop[j].f, M);

  if(n1 < n2) 
    return -1; 
  else if(n1 > n2) 
    return 1; 

  return 0;
}

double getClock(const char *s) {
  static struct timeval start, begin;
  struct timeval end;
  double elapsed;

  if(strcmp(s, "init") == 0) {
    gettimeofday(&begin, NULL);
    gettimeofday(&start, NULL);
    return 0;
  }

  gettimeofday(&end, NULL);
  elapsed = (end.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
  elapsed += (end.tv_usec - start.tv_usec) / 1000.0;   // us to ms

  if(strcmp(s, "end") == 0) {
    elapsed = (end.tv_sec - begin.tv_sec) * 1000.0;      // sec to ms
    elapsed += (end.tv_usec - begin.tv_usec) / 1000.0;   // us to ms
  }
  start = end;
  return elapsed;
}


void parameter(char *name) {
  FILE *fp;
  int i;

  if((fp = fopen(name, "w")) == NULL) {
    printf("Error to open file %s.\n", name);
    exit(1);
  }

  fprintf(fp, "Psize     = %d\n", Psize);
  fprintf(fp, "Gmax      = %d\n", Gmax);
  fprintf(fp, "Function  = %s\n", fname);
  fprintf(fp, "N         = %d\n", N);
  fprintf(fp, "M         = %d\n", M);

  for(i = 0; i < N; i++)
    fprintf(fp, "Range x%.2d = [%f, %f]\n", (i+1), range[0][i], range[1][i]);

  fprintf(fp, "Wsize     = %d\n", Wsize);
  fprintf(fp, "Wfile     = %s\n", wfile);
  fprintf(fp, "Pc        = %f\n", Pc);
  fprintf(fp, "Nc        = %f\n", Nc);
  fprintf(fp, "Pm        = %f\n", Pm);
  fprintf(fp, "Nm        = %f\n", Nm);
  fprintf(fp, "Debug     = %d\n", debug);
  fprintf(fp, "Plot      = %d\n", plot);
  fprintf(fp, "PFTrue    = %s\n", pftrue);
  fprintf(fp, "Sample    = %d\n\n", sample);
  fclose(fp);
}

void summary(char *name, int run,  float seed, int feval, int ncross, int nmut, double elapsed) {
  FILE *fp;

  if((fp = fopen(name, "a")) == NULL) {
    printf("Error to open file %s.\n", name);
    exit(1);
  }

  fprintf(fp, "Run       = %d\n", run);
  fprintf(fp, "Rseed     = %f\n", seed);
  fprintf(fp, "Evaluations              = %d\n", feval);
  fprintf(fp, "Number of crossovers     = %d\n", ncross);
  fprintf(fp, "Number of mutations      = %d\n", nmut);
  fprintf(fp, "Elapsed time             = %f (ms)\n", elapsed);
  fprintf(fp, "--------------------------------------------\n");
  fclose(fp);
}

void run(int r) {
  int i, j;
  double elapsed;

  getClock("init"); 
 
  ncross = nmut = feval = 0;

  initialize_pop();
  scale(Psize);
  ranking(&pop, Psize); 

  for(gen = 1; gen < Gmax; gen++) {

    if(plot) graphic(gp, pop, Psize, M, pftrue, fname, gen);
 
    i = j = 0;
    seleccion_torneo(&parent1);
    seleccion_torneo(&parent2);

    do {
      realcross(&pop[i+Psize], &pop[i+Psize+1], &ncross, pop[parent1[j]], pop[parent2[j]], range, N, Pc, Nc);

      real_mutate(&pop[Psize + i], &nmut, N, range, Pm, Nm);
      real_mutate(&pop[Psize + i + 1], &nmut, N, range, Pm, Nm);
 
      eval(&pop[Psize + i]);
      eval(&pop[Psize + i + 1]);
 
      i += 2;
      j++;
    } while (i < Psize);

    scale(Psize*2);
    ranking(&pop, Psize*2); 

    reducePopulation(&pop, &available, &missing, &sort, Psize * 2, Psize, N, M);
  }
  elapsed = getClock("end"); 
 
  if(plot) graphic(gp, pop, Psize, M, pftrue, fname, gen);

  sprintf(str, "output/MOMBI_%s_%.2dD_R%.2d.pos", fname, M, r);
  save(str, POS, pop, Psize, N);
 
  sprintf(str, "output/MOMBI_%s_%.2dD_R%.2d.pf", fname, M, r);
  save(str, POF, pop, Psize, M);
 
  sprintf(str, "output/MOMBI_%s_%.2dD.sum", fname, M);
  summary(str, r, Rseed, feval, ncross, nmut, elapsed);
}

int main(int argc, char *argv[]) {
  int i;

  if(argc != 2 && argc != 3) {
    printf("Syntax: ./genetic parameter_file_name [#runs]\n");
    exit(1);
  }

  if(argc == 3) 
    sample = atoi(argv[2]);
  else
    sample = 1;

  // Initializes random seed
  srand(time(0));

  // Obtains parameters
  read_param(argv[1]); 
  getInt("Psize", &Psize);
  getInt("Gmax", &Gmax);

  getInt("N", &N);
  getInt("M", &M);
  getInt("Wsize", &Wsize);
  getDouble("Pc", &Pc);
  getDouble("Nc", &Nc);
  getDouble("Pm", &Pm); 
  if(Pm == -1) 
    Pm = 1/(double) N;
  getDouble("Nm", &Nm);
  getInt("Debug", &debug);
  getInt("Plot", &plot);
  memory_allocation();

  getArrayDouble("Rmin", range[0], N);
  getArrayDouble("Rmax", range[1], N);
  getString("Function", &fname);
 
  if(strncmp(fname, "WFG", 3) == 0) {
    getInt("K", &K);
    init_WFG(fname[3] - '0', M, N, K);
  }

  findfunc(fname, &f);
  getString("Wfile", &wfile);
  getString("PFTrue", &pftrue);
  initW(wfile, &W, Wsize, M);

  sprintf(str, "output/MOMBI_%s_%.2dD.sum", fname, M);
  parameter(str);

  for(i=1; i <= sample; i++) {
    Rseed = (double) rand() / RAND_MAX;
    randomize(Rseed);

    run(i);
  }

  if(strncmp(fname, "WFG", 3) == 0) 
    free_WFG();

  free_param();
  free_memory();

  return 0;
}

