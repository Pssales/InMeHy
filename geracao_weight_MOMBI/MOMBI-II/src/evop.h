
#ifndef _EVOP_
#define _EVOP_

#include "common.h"

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
#endif

