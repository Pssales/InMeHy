
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "emo.h"

#define _MAXCHAR 2000
#define _HVWFG 1 
#define _HVFON 2 

void filename(char **tmp, const char *prefix, int run) {
  if(strstr(prefix, ".") == NULL)
    sprintf(*tmp, "%s_R%.2d.pof", prefix, run);
  else
    sprintf(*tmp, "%s", prefix);
}

void savefile(double *v, int run, char **tmp, const char *prefix, const char *extension) {
  sprintf(*tmp, "%s.%s", prefix, extension);
  printf("Data stored in %s\n", *tmp);
  EMO_File_write(v, NULL, run, 1, *tmp, "%e ");
}

int hv(double *v, char **str, int nrun, int argc, char **argv, int type) {
  int r, row, col, *filter = NULL;
  double *data, *ref;
  EMO_HV wfg;
 
  row = col = 0;

  filename(str, argv[2], 1);
  data = EMO_File_read(NULL, &row, &col, *str, 0);

  if(argc != (4 + col)) {
    printf("Syntax: %s %s {prefix,file} number_of_runs reference_point\n", argv[0], argv[1]);
    return 1;
  }

  if((ref = (double *) malloc(sizeof(double) * col)) == NULL) {
    printf("Error, not enough memory in hv function\n");
    return 1;
  }

  if(type == _HVWFG) {
    if((filter = (int *) calloc(sizeof(int), row)) == NULL) {
      printf("Error, not enough memory in hv function\n");
      return 1;
    }

    EMO_Dominance_ndset(filter, data, row, col, EMO_Dominance_strict);
    EMO_HV_alloc(&wfg, row, col);
  }

  for(r = 0; r < col; r++)
    ref[r] = atof(argv[4 + r]);

  for(r = 1; r <= nrun;) {

    if(type == _HVWFG) 
      v[r-1] = EMO_hv2(data, row, ref, col);
    else
      v[r-1] = EMO_HV_run(&wfg, data, filter, row, ref);

    printf("%d %s %f\n", r, *str, v[r-1]);
    free(data);

    if(type == _HVWFG)
      free(filter);

    if(++r <= nrun) {
      row = 0;
      filename(str, argv[2], r);
      data = EMO_File_read(NULL, &row, &col, *str, 0);

      if(type == _HVWFG) {
        if((filter = (int *) calloc(sizeof(int), row)) == NULL) {
          printf("Error, not enough memory in hv function\n");
          return 1;
        }

        EMO_Dominance_ndset(filter, data, row, col, EMO_Dominance_strict);
      }
    }
  }

  if(type == _HVWFG)
    EMO_HV_free(&wfg);
 
  free(ref);
  return 0;
}

int refpoint(double *v, char **str, int nrun, int argc, char **argv) {
  double (*apf)(double *, int *, int, double *, int, int, double);
  int r, row, row2, col, p;
  double *data, *ref;
 
  if(argc != 6) {
    printf("Syntax: %s %s {prefix,file} number_of_runs p_norm reference_set_file\n", argv[0], argv[1]);
    return 1;
  }

  apf = NULL;

 if(strcmp(argv[1], "GD") == 0)
   apf = EMO_gd;
 else if(strcmp(argv[1], "IGD") == 0)
   apf = EMO_igd;
 else if(strcmp(argv[1], "DELTAP") == 0)
   apf = EMO_deltap;

  row2 = col = 0;
  p = atoi(argv[4]);
  ref = EMO_File_read(NULL, &row2, &col, argv[5], 0);
  printf("Reference set file: rows %d, columns %d\n", row2, col);

  for(r = 1; r <= nrun; r++) {
    row = 0;
    filename(str, argv[2], r);
    data = EMO_File_read(NULL, &row, &col, *str, 0);
    //printf("Data file: rows %d, columns %d\n", row, col);

    v[r-1] = apf(data, NULL, row, ref, row2, col, p);
    printf("%d %s %f\n", r, *str, v[r-1]);
    free(data);
  }

  free(ref);
  return 0;
}

int igd_plus(double *v, char **str, int nrun, int argc, char **argv) {
  int r, row, row2, col;
  double *data, *ref;
 
  if(argc != 5) {
    printf("Syntax: %s %s {prefix,file} number_of_runs reference_set_file\n", argv[0], argv[1]);
    return 1;
  }

  row2 = col = 0;
  ref = EMO_File_read(NULL, &row2, &col, argv[4], 0);
  printf("Reference set file: rows %d, columns %d\n", row2, col);

  for(r = 1; r <= nrun; r++) {
    row = 0;
    filename(str, argv[2], r);
    data = EMO_File_read(NULL, &row, &col, *str, 0);
    //printf("Data file: rows %d, columns %d\n", row, col);

    v[r-1] = EMO_igd_plus(data, NULL, row, ref, row2, col);
    printf("%d %s %f\n", r, *str, v[r-1]);
    free(data);
  }

  free(ref);
  return 0;
}


int main(int argc, char **argv) {
  char *name, *str, *ext;
  int nrun, r, n;
  double *v;

  if(argc < 4) {
    printf("\nSyntax: %s INDICATOR {file, prefix} number_of_runs [options...] \n\n", argv[0]);

    printf("INDICATOR = {HV, GD, IGD, IGD+, \\Delta_p, SP}\n");
    printf("HV:        Hypervolume\n");
    printf("GD:        Generational Distance\n");
    printf("IGD:       Inverted Generational Distance\n");
    printf("IGD+:      Modified Inverted Generational Distance\n");
    printf("DELTAP: \n");
    printf("SP:        Spacing\n\n");
    return 1;
  }

  nrun = atoi(argv[3]);
  nrun = (strstr(argv[2], ".") == NULL)? nrun : 1;  // one file

  name = (char *) malloc(sizeof(char) * _MAXCHAR);
  str = (char *) malloc(sizeof(char) * _MAXCHAR);
  ext = (char *) malloc(sizeof(char) * _MAXCHAR);
  v = (double *) malloc(sizeof(double) * nrun);

  n = strlen(argv[1]);

  for(r = 0; r < n; r++)
    name[r] = toupper(argv[1][r]);
  name[n] = '\0';

  if(strcmp(name, "HV") == 0) {
    r = hv(v, &str, nrun, argc, argv, _HVWFG);
    strcpy(ext, "hv");
  }
  else if(strcmp(name, "GD") == 0) {
    r = refpoint(v, &str, nrun, argc, argv);
    strcpy(ext, "gd");
  }
  else if(strcmp(name, "IGD") == 0) {
    r = refpoint(v, &str, nrun, argc, argv);
    strcpy(ext, "igd");
  }
  else if(strcmp(name, "IGD+") == 0) {
    r = igd_plus(v, &str, nrun, argc, argv);
    strcpy(ext, "igd_plus");
  }
  else if(strcmp(name, "DELTAP") == 0) {
    r = refpoint(v, &str, nrun, argc, argv);
    strcpy(ext, "deltap");
  }

  if(r == 0 && nrun > 1)
    savefile(v, nrun, &str, argv[2], ext);

  free(v);
  free(ext);
  free(str);
  free(name);
  return r;
}

#undef _MAXCHAR 
#undef _HVWFG
#undef _HVFON


