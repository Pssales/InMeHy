
/**************************************************************
 * parameter.h  Function prototypes for processing            *
 *              configuration files.                          *
 *                                                            *
 * March 2013                                                 *
 *************************************************************/

#ifndef _PARSER_
#define _PARSER_

#include "debug.h"

typedef struct {
  char **name;  /* Parameter names */
  char **value; /* Parameter value */
  int size;     /* Number of parameters */
  int current;  /* Current parameter (only for set function) */
  EMO_Debug *dbg;
} EMO_Parser;

void EMO_Parser_alloc(EMO_Parser *parser, EMO_Debug *dbg, int max_param);
void EMO_Parser_alloc_from_file(EMO_Parser *parser, EMO_Debug *dbg, char *file);
void EMO_Parser_free(EMO_Parser *parser);
int EMO_Parser_get_int(EMO_Parser *parser, int *v, const char *s);
int EMO_Parser_get_double(EMO_Parser *parser, double *v, const char *s);
int EMO_Parser_get_char(EMO_Parser *parser, char *v, const char *s);
void EMO_Parser_set(EMO_Parser *parser, const char *name, const char *value);

#endif

