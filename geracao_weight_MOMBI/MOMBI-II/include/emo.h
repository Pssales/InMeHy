
#ifndef _EMO_H
#define _EMO_H

#include <sys/time.h>
#include <stdio.h>

typedef struct Node {
  int index;
  int elem;
  struct Node *ant;
  struct Node *sig;
  int inblock;  // specifies whether the node is taken from memblock 
} Node;

typedef struct EMO_List {
  int length;
  int max_length;
  Node *ini;
  Node *fin;
  Node **memblock;
  int available;
} EMO_List;

void EMO_initList(EMO_List *l, int mlength);
void EMO_queue(EMO_List *l, int elem);
void EMO_add(EMO_List *l, int elem, int pos);

int EMO_dequeue(EMO_List *l);
int EMO_eliminateAt(EMO_List *l, int *elem, int pos);
int EMO_eliminate(EMO_List *l, int pos);
int EMO_List_find(EMO_List *l, int elem);
int EMO_element(EMO_List l, int pos);
void EMO_lprint(EMO_List l);
void EMO_clear(EMO_List *l);
void EMO_freeList(EMO_List *l);


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


#define NN 312

typedef struct {
  unsigned long long mt[NN]; /* The array for the state vector */
  int mti; // = NN + 1;      /* mti==NN+1 means mt[NN] is not initialized */
  unsigned long long seed;   /* Current seed */
  FILE *fp;                  /* Pointer to the seed file */
  char *token;               /* Temporary array for parsing the seed file */
  EMO_Debug *dbg;
} EMO_Rand;


void EMO_Rand_alloc(EMO_Rand *rnd, EMO_Debug *dbg, unsigned long long seed);
int EMO_Rand_next_seed(EMO_Rand *rnd, int skip);
void EMO_Rand_alloc_from_file(EMO_Rand *rnd, EMO_Debug *dbg, const char *file, int skip);
void EMO_Rand_free(EMO_Rand *rnd);
double EMO_Rand_prob1(EMO_Rand *rnd);
double EMO_Rand_prob2(EMO_Rand *rnd);
double EMO_Rand_prob3(EMO_Rand *rnd);
double EMO_Rand_real1(EMO_Rand *rnd, double a, double b);
double EMO_Rand_real2(EMO_Rand *rnd, double a, double b);
double EMO_Rand_real3(EMO_Rand *rnd, double a, double b);
int EMO_Rand_int1(EMO_Rand *rnd, int a, int b);
int EMO_Rand_int2(EMO_Rand *rnd, int a, int b);
int EMO_Rand_int3(EMO_Rand *rnd, int a, int b);  // me salio b
int EMO_Rand_flip(EMO_Rand *rnd, double prob);
void EMO_Rand_shuffle(EMO_Rand *rnd, int *idx, int size);

double EMO_vnorm(double *x, double p, int n);
void EMO_vmul(double *y, double *x, double alpha, int n);
void EMO_vsum(double *y, double *x, double alpha, int n);
double EMO_vdot(double *x, double *y, int n);
void EMO_vproj(double *r, double *x, double *y, int n);
void EMO_vadd(double *r, double *x, double *y, int n);
void EMO_vdiff(double *r, double *x, double *y, int n);
void EMO_vorth(double *r, double *p, double *v, int n);
double EMO_vdist(double *x, double *y, int n);
void EMO_vzero(double *x, int n);
int EMO_isvzero(double *x, int n);
void EMO_vaxes(double *x, int n);
void EMO_vcopy(double *dest, double *src, int n);
void EMO_vprint(FILE *fp, double *x, int n, const char *s);
void EMO_vprinti(FILE *fp, int *x, int n, const char *s);

void EMO_mprint(FILE *fp, const char *s, double *A, int n, int m);
void EMO_matmul(double *a, double *b, double *c, int n, int m, int l);
int EMO_minverse(double *B, double *A, int n, int nthread, int metodo, int tridiag);


#define PI 3.1415926535897932384626433832795
#define max(A,B) ((A)>(B) ? (A) : (B))  //ver funcion fmin:  double fmin(double x, double y); en math
#define min(A,B) ((A)<(B) ? (A) : (B))

