#include <stdio.h>
#include <stdlib.h>

void create_vectors(double **wv, const int s, const int k); 
int choose(const int n, const int r); 
inline int mypow(int x, int exp); 
int factorialdiff(const int n, const int r); 
int factorial(const int n); 
inline void int2kary(int x, const int basek, const int digits, int *kary);

void create_vectors(double **wv, const int s, const int k) {
  int i,j;
  int c=0;
  int num, sum;
  int *count;

  count = (int *)malloc(k*sizeof(int));
  num = choose(s+k-1,k-1);

  i=0;

  while(i<mypow(s+1,k)) {
    sum=0;

    int2kary(i,s+1,k,count);

    for(j=0;j<k;j++) {
      sum+=count[j];
    }

    if(sum==s) {

      for(j=0;j<k;j++) {
        wv[c][j]=(double)count[j]/(double)s;
      }
      c++;
    }      
    i++;
  }
  free(count);
}

int choose(const int n, const int r) {
  return(factorialdiff(n,r)/factorial(r));
}

inline int mypow(int x, int exp) {
  int i;
  int var=1;

  for(i=0;i<exp;i++)
    var*=x;

  return(var);
}

int factorialdiff(const int n, const int r) {
  //this function calculates n!/(n-r)!

  int i;
  int result=1;

  for(i=n;i>(n-r);i--) {
    result*=i;
  }
  return(result);
}


int factorial(const int n) {

  if(n>11) {
    printf("too big number to factorial!\n");
    exit(1);
  }

  int i;
  int result=1;

  for(i=n;i>1;i--) {
    result*=i;
  }

  return(result);
}

void int2kary(int x, const int basek, const int digits, int *kary) {
  int i;
  int val;

  if (x>=mypow(basek,digits)) {
    printf("Number in int2kary() too large. Exiting.\n");
    exit(-1);
  }

  val=digits-1;

  for(i=0;i<digits;i++)
    kary[i]=0;

  i=0;

  while(x) {

    if(x>=mypow(basek,val)) {
      kary[i]+=1;
      x-=mypow(basek,val);
    }
    else {
      val-=1;
      i++;
    }
  }
}

int main(int argc, char *argv[]){
  int i, j, n, s = atoi(argv[1]), k = atoi(argv[2]);
  double **arr;

  n = choose(s+k-1,k-1);

  arr = (double **) malloc(sizeof(double*)*n);

  for(i=0; i < n; i++)
    arr[i] = (double *) malloc(sizeof(double)*k);
 
 create_vectors(arr, s, k);

  for(i=0; i < n; i++) {
    for(j=0; j < k; j++)
      printf("%f ", arr[i][j]);
    printf("\n");
  }

  for(i=0; i < n;i++)
    free(arr[i]);


  free(arr);
}
