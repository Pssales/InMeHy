/**************************************************************
 * parameter.c  Process the parameters of a configuration     *
 *              file and makes them available for a program.  * 
 *                                                            *
 * March 2013                                                 *
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#define CSIZE  1000  /* Longitud de cadenas */
#define COMMENT 1    /* Estados de lectura posibles en archivo de configuracion */
#define PARAM   2
#define VALUE   3
#define NEWLN   4

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


/* Procesa archivo de configuracion */
void parse_param(EMO_Parser *parser, FILE *fp) {
  int i, j, c, flag;
 
  flag = PARAM;
  i = j = 0;
 
  while((c = fgetc(fp)) != EOF) {

    if(c == '\n') {  /* Nueva linea */

      if(flag == VALUE) {  /* Termina de leer el valor de un parametro */
        parser->value[i][j] = '\0';
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
      parser->name[i][j] = '\0';
      flag = VALUE;
      j = 0;
      continue;
    }

    /* Establece caracter por caracter nombres de parametros y valores */
    if(flag == PARAM) 
      parser->name[i][j++] = c;

    if(flag == VALUE)
      parser->value[i][j++] = c;
  }
}

void EMO_Parser_alloc(EMO_Parser *parser, EMO_Debug *dbg, int max_param) {
  int i;

  parser->dbg = dbg;
  parser->current = 0;
  parser->size = max_param;

  if((parser->name = (char **) malloc(sizeof(char *) * parser->size)) == NULL) {
    printf("Error al asignar memoria en parametros 1.\n");
    exit(1);
  }

  if((parser->value = (char **) malloc(sizeof(char *) * parser->size)) == NULL) {
    printf("Error al asignar memoria en parametros 2.\n");
    exit(1);
  }

  for(i = parser->size-1; i > -1; i--) {
    if((parser->name[i] = (char *) malloc(sizeof(char) * CSIZE)) == NULL) {
      printf("Error al asignar memoria en parametros 3.\n");
      exit(1);
    }

    if((parser->value[i] = (char *) malloc(sizeof(char) * CSIZE)) == NULL) {
      printf("Error al asignar memoria en parametros 4.\n");
      exit(1);
    }
  }
}

/* Reserva memoria 
 * file: Nombre del archivo de configuracion */
void EMO_Parser_alloc_from_file(EMO_Parser *parser, EMO_Debug *dbg, char *file) {
  FILE * fp;
  int i;

  parser->dbg = dbg;
  EMO_Debug_printf(parser->dbg, "Reading parameter file %s", file);

  if((fp = fopen(file, "r")) == NULL) {
    printf("Error al abrir archivo de configuracion: %s\n", file);
    exit(1);
  }

  parser->size = count_param(fp);
 
  if((parser->name = (char **) malloc(sizeof(char *) * parser->size)) == NULL) {
    printf("Error al asignar memoria en parametros 1.\n");
    exit(1);
  }

  if((parser->value = (char **) malloc(sizeof(char *) * parser->size)) == NULL) {
    printf("Error al asignar memoria en parametros 2.\n");
    exit(1);
  }

  for(i = parser->size-1; i > -1; i--) {
    if((parser->name[i] = (char *) malloc(sizeof(char) * CSIZE)) == NULL) {
      printf("Error al asignar memoria en parametros 3.\n");
      exit(1);
    }

    if((parser->value[i] = (char *) malloc(sizeof(char) * CSIZE)) == NULL) {
      printf("Error al asignar memoria en parametros 4.\n");
      exit(1);
    }
  }

  parse_param(parser, fp);
  fclose(fp);
}

/* Libera memoria */
void EMO_Parser_free(EMO_Parser *parser) {
  int i;

  for(i = parser->size-1; i > -1; i--) {
    free(parser->name[i]);
    free(parser->value[i]);
  }
  free(parser->name);
  free(parser->value);
}

/* Identifica la posicion que ocupa un parametro en arreglo */
int find_param(EMO_Parser *parser, const char *s) {
  int i;

  for(i = parser->size-1; i > -1; i--) 
    if(strcmp(s, parser->name[i]) == 0)
      return i;
 
  return -1; 
}

/* Obtiene un parametro de acuerdo al tipo de dato definido */
int EMO_Parser_get_int(EMO_Parser *parser, int *v, const char *s) {
  int i;

  if((i = find_param(parser, s)) == -1)
    return 0;

  *v = atoi(parser->value[i]);

  EMO_Debug_printf(parser->dbg, "Parser:get_int:%s=%d", s, *v);
  return 1;
}

int EMO_Parser_get_double(EMO_Parser *parser, double *v, const char *s) {
  int i;

  if((i = find_param(parser, s)) == -1)
    return 0;

  *v = atof(parser->value[i]);

  EMO_Debug_printf(parser->dbg, "Parser:get_double:%s=%f", s, *v);
  return 1;
}

int EMO_Parser_get_char(EMO_Parser *parser, char *v, const char *s) {
  int i;

  if(v == NULL || (i = find_param(parser, s)) == -1)
    return 0;

  strcpy(v, parser->value[i]);

  EMO_Debug_printf(parser->dbg, "Parser:get_char:%s=%s", s, v);
  return 1;
}

void EMO_Parser_set(EMO_Parser *parser, const char *name, const char *value) {
  int i;

  i = parser->current;

  if(i >= parser->size) {
    printf("Error, not enough space in Parser (%d/%d).\n", i, parser->size);
    exit(1);
  }

  strcpy(parser->name[i], name);
  strcpy(parser->value[i], value);

  parser->current++;
}

#undef CSIZE
#undef COMMENT
#undef PARAM
#undef VALUE
#undef NEWLN