double EMO_dmin(int *idx, double *arr, int *filter, int size);
double EMO_dmax(int *idx, double *arr, int *filter, int size);
int EMO_min(int *idx, int *arr, int *filter, int size);
int EMO_max(int *idx, int *arr, int *filter, int size);
void EMO_maxBound(double *max, double *data, int *filter, int row, int col);
void EMO_minBound(double *min, double *data, int *filter, int row, int col);
void EMO_maxminBound(double *max, double *min, double *data, int *filter, int row, int col);
void EMO_findmaxminBound(int *max, int *min, double *data, int *filter, int row, int col);
void EMO_normalize(double *norm, double *data, int *filter, int size, double *min, double *max, int dim);
void EMO_lsq(double *a, double *tmp1, double *tmp2, double *y, int n);
int EMO_find(int *data, int size, int elem);

double EMO_mean(double *data, int size);
double EMO_var(double *data, double mu, int size);
double EMO_std(double *data, double mu, int size);


#define EMO_BINARY 0
#define EMO_REAL 1

typedef struct EMO_MOP {
  int nvar;            /* number of decision variables  */
  int nobj;            /* number of objective functions */
  int ncon;            /* number of constraints */

  int npos;            /* number of position related parameters (WFG test problems) */

  double *xmin, *xmax; /* valid ranges of decision variables */

  void (*f)(struct EMO_MOP *mop, double *, double *);  /* pointer to the evaluation function */

  char *name;          /* name of the problem */
  int coding;          /* representation: real or binary */
  int L;               /* chromosome length (binary representation) */
  unsigned long feval; /* number of objective function evaluations */

                       /* Auxiliary variables */
  double *t;  /* theta for DTLZ5, DTLZ6 and transition vector for WFG test problems */
  double *x;           /* underlying parameters */
  double *h;           /* shape */
  double *S;           /* constant */
  double *A;           /* constant */
  double *y;           /* temporary vector */
  double *w;           /* weight vector */
} EMO_MOP;

typedef void (*EMO_MOP_evaluate)(EMO_MOP *mop, double *, double *);

typedef struct {
  double *var;
  double *obj;
  double *con;
  int mu;     /* parent population size */
  int lambda; /* offspring population size */
  int size;  /* total population size */
  double *vdummy, *odummy, *cdummy; // Internal use
} EMO_Population;

void EMO_Population_alloc(EMO_Population *pop, EMO_MOP *mop, int mu, int lambda);
void EMO_Population_init(EMO_Population *pop, EMO_Rand *rand, EMO_MOP *mop);
int EMO_Population_init_from_file(EMO_Population *pop, EMO_MOP *mop, const char *file, int start);
void EMO_Population_write(EMO_Population *pop, int *filter, EMO_MOP *mop, const char *prefix, int run);
void EMO_Population_free(EMO_Population *pop);
void EMO_Population_copy(EMO_Population *pop, int *fit1, double *fit2, EMO_MOP *mop, int i, int j);
void EMO_Population_swap(EMO_Population *pop, int *fit1, double *fit2, EMO_MOP *mop, int i, int j);
void EMO_Population_survive(EMO_Population *pop, int *fit1, double *fit2, EMO_MOP *mop, EMO_List *missing, EMO_List *available, int *filter);


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

typedef struct {
  char **name;  /* Parameter names */
  char **value; /* Parameter value */
  int size;     /* Number of parameters */
  int current;  /* Current parameter (only for set function) */
  EMO_Debug *dbg;
} EMO_Parser;

void EMO_Parser_alloc(EMO_Parser *parser, EMO_Debug *dbg, int max_param);
void EMO_Parser_alloc_from_file(EMO_Parser *parser, EMO_Debug *dbg, char *file);
void EMO_Parser_free(EMO_Parser *parser);
int EMO_Parser_get_int(EMO_Parser *parser, int *v, const char *s);
int EMO_Parser_get_double(EMO_Parser *parser, double *v, const char *s);
int EMO_Parser_get_char(EMO_Parser *parser, char *v, const char *s);
void EMO_Parser_set(EMO_Parser *parser, const char *name, const char *value);

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
void EMO_Param_save(EMO_Param *param, EMO_Population *pop, EMO_MOP *mop, int run);
void EMO_Param_free(EMO_Param *param);
void EMO_Param_set(EMO_Param *param, const char *name, const char *value);

void EMO_crossSBX(double *child1, double *child2, double *parent1, double *parent2, EMO_Rand *rand, EMO_MOP *mop, double Pc, double eta_c);
void EMO_mutatePolynom(double *child, EMO_Rand *rand, EMO_MOP *mop, double Pm, double eta_m);
  
