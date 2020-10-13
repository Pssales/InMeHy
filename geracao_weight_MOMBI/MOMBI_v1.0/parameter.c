/* parametros.c: Procesa y hace disponibles los parametros de un programa,
 *               a partir de un archivo de configuracion.
 *
 * Seccion de Computacion - CINVESTAV
 *
 * Introduccion a la Computacion Evolutiva
 * Profesor: Carlos Coello Coello
 * Autor:    Raquel Hernandez Gomez
 * Junio 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "parameter.h"

#define CSIZE  50  /* Longitud de cadenas */
#define COMMENT 1  /* Estados de lectura posibles en archivo de configuracion */
#define PARAM   2
#define VALUE   3
#define NEWLN   4

char **param, **value;
int Np;  /* Numero de parametros */

/* Reserva memoria */
void init_param() {
  int i;

  if((param = (char **) malloc(sizeof(char *) * Np)) == NULL) {
    printf("Error al asignar memoria en parametros 1.\n");
    exit(1);
  }

  if((value = (char **) malloc(sizeof(char *) * Np)) == NULL) {
    printf("Error al asignar memoria en parametros 2.\n");
    exit(1);
  }

  for(i=0; i < Np; i++) {
    if((param[i] = (char *) malloc(sizeof(char)*CSIZE)) == NULL) {
      printf("Error al asignar memoria en parametros 3.\n");
      exit(1);
    }
  }

  for(i=0; i < Np; i++) {
    if((value[i] = (char *) malloc(sizeof(char)*CSIZE)) == NULL) {
      printf("Error al asignar memoria en parametros 4.\n");
      exit(1);
    }
  }
}

/* Libera memoria */
void free_param() {
  int i;

  for(i=0; i < Np; i++) {
    free(param[i]);
    free(value[i]);
  }
  free(param);
  free(value);
}

/* Contabiliza parametros de un archivo */
int count_param(FILE *fp) {
  int c, flag, i = 0;

  flag = NEWLN;
 
  while((c = fgetc(fp)) != EOF) {

    if(c == '\n') {
      flag = NEWLN;
      continue;
    }

    if(c == ' ' || c == '\t' || c == '\r') /* Ignora espacios */
      continue;

    if(flag == NEWLN) {
      flag = 0;

      if(c != '#') 
        i++;
    }
  }

  rewind(fp);
  return i;
} 

/* Procesa archivo de configuracion. Argumentos:
 * arch: Nombre del archivo
 * n:    Numero de parametros
 */
void read_param(char *arch) {
  int i, j, c, flag;
  FILE * fp;
 
  if((fp = fopen(arch, "r")) == NULL) {
    printf("Error al abrir archivo de configuracion: %s\n", arch);
    exit(1);
  }

  Np = count_param(fp);
  //printf("numero de parametros %d\n", Np);
  init_param();
  flag = PARAM;
  i = j = 0;
 
  while((c = fgetc(fp)) != EOF) {

    if(c == '\n') {  /* Nueva linea */

      if(flag == VALUE) {  /* Termina de leer el valor de un parametro */
        value[i][j] = '\0';
        i++;
      }
      flag = PARAM;
      j = 0;
      continue;
    }

    if(c == ' ' || c == '\t' || c == '\r') /* Ignora espacios */
      continue;

    if(flag == COMMENT)  /* Ignora comentarios */
     continue;

    if(c == '#') {  /* Comentarios inician con simbolo # */
      flag = COMMENT;
      continue;
    }

    if(c == '=') {  /* Finaliza lectura del nombre de parametro */
      param[i][j] = '\0';
      flag = VALUE;
      j = 0;
      continue;
    }

    /* Establece caracter por caracter nombres de parametros y valores */
    if(flag == PARAM) 
      param[i][j++] = c;

    if(flag == VALUE)
      value[i][j++] = c;
  }
  fclose(fp);
}

/* Identifica la posicion que ocupa un parametro en arreglos globales */
int find_param(char *s) {
  int i;

  for(i=0; i < Np; i++) 
    if(strcmp(s, param[i]) == 0)
      return i;
  
  printf("Error, parametro no encontrado: %s.\n", s);
  free_param();
  exit(1);
}

/* Obtiene un parametro de acuerdo al tipo de dato definido */
void getInt(char *s, int *v) {
  int i;

  i = find_param(s);
  *v = atoi(value[i]);
}

void getDouble(char *s, double *v) {
  int i;

  i = find_param(s);
  *v = atof(value[i]);
}

void getString(char *s, char **v) {
  int i;

  i = find_param(s);
  strcpy(*v, value[i]);
}

void valida_lim(char *s, int n, int i) {

  if(i < 1 || i > n) {
    printf("Error, indice %d fuera de rango en arreglo %s.\n", i, s);
    free_param();
    exit(1);
  }
}

/* Obtiene un parametro de tipo arrego de longitud n.

   Sintaxis:

   Rmin:* = 0    Inicializa todo el arreglo Rmin con cero
   Rmin:3 = 4    Inicializa Rmin en la posicion 2
   Rmin:2-n = 3  Inicializa Rmin en las posiciones 1 a [n-1]
*/
void getArrayDouble(char *s, double *v, int n) {
  char *idx, *tok;
  int i, j, k, l, flag = 0;

  l = strlen(s);

  for(i = 0; i < n; i++)
    v[i] = -DBL_MAX;
 
  for(i = 0; i < Np; i++) {
    if(strncmp(s, param[i], l) == 0) {
      flag = 1;

      idx = param[i];
      idx += l + 1;

      if(strcmp(idx, "*") == 0) {
        for(j = 0; j < n; j++)
          v[j] = atof(value[i]); 
        return;
      }
      else {
        tok = strtok(idx, "-");
        j = atoi(tok);
        valida_lim(s, n, j);

        tok = strtok(NULL, "-");

        if(tok != NULL) {
          if(strcmp(tok, "n")) {
            k = atoi(tok);
            valida_lim(s, n, k);
          }
          else 
            k = n;

          if(strtok(NULL, "-") != NULL) {
            printf("Error, sintaxis invalida en arreglo %s.\n", s);
            free_param();
            exit(1);
          }
 
          for(; j <= k; j++)
            v[j-1] = atof(value[i]); 
        }
        else 
          v[j-1] = atof(value[i]); 
      }
    }
  }

  if(flag == 0) {
    printf("Error, arreglo %s no encontrado.\n", s);
    free_param();
    exit(1);
  }

  for(i = 0; i < n; i++) {
    if(v[i] == -DBL_MAX) {
      printf("Error, arreglo %s en posicion %d no inicializado.\n", s, (i+1));
      free_param();
      exit(1);
    }
  }
}

