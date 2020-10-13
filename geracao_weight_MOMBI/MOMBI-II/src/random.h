
/* Original file: mt19937_64.c */
 
#ifndef _mt19937_64_
#define _mt19937_64_

#include <stdio.h>
#include "debug.h"

#define NN 312
#define MM 156
#define MATRIX_A 0xB5026F5AA96619E9ULL
#define UM 0xFFFFFFFF80000000ULL /* Most significant 33 bits */
#define LM 0x7FFFFFFFULL         /* Least significant 31 bits */

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
int EMO_Rand_int3(EMO_Rand *rnd, int a, int b);
int EMO_Rand_flip(EMO_Rand *rnd, double prob);
void EMO_Rand_shuffle(EMO_Rand *rnd, int *idx, int size);

#endif

