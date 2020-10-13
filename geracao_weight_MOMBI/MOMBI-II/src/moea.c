
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "moea.h"

void EMO_MOEA_alloc(EMO_MOEA *moea, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop, const char *str) {

  const char *name[]       = {  "MOMBI2",
                                NULL};

  const EMO_MOEA_falloc a[] = {(EMO_MOEA_falloc) EMO_MOMBI2_alloc
                              };

  const EMO_MOEA_ffree f[]  = {(EMO_MOEA_ffree) EMO_MOMBI2_free
                              };

  const EMO_MOEA_frun r[]   = {(EMO_MOEA_frun) EMO_MOMBI2_run
                              };

  const EMO_MOEA_fprun p[]  = {(EMO_MOEA_fprun) EMO_MOMBI2_prun
                              };

  int i, n;
  char *aux;

  n = strlen(str);
  aux = (char *) malloc(sizeof(char) * (n+1));

  for(i=0; i < n; i++)
    aux[i] = toupper(str[i]);
  
  aux[n] = '\0';
  i = 0;

  while(name[i] != NULL) {
    if(strcmp(aux, name[i]) == 0) {
      moea->alloc = a[i];
      moea->free = f[i];
      moea->run = r[i];
      moea->prun = p[i];

      switch(i) {
        case 0:  moea->palg = &moea->alg.mombi2; 
                 break;
      }

      moea->alloc(moea->palg, param, pop, mop);
      free(aux);
      return;
    }
    i++;
  }

  printf("Error, unknown MOEA %s.\n", aux);
  free(aux);
  exit(1);
}


void EMO_MOEA_free(EMO_MOEA *moea) {
  moea->free(moea->palg);
}

void EMO_MOEA_run(EMO_MOEA *moea, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop) {
  moea->run(moea->palg, param, pop, mop);
}

void EMO_MOEA_prun(EMO_MOEA *moea, EMO_Param *param, EMO_Population *pop, EMO_MOP *mop) {
  moea->prun(moea->palg, param, pop, mop);
}