// Representacion binaria
void EMO_one_point_crossover(double *child1, double *child2, double *parent1, double *parent2, EMO_Rand *rand, EMO_MOP *mop, double Pc);
void EMO_one_bit_mutation(double *child, EMO_Rand *rand, EMO_MOP *mop);
   
void tournament_selection_dominance(int *ind, EMO_Rand *rand, int *seedp, double *obj, int n, int nobj);
void tournament_selection_rank(int *ind, EMO_Rand *rand, int *seedp, int *rank, int n);
void tournament_selection_rank_crowding(int *ind, EMO_Rand *rand, int *seedp, int *rank, double *cd, int n);
void tournament_selection_fitness(int *ind, EMO_Rand *rand, int *seedp, double *fit, int n);
void tournament_selection_r2(int *ind, EMO_Rand *rand, int *seedp, double *r2, int n, int m);
int tournament_selection_fitness2(double *fit, EMO_Rand *rand, int size);


typedef struct EMO_Utility {
  char *name;
  int nobj;
  double *tmp;  // pbi parameter
  double (*uf)(struct EMO_Utility *u, double *w, double *x);
} EMO_Utility;

typedef double (*EMO_UtilityFunction)(EMO_Utility *u, double *w, double *x);

void EMO_Utility_alloc(EMO_Utility *u, int nobj, const char *str);
void EMO_Utility_free(EMO_Utility *u);
double EMO_Utility_asf(EMO_Utility *u, double *w, double *x);
double EMO_Utility_weighted_sum(EMO_Utility *u, double *w, double *x);
double EMO_Utility_least_squares(EMO_Utility *u, double *w, double *x);
double EMO_Utility_tchebycheff(EMO_Utility *u, double *w, double *x);
double EMO_Utility_aug_tchebycheff(EMO_Utility *u, double *w, double *x);
double EMO_Utility_mod_tchebycheff(EMO_Utility *u, double *w, double *x);
double EMO_Utility_pbi(EMO_Utility *u, double *w, double *x);

double *EMO_File_read(double *data, int *row, int *col, const char *str, int start);
void EMO_File_write(double *data, int *filter, int row, int col, const char *str, const char *fmt);


double EMO_hv2(double *data, int n, const double *ref, int d);
double EMO_hv2_contribution(double *deltahv, double *data, int *enable, int n, const double *ref, int d);


typedef double OBJECTIVE;

typedef struct
{
        OBJECTIVE *objectives;
} POINT;

typedef struct
{
        int nPoints;
//      int n;
        POINT *points;
} FRONT;


typedef struct {
  int col;      // the number of objectives 
  int row;
  FRONT *fs;    // memory management stuff 
  FRONT f;
  int fr;       // current depth 
  int safe;     // the number of points that don't need sorting 
} EMO_HV;


void EMO_HV_alloc(EMO_HV *hv, int row, int col);
void EMO_HV_free(EMO_HV *hv);
double EMO_HV_run(EMO_HV *hv, double *data, int *enable, int row, const double *ref);
double EMO_HV_contribution(EMO_HV *hv, double *deltahv, double *data, int *enable, int row, const double *ref, int col);

double EMO_gd(double *f, int *filter, int n, double *pf, int m, int dim, double p);
double EMO_igd(double *f, int *filter, int n, double *pf, int m, int dim, double p);
double EMO_igd2(double *f, int *filter, int n, double *pf, int m, int dim);
double EMO_igd_plus(double *f, int *filter, int n, double *pf, int m, int dim);
double EMO_deltap(double *f, int *filter, int n, double *pf, int m, int dim, double p);

double EMO_r2(double *data, int *filter, int size, double *W, int wsize, EMO_Utility *utl);
void EMO_r2_ranking(double *rank, double **sort, double *norm, double *tmp, double *data, int size, double *W, int wsize, EMO_Utility *utl);


void EMO_Benchmark_alloc(EMO_MOP *mop, EMO_Param *param, const char *problem);
void EMO_Benchmark_free(EMO_MOP *mop);

typedef int (*EMO_Dominance)(double *, double *, int);

int EMO_Dominance_weak(double *x, double *y, int n);
int EMO_Dominance_strict(double *x, double *y, int n);
int EMO_Dominance_strong(double *x, double *y, int n);
int EMO_Dominance_alpha(double *x, double *y, double *tmp, double *alpha, int n);

