/**************************************************************
 * benchmark.h   Definition of test functions.                *
 *                                                            *
 * March 2013                                                 *
 *************************************************************/

#ifndef _TESTF_
#define _TESTF_

#include "common.h"
#include "param.h"

void EMO_Benchmark_alloc(EMO_MOP *mop, EMO_Param *param, const char *problem);
void EMO_Benchmark_free(EMO_MOP *mop);

#endif



