
/*problemdef.h   Test problem definitions */

#define PI 3.1415926535897932384626433832795

void zdt1(double **f, double *x, int m, int n, ...);
void zdt2(double **f, double *x, int m, int n, ...);
void zdt3(double **f, double *x, int m, int n, ...);
void zdt4(double **f, double *x, int m, int n, ...);
void zdt6(double **f, double *x, int m, int n, ...);
void dtlz1(double **f, double *x, int m, int n, ...);
void dtlz2(double **f, double *x, int m, int n, ...);
void dtlz3(double **f, double *x, int m, int n, ...);
void dtlz4(double **f, double *x, int m, int n, ...);
void dtlz5(double **f, double *x, int m, int n, ...);
void dtlz6(double **f, double *x, int m, int n, ...);
void dtlz7(double **f, double *x, int m, int n, ...);

void findfunc(char *nom, void (**apf)(double **, double *x, int, int, ...)); 

