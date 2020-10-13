/* 
   Original file: mt19937_64.c

   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt64.html
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/VERSIONS/C-LANG/mt19937-64.c

   A C-program for MT19937-64 (2004/9/29 version).
   Coded by Takuji Nishimura and Makoto Matsumoto.

   This is a 64-bit version of Mersenne Twister pseudorandom number
   generator.

   Before using, initialize the state by using init_genrand64(seed)  
   or init_by_array64(init_key, key_length).

   Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
     ACM Transactions on Modeling and 
     Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
     ``Mersenne Twister: a 623-dimensionally equidistributed
       uniform pseudorandom number generator''
     ACM Transactions on Modeling and 
     Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)

   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/efaq.html

   Want a uniform random number in the integers [0..N-1].

   If N is not large (say, <216) and you don't need the precision, 
   it would be enough to use [0,1)-version (use double-precision), 
   multiply the output by N, then take floor (round-off). 
   (Caution: do not use [0,1]-version. Appearance of 1.0 may cause 
   the output of N. This occurs roughly once in 4.2 billion times).
   WARNING: this solution has numerical error in the distribution, 
   unless N is a power of 2. The order of the error is N times 2^-32. 
   A better, safer method is to obtain minimum integer n such that 
   N<=2^n, generate integer random numbers, take the most significant 
   n bits, and discard those more than or equal to N. 

   This version is thread-safe.
*/

#include <math.h>
#include <stdlib.h>

#include "random.h"
#include "debug.h"