int EMO_Dominance_incomparable(double *x, double *y, int n, EMO_Dominance r);
int EMO_Dominance_indifferent(double *x, double *y, int n);
int EMO_Dominance_constraint(double *x, double *y, int n, double *gx, double *gy, int k, EMO_Dominance r);
int EMO_Dominance_constraint2(double *x, double *y, int n, double *gx, double *gy, int k, EMO_Dominance r);
int EMO_Dominance_feasible(double *g, int k);
int EMO_Dominance_ndset(int *nd, double *data, int row, int col, EMO_Dominance r);

typedef struct {
  int size;         // numero de soluciones a evaluar
  int *n;           // auxiliar, numero de veces que una solucion i ha sido dominada
  int *rank;        // jerarquia de los individuos
  int nfront;       // numero de frentes
  EMO_List *S;      // auxiliar, lista de soluciones que domina el i-esimo individuo
  EMO_List *front;  // Lista de frentes
} EMO_NDSort;

void EMO_NDSort_alloc(EMO_NDSort *nd, int max_size);
void EMO_NDSort_free(EMO_NDSort *nd);
void EMO_NDSort_run(EMO_NDSort *nd, double *obj, int *filter, int size, int nobj);
void EMO_NDSort_run2(EMO_NDSort *nd, double *obj, int *filter, double *tmp, double *alpha, int size, int nobj);
void EMO_Dominance_ranking_sum(double *rank, double **sort, double *data, int size, int nobj);
void EMO_Dominance_ranking_min(double *rank, double **sort, double *data, int size, int nobj);


void EMO_nicheCount(double *niche, double *data, int row, int col, double alpha, double sigma_share);
void EMO_crowdingDistance(double *cd, double **sort, double *data, int row, double *min, double *max, int col);
void EMO_crowdingDistance2(double *cd, double **sort, double *data, EMO_List *front, double *max, double *min, int col);
void EMO_wdist(double *wd, double *data, int size, double *W, int wsize, double *tmp, int dim);


// Metodo de truncado de soluciones basado en SPEA2
typedef struct {
  EMO_List *lnn;          /* array of lists for storing neighbors */
  EMO_List copy;
  double **sort;          /* temporary array for sorting */
  double **dist;
  int *min, *max;         /* individuals that are part of the extreme points */
  int dim, ssize;
} EMO_KNN;

void EMO_KNN_alloc(EMO_KNN *knn, int size, int dim);
void EMO_KNN_free(EMO_KNN *knn);
void EMO_KNN_prune(EMO_KNN *knn, int *filter, int max_size, double *data, int size);

void EMO_knn(EMO_List *l, double **sort, double *W, int size, int dim, int k, int itself);
void EMO_knn2(EMO_List *l, EMO_List *copy, double **dist, double **sort, double *W, int size, int dim);
void EMO_kNN2(EMO_List *lst, double **sort, double *data, int row, int col);

int EMO_compare_asc(const void **a, const void **b);
int EMO_compare_desc(const void **a, const void **b);
int EMO_compare_rows(const void *a, const void *b, int start, int n);
void EMO_quicksort (void *const pbase, size_t total_elems, int start, int dimension, size_t size, int(*cmp)(const void *, const void *, int, int));
int EMO_binsert(const void *A, const void *key, int total_elems, int start, int dimension, size_t size, int(*cmp)(const void *, const void *, int, int));

typedef struct {
  EMO_List lst1, lst2; // temporary lists
  double *norm;        // normalized objective functions
  double *rank;        // rank of individuals
  double *l2;          // l2 norm
  double **sort;       // temporary array for sorting population
  int ssize;           // temporal population size (for memory free)
  double *tmp;         // temporary array
  int wsize;           // number of weight vectors
  double *W;           // weight vectors
  double *min;         // ideal point
  double *max;         // nadir point
  double *ideal;       // ideal point
  double *new_min;
  double *new_max;
  double *hist;
  int *update;
  EMO_Utility utl;
  int *filter;         // selected individuals
  int max_hist;        /* parameters of the algorithm */
  double epsilon;
  double alpha;
  char *wfile;
} EMO_MOMBI2;

void EMO_MOMBI2_alloc(EMO_MOMBI2 *alg, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop);
void EMO_MOMBI2_free(EMO_MOMBI2 *alg);
void EMO_MOMBI2_run(EMO_MOMBI2 *alg, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop);
void EMO_MOMBI2_prun(EMO_MOMBI2 *alg, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop);

/* Al final, Generic MOEA */
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

