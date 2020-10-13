#ifndef _HV_WFG_H_
#define _HV_WFG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef double OBJECTIVE;

typedef struct
{
	OBJECTIVE *objectives;
} POINT;

typedef struct
{
	int nPoints;
//	int n;
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

#endif

