#include <stdlib.h>
#include <stdio.h>

#include "emo.h"

int main(int argc, char **argv) {
  int row, col, *nd;
  char str[2000];
  double *data;

  if(argc != 2) {
    printf("Syntax: %s file\n", argv[0]);
    exit(1);
  }

  row = 0;
  col = 0;

  data = EMO_File_read(NULL, &row, &col, argv[1], 0);

  nd = (int *) malloc(sizeof(int) * row);
  EMO_Dominance_ndset(nd, data, row, col, EMO_Dominance_strict);

  sprintf(str, "%s.nd", argv[1]);
  EMO_File_write(data, nd, row, col, str, "%e ");

  free(nd);
  free(data);

  return 0;
}

