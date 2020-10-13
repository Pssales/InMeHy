
#define max(A,B) ((A)>(B) ? (A) : (B)) 
#define min(A,B) ((A)<(B) ? (A) : (B)) 
#define PI 3.1415926535897932384626433832795

double *zmax;
double *t;
double *x;
double *h;
double *S;
double *A;
double *y;
double *w;

void print_vec(char *s, double *v, int n);

/* Shapes */
double sh_linear_1(double *x, int M);
double sh_linear_m(double *x, int M, int m);
double sh_linear_M(double *x);
void sh_linear(double **h, double *x, int M); 
double sh_convex_1(double *x, int M);
double sh_convex_m(double *x, int M, int m);
double sh_convex_M(double *x);
void sh_convex(double **h, double *x, int M); 
double sh_concave_1(double *x, int M);
double sh_concave_m(double *x, int M, int m);
double sh_concave_M(double *x);
void sh_concave(double **h, double *x, int M); 
double sh_mixed_M(double *x, double A, double alpha);
double sh_disc_M(double *x, double A, double alpha, double beta);

/* Transformations */
double tr_b_poly(double y, double alpha);
double tr_b_flat(double y, double A, double B, double C);
double tr_b_param(double y, double u, double A, double B, double C);
double tr_s_linear(double y, double A);
double tr_s_decept(double y, double A, double B, double C);
double tr_s_multi(double y, double A, double B, double C);
double tr_r_sum(double *y, double *w, int n);
double tr_r_nonsep(double *y, double A, int n);

/* Transformations per function */
void wfg1_t1(double **t, double *z, int N, int K); 
void wfg1_t2(double **t, double *z, int N, int K); 
void wfg1_t3(double **t, double *z, int N); 
void wfg1_t4(double **t, double *z, int M, int N, int K); 
void wfg2_t2(double **t, double *z, int N, int K); 
void wfg2_t3(double **t, double *z, int M, int N, int K); 
void wfg4_t1(double **t, double *z, int N); 
void wfg4_t2(double **t, double *z, int M, int N, int K); 
void wfg5_t1(double **t, double *z, int N); 
void wfg6_t2(double **t, double *z, int M, int N, int K); 
void wfg7_t1(double **t, double *z, int N, int K); 
void wfg8_t1(double **t, double *z, int N, int K);
void wfg9_t1(double **t, double *z, int N); 
void wfg9_t2(double **t, double *z, int N, int K); 

void calc_x(double **x, double *t, double *A, int M); 
void calc_f(double **f, double *x, double *S, double *h, int M); 

/* WFG test suite */
void wfg1(double **f, double *z, int M, int N, ...);
void wfg2(double **f, double *z, int M, int N, ...);
void wfg3(double **f, double *z, int M, int N, ...);
void wfg4(double **f, double *z, int M, int N, ...);
void wfg5(double **f, double *z, int M, int N, ...);
void wfg6(double **f, double *z, int M, int N, ...);
void wfg7(double **f, double *z, int M, int N, ...);
void wfg8(double **f, double *z, int M, int N, ...);
void wfg9(double **f, double *z, int M, int N, ...);

void init_WFG(int wfg, int M, int N, int K);
void free_WFG(); 

//#endif

