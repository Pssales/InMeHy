/* genetico.h: Implementacion de algoritmo genetico.
 *             Definicion de parametros y prototipos de funcion.
 *
 * Seccion de Computacion - CINVESTAV
 *
 * Introduccion a la Computacion Evolutiva
 * Profesor: Carlos Coello Coello
 * Autor:    Raquel Hernandez Gomez
 * Julio 2012
 */

#include "list.h"

/* Codigo de errores */
#define MEMORY 1
#define OPEN   2

#define GNUPLOT_COMMAND "gnuplot -persist 1>gnuplot.out 2>&1"
#define TERM "wxt" //"x11"
#define MAX_CHAR 5000
#define POS 1
#define POF  2

typedef struct {
  double *x;     // Decision variables
  double *f;     // Vector of objective function evaluations
  double tch;
  int rank;
} Individual;

typedef void (*Func)(double **, double *, int, int, ...);  // Objective function


Individual *pop; // Population (mu + lambda)

int Psize;       // Population size
int Gmax;        // Maximum number of generations
char *fname;     // Objective function name
void (*f)(double **, double *, int, int, ...);  // Objective function
int N;           // Number of decision variables
int M;           // Number of objective functions
int K;           // Position-related parameter (WFG test suite)
double **range;  // Range of decision variables 
                 //   inf: range[*][0], sup: range[*][1] 
float Rseed;
int Wsize;       // Number of weight vectors
char *wfile;     // Weight vector file
double **W;      // Set of weight vectors
double Pc;       // Crossover probability
double Nc;       // Crossover distribution index
double Pm;       // Mutation probability
double Nm;       // Mutation distribution index
char *str;  // Output file 
int debug;       // Prints out detailes of the program
int plot;        // Plots for 2 or 3 objectives
char *pftrue;    // True Pareto front
int sample;      // Number of executions of the program

int gen;         // Current generation
int ncross;      // Counter of crossovers
int nmut;        // Counter of mutations
int feval;       // Number of objective function evaluations

double **wmax;
double *Ref;
double *f_min;
double *f_max;

int *seedp;
int *parent1;
int *parent2;

double **sort;
double prob[2000];
int ndl[2000];

List available, missing;
int *wrank;

FILE **gp;

/* Prototipos de funcion */
int compareNumber(const void **a, const void **b); 
int compare_individual(const void **, const void **);

void reducePopulation(Individual **pop, List *available, List *missing, double ***sort, int size, int mu, int n, int m); 

void memory_allocation();
void free_memory();
void initialize_pop();
void scale(int psize);
void initW(char *nom, double ***W, int Wsize, int Dim);
void shuffle(int n, int **idx);
void seleccion_torneo(int **ind);
void seleccion_cruza(int **ind);
double R2(Individual *pop, int T, double **W, int Wsize, double *r, int Dim);
void realcross (Individual *child1, Individual *child2, int *ncross, Individual parent1, Individual parent2, double **range, int n, double pc, double eta_c); 
void real_mutate (Individual *ind, int *nmut, int n, double **range, double pm, double eta_m); 
void eval(Individual *ind);
void copy(Individual *dest, Individual src, int n, int m); 
void save(char *nom, int type, Individual *pop, int fil, int col); 
void graphic (FILE **gp, Individual *pop, int psize, int dim, char *pftrue, char *fname, int gen); 
void printIndividual(Individual ind, int idx, int n, int m); 
double norm(double *a, int n);
double getClock(const char *s); 
void parameter(char *name); 
void summary(char *name, int run,  float seed, int feval, int ncross, int nmut, double time); 
 
