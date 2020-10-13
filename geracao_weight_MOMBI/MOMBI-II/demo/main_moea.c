#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "emo.h"


/**** Only for the definition of a new Multi-objective Optimization Problem ****/

void myMOP_eval(EMO_MOP *mop, double *f, double *x) {
  f[0] = x[0] * x[0];
  f[1] = pow(x[0] - 1.0, 2.0);
  mop->feval++;
}

void myMOP_alloc(EMO_MOP *mop) {
  int i;

  /* MOP's name */
  mop->name = (char *) malloc(sizeof(char) * 200);
  strcpy(mop->name, "WING_DESIGN");

  /* Number of decision variables */
  mop->nvar = 2;

  /* Number of objectives */
  mop->nobj = 2;

  /* Box constraints */
  mop->xmin = (double *) calloc(sizeof(double), mop->nvar);
  mop->xmax = (double *) calloc(sizeof(double), mop->nvar);

  for(i = 0; i < mop->nvar; i++) {
    mop->xmin[i] = 0.0;
    mop->xmax[i] = 1.0;
  }

  /* Required */
  mop->ncon = 0;
  mop->npos = 0;
  mop->feval = 0;
  mop->coding = EMO_REAL;
  mop->f = myMOP_eval;
}

/**************************** End of the definition ****************************/


int main(int argc, char **argv) {
  char alg[] = "MOMBI2";
  EMO_Population pop;
  EMO_Param param;
  EMO_MOEA moea;
  EMO_MOP mop;
  int r, nrun;

  if(argc != 4) {
    printf("Syntax: %s parameter_file test_problem runs\n", argv[0]);
    exit(1);
  }

  nrun = atoi(argv[3]);

  if(strcmp(argv[2], "DEFAULT") == 0 || strcmp(argv[2], "default") == 0)
    myMOP_alloc(&mop);
  
  EMO_Param_alloc_from_file(&param, &mop, alg, argv[1], argv[2]);
  EMO_MOEA_alloc(&moea, &param, &pop, &mop, alg);

  for(r = 1; r <= nrun; r++) {
    EMO_Stop_start(param.stop);

    EMO_Population_init(&pop, param.rand, &mop);

    EMO_MOEA_run(&moea, &param, &pop, &mop);

    EMO_Population_write(&pop, NULL, &mop, param.prefix, r);
    EMO_Param_save(&param, &pop, &mop, r);

    EMO_Rand_next_seed(param.rand, 0);
  }

  EMO_MOEA_free(&moea);
  EMO_Param_free(&param);
  EMO_Population_free(&pop);
  return 0;
}

