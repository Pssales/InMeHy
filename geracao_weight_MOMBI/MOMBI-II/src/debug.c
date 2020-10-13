
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "debug.h"

void get_localtime(char *str) {
  time_t timer;
  struct tm* tm_info;

  time(&timer);
  tm_info = localtime(&timer);

  if(tm_info == NULL)
    sprintf(str, "%s", "time is not available");
  else
    strftime(str, 26, "%Y/%m/%d %H:%M:%S", tm_info);
}

void EMO_Debug_alloc(EMO_Debug *dbg, unsigned int level, unsigned int rank, const char *file) {

  dbg->rank = rank;
  dbg->level = level;
  
  if(strcmp(file, "stdout") == 0) {
    dbg->fp = stdout;
    dbg->name = NULL;
  }
  else {
    if((dbg->name = (char *) malloc(sizeof(char) * (strlen(file) + 1) )) == NULL) {
      printf("Error, not enough memory in debug.c %s\n", file);
      exit(1);
    }

    strcpy(dbg->name, file);

    if((dbg->fp = fopen(dbg->name, "w")) == NULL) {
      printf("Error to create the file %s\n", dbg->name);
      exit(1);
    }
  }

  dbg->line = 1;
  EMO_Debug_printf(dbg, "Start of the debug");
}

void EMO_Debug_free(EMO_Debug *dbg) {
  EMO_Debug_printf(dbg, "End of the debug");

  if(dbg != NULL)
    fclose(dbg->fp);

  if(dbg->name != NULL)
    free(dbg->name);
}

void EMO_Debug_rename(EMO_Debug *dbg, const char *file) {

  if(rename(dbg->name, file) != 0) {
    printf("Error in renaming the file %s by %s\n", dbg->name, file);
    exit(1);
  }

  if(dbg != NULL)
    fclose(dbg->fp);

  if(dbg->name != NULL)
    free(dbg->name);
 
  if(strcmp(file, "stdout") == 0) {
    dbg->fp = stdout;
    dbg->name = NULL;
  }
  else {
    if((dbg->name = (char *) malloc(sizeof(char) * (strlen(file) + 1) )) == NULL) {
      printf("Error, not enough memory in debug.c %s\n", file);
      exit(1);
    }

    strcpy(dbg->name, file);

    if((dbg->fp = fopen(dbg->name, "a")) == NULL) {
      printf("Error to create the file %s\n", dbg->name);
      exit(1);
    }
  }
}

void EMO_Debug_printf(EMO_Debug *dbg, const char *fmt, ...) {
  va_list ap;
  FILE *fp;

  fp = (dbg == NULL)? stdout : dbg->fp;

  if(dbg != NULL && dbg->level == EMO_DEBUG_OFF) 
    return;

  if(dbg != NULL) {
    get_localtime(dbg->buf);

    #ifdef EMO_MPI
      fprintf(fp, "%d|%s|pid %d => ", dbg->line++, dbg->buf, dbg->rank);
    #else
      fprintf(fp, "%d|%s => ", dbg->line++, dbg->buf);
    #endif
  }

  va_start(ap, fmt);
  vfprintf(fp, fmt, ap);
  va_end(ap);

  fprintf(fp, "\n");
  fflush(fp);
}

/* Prints a vector */
void EMO_Debug_printv(EMO_Debug *dbg, double *x, int n, const char *fmt, ...) {
  va_list ap;
  FILE *fp;
  int i;

  fp = (dbg == NULL)? stdout : dbg->fp;

  if(dbg != NULL && dbg->level == EMO_DEBUG_OFF) 
    return;

  if(dbg != NULL) {
    get_localtime(dbg->buf);

    #ifdef EMO_MPI
      fprintf(fp, "%d|%s|pid %d => ", dbg->line++, dbg->buf, dbg->rank);
    #else
      fprintf(fp, "%d|%s => ", dbg->line++, dbg->buf);
    #endif
  }

  va_start(ap, fmt);
  vfprintf(fp, fmt, ap);
  va_end(ap);

  fprintf(fp, " (");
  for(i = 0; i < n; i++)
    fprintf(fp, " %f", x[i]);
 
  fprintf(fp, ")\n");
  fflush(fp);
}

