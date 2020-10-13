
#include <stdio.h>
#include <stdlib.h>


void print(FILE *fp, double *x, int m, int h) {
  int i;

  for(i = 0; i < m; i++)
    fprintf(fp, "%e ", x[i]/h);

  fprintf(fp, "\n");
}

int simplex_lattice_design(FILE *fp, int m, int h) {
  double *x, s;
  int i, j, k;

  k = 1;

  x = (double *) malloc(sizeof(double) * m);

  x[0] = h;

  for(i = 1; i < m; i++)
    x[i] = 0;

  j = 0;

  do {
    print(fp, x, m, h);

    x[j]--;

    s = 0;

    for(i = 0; i <= j; i++)
      s += x[i];

    x[j+1] = h - s;

    for(i = j + 2; i < m; i++)
      x[i] = 0;

    j++;

    if(j == m-1) { 

      for(i = m-2; i >= 0; i--)
        if(x[i] > 0) {
          j = i;
          break;
        }
    }

    k++;

  } while(x[m-1] != h);

  print(fp, x, m, h);
  free(x);
  return k;
}

void open_file(FILE **fp, char *nom, char *mode) {

  if((*fp = fopen(nom, mode)) == NULL) {
    printf("Error to open the file %s.\n", nom);
    exit(1);
  }
}

void copy_file(FILE *fp1, FILE *fp2) {
  char *str = NULL;
  size_t n;

  rewind(fp1);

  while(getline(&str, &n, fp1) != -1) 
    fprintf(fp2, "%s", str);

  free(str);
}

int main(int argc, char *argv[]) {
  char nom1[100], nom2[100];
  FILE *fp1, *fp2;
  int m, h, k;

  if(argc < 3) {
    printf("Syntax: %s dimension samples\n", argv[0]);
    exit(1);
  }

  m = atoi(argv[1]);
  h = atoi(argv[2]);

  sprintf(nom1, "./output/.weight_%.2dD_%d.sld", m, h);
  sprintf(nom2, "./output/weight_%.2dD_%d.sld", m, h);

  open_file(&fp1, nom1, "w+"); 

  k = simplex_lattice_design(fp1, m, h);

  open_file(&fp2, nom2, "w"); 

  fprintf(fp2, "# %d %d\n", k, m);

  copy_file(fp1, fp2);
 
  fclose(fp1);
  fclose(fp2);

  if(remove(nom1) == -1) {
    printf("Error to remove file %s.", nom1);
    exit(1);
  }

  return 0;
}