/* initializes mt[NN] with a seed */
void init_genrand64(EMO_Rand *rnd, unsigned long long seed) {
  rnd->mt[0] = seed;

  for(rnd->mti = 1; rnd->mti < NN; rnd->mti++) 
    rnd->mt[rnd->mti] = (6364136223846793005ULL * (rnd->mt[rnd->mti - 1] ^ (rnd->mt[rnd->mti - 1] >> 62)) + rnd->mti);

  EMO_Debug_printf(rnd->dbg, "EMO_Rand:Setting the seed %20llu", seed);
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
void init_by_array64(EMO_Rand *rnd, unsigned long long init_key[], unsigned long long key_length) {
  unsigned long long i, j, k;

  init_genrand64(rnd, 19650218ULL);
  k = (NN > key_length ? NN : key_length);
  i = 1;
  j = 0;
 
  for(; k; k--) {
    rnd->mt[i] = (rnd->mt[i] ^ ((rnd->mt[i - 1] ^ (rnd->mt[i - 1] >> 62)) * 3935559000370003845ULL)) + init_key[j] + j; /* non linear */
    i++;
    j++;

    if(i >= NN) {
      rnd->mt[0] = rnd->mt[NN - 1]; 
      i = 1;
    }
    if(j >= key_length)
      j = 0;
  }

  for(k = NN-1; k; k--) {
    rnd->mt[i] = (rnd->mt[i] ^ ((rnd->mt[i - 1] ^ (rnd->mt[i - 1] >> 62)) * 2862933555777941757ULL)) - i; /* non linear */
    i++;

    if(i >= NN) { 
      rnd->mt[0] = rnd->mt[NN - 1]; 
      i = 1;
    }
  }

  rnd->mt[0] = 1ULL << 63; /* MSB is 1; assuring non-zero initial array */ 
}

void EMO_Rand_alloc(EMO_Rand *rnd, EMO_Debug *dbg, unsigned long long seed) {
  rnd->dbg = dbg;
  init_genrand64(rnd, seed);
  rnd->seed = seed;
  rnd->fp = NULL;
  rnd->token = NULL;
}

/* Read the next seed from file and reset the random number generator,
discard "skip" seeds */
int EMO_Rand_next_seed(EMO_Rand *rnd, int skip) {
  int i, j, c, flag;

  i = j = flag = 0;

  EMO_Debug_printf(rnd->dbg, "EMO_Rand:skipping %d seeds", skip);

  while((c = fgetc(rnd->fp)) != EOF) {
    if(c == '#') {
      flag = 1;
    }
    else if(c == '\n') {
      if(!flag) {
        rnd->token[i] = '\0';
        //*seed = atof(rnd->token);
        rnd->seed = atoll(rnd->token);

        if(skip == j++) {
          init_genrand64(rnd, rnd->seed);
          return 1;
        }
      }
      i = flag = 0;
    }
    else if(!flag) {
      rnd->token[i++] = c;
    }
  }

  EMO_Debug_printf(rnd->dbg, "EMO_Rand:warning, no more available seeds from file");
  return 0;
}

void EMO_Rand_alloc_from_file(EMO_Rand *rnd, EMO_Debug *dbg, const char *file, int skip) {

  rnd->dbg = dbg;

  if((rnd->fp = fopen(file, "r")) == NULL) {
    EMO_Debug_printf(rnd->dbg, "EMO_Rand:error to open the file %s", file);
    exit(1);
  }

  if((rnd->token = (char *) malloc(sizeof(char) * 1000)) == NULL) {
    EMO_Debug_printf(rnd->dbg, "EMO_Rand:error to allocate memory");
    exit(1);
  }

  if(EMO_Rand_next_seed(rnd, skip) == 0) {
    EMO_Debug_printf(rnd->dbg, "EMO_Rand:error to read seed from file");
    exit(1);
  }
}

void EMO_Rand_free(EMO_Rand *rnd) {
  if(rnd->fp != NULL)
    fclose(rnd->fp);

  if(rnd->token != NULL)
    free(rnd->token);
}

/* generates a random number on [0, 2^64-1]-interval */
unsigned long long genrand64_int64(EMO_Rand *rnd) {
  static unsigned long long mag01[2] = {0ULL, MATRIX_A};
  unsigned long long x;
  int i;

  if(rnd->mti >= NN) { /* generate NN words at one time */

    /* if init_genrand64() has not been called, */
    /* a default initial seed is used     */
    if (rnd->mti == NN + 1) 
      init_genrand64(rnd, 5489ULL); 

    for (i = 0; i < NN - MM; i++) {
      x = (rnd->mt[i] & UM) | (rnd->mt[i + 1] & LM);
      rnd->mt[i] = rnd->mt[i + MM] ^ (x >> 1) ^ mag01[(int)(x & 1ULL)];
    }
    for (;i < NN - 1; i++) {
      x = (rnd->mt[i] & UM) | (rnd->mt[i + 1] & LM);
      rnd->mt[i] = rnd->mt[i + (MM - NN)] ^ (x >> 1) ^ mag01[(int)(x & 1ULL)];
    }
    x = (rnd->mt[NN - 1] & UM) | (rnd->mt[0] & LM);
    rnd->mt[NN - 1] = rnd->mt[MM - 1] ^ (x >> 1) ^ mag01[(int)(x & 1ULL)];
    rnd->mti = 0;
  }
 
  x = rnd->mt[rnd->mti++];

  x ^= (x >> 29) & 0x5555555555555555ULL;
  x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
  x ^= (x << 37) & 0xFFF7EEE000000000ULL;
  x ^= (x >> 43);

  return x;
}

/* generates a random number on [0, 2^63-1]-interval */
long long genrand64_int63(EMO_Rand *rnd) {
  return (long long)(genrand64_int64(rnd) >> 1);
}

/* generates a random number on [0,1]-real-interval */
double EMO_Rand_prob1(EMO_Rand *rnd) {
  return (genrand64_int64(rnd) >> 11) * (1.0 / 9007199254740991.0);
}

/* generates a random number on [0,1)-real-interval */
double EMO_Rand_prob2(EMO_Rand *rnd) {
  return (genrand64_int64(rnd) >> 11) * (1.0 / 9007199254740992.0);
}

/* generates a random number on (0,1)-real-interval */
double EMO_Rand_prob3(EMO_Rand *rnd) {
  return ((genrand64_int64(rnd) >> 12) + 0.5) * (1.0 / 4503599627370496.0);
}

/* generates a random number on [a,b]-real-interval */
double EMO_Rand_real1(EMO_Rand *rnd, double a, double b) {
  return a + ((genrand64_int64(rnd) >> 11) * (1.0 / 9007199254740991.0)) * (b - a);
}

/* generates a random number on [a,b)-real-interval */
double EMO_Rand_real2(EMO_Rand *rnd, double a, double b) {
  return a + ((genrand64_int63(rnd) >> 11) * (1.0 / 9007199254740992.0)) * (b - a);
}

/* generates a random number on (a,b)-real-interval */
double EMO_Rand_real3(EMO_Rand *rnd, double a, double b) {
  return a + (((genrand64_int64(rnd) >> 12) + 0.5) * (1.0 / 4503599627370496.0)) * (b - a);
}

/* generates a random number on [a,b]-interval */
int EMO_Rand_int1(EMO_Rand *rnd, int a, int b) {
  return a + (int) floor(EMO_Rand_prob1(rnd) * (b - a + 1));
}

/* generates a random number on [a,b)-interval */
int EMO_Rand_int2(EMO_Rand *rnd, int a, int b) {
  return a + (int) floor(EMO_Rand_prob2(rnd) * (b - a));
}

/* generates a random number on (a,b)-interval */
int EMO_Rand_int3(EMO_Rand *rnd, int a, int b) {
  return a + (int) floor(EMO_Rand_prob3(rnd) * (b - a - 1)) + 1;
}

int EMO_Rand_flip(EMO_Rand *rnd, double prob) {
  if(EMO_Rand_prob1(rnd) <= prob)
    return 1;
  return 0;
}

/* Fisher-Yates shuffle, algorithm for generating a random permutation 
   of a finite set */
void EMO_Rand_shuffle(EMO_Rand *rnd, int *idx, int size) {
  int i, r, swap;

  for(i = size - 1; i > 0; i--) {
    r = EMO_Rand_int1(rnd, 0, i);
    swap = idx[i];
    idx[i] = idx[r];
    idx[r] = swap;
  }
}

/*int main(void) {
  //unsigned long long init[4] = {0x12345ULL, 0x23456ULL, 0x34567ULL, 0x45678ULL}, length = 4;
  int i, j, *idx;
  EMO_Rand rnd;

  idx = (int *) malloc(sizeof(int) * 20);

  for(i = 0; i < 20; i++)
    idx[i] = i + 1;

  // Se salta al principio 5 semillas
  EMO_Rand_alloc_from_file(&rnd, NULL, "../demo/input/seed.dat", 5); 

  do {
    printf("1000 EMO_Rand_prob1\n");
    for (i = 0; i < 1000; i++) {
      //printf("%20llu ", EMO_Rand_prob1(&rnd));
      printf("%10.8f ", EMO_Rand_prob1(&rnd));
      if (i % 5 == 4) printf("\n");
    }

    printf("\n1000 EMO_Rand_prob2\n");
    for (i = 0; i < 1000; i++) {
      printf("%10.8f ", EMO_Rand_prob2(&rnd));
      if (i % 5 == 4) printf("\n");
    }

    printf("\n1000 EMO_Rand_prob3\n");
    for (i = 0; i < 1000; i++) {
      printf("%10.8f ", EMO_Rand_prob3(&rnd));
      if (i % 5 == 4) printf("\n");
    }

    printf("\n1000 EMO_Rand_real1\n");
    for (i = 0; i < 1000; i++) {
      printf("%10.8f ", EMO_Rand_real1(&rnd, 12.0, 13.0));
      if (i % 5 == 4) printf("\n");
    }

    printf("\n1000 EMO_Rand_real2\n");
    for (i = 0; i < 1000; i++) {
      printf("%10.8f ", EMO_Rand_real2(&rnd, 12.0, 13.0));
      if (i % 5 == 4) printf("\n");
    }

    printf("\n1000 EMO_Rand_real3\n");
    for (i = 0; i < 1000; i++) {
      printf("%10.8f ", EMO_Rand_real3(&rnd, 12.0, 13.0));
      if (i % 5 == 4) printf("\n");
    }

    printf("\n1000 EMO_Rand_int1\n");
    for (i = 0; i < 1000; i++) {
      printf("%d ", EMO_Rand_int1(&rnd, 10.0, 20.0));
      if (i % 5 == 4) printf("\n");
    }

    printf("\n1000 EMO_Rand_int2\n");
    for (i = 0; i < 1000; i++) {
      printf("%d ", EMO_Rand_int2(&rnd, 10.0, 20.0));
      if (i % 5 == 4) printf("\n");
    }

    printf("\n1000 EMO_Rand_int3\n");
    for (i = 0; i < 1000; i++) {
      printf("%d ", EMO_Rand_int3(&rnd, 10.0, 20.0));
      if (i % 5 == 4) printf("\n");
    }

    printf("\n1000 EMO_Rand_flip\n");
    for (i = 0; i < 1000; i++) {
      printf("%d ", EMO_Rand_flip(&rnd, 0.5));
      if (i % 5 == 4) printf("\n");
    }

    printf("\n1000 EMO_Rand_shuffle\n");
    for (i = 0; i < 1000; i++) {
      EMO_Rand_shuffle(&rnd, idx, 20);
 
     for(j = 0; j < 20; j++)
        printf("%d ", idx[j]);
     printf("\n");
    }

    // Se salta 0 semillas del archivo
  } while(EMO_Rand_next_seed(&rnd, 0));


  EMO_Rand_free(&rnd);
  free(idx);

  return 0;
}*/



