/**************************************************************
 * error.c      Code error definitions.                       *
 *                                                            *
 * Computer Science Department, CINVESTAV-IPN                 *
 *                                                            *
 * Professor:   Dr. Carlos A. Coello Coello                   *
 * Author:      Raquel Hernandez Gomez                        *
 *                                                            *
 * March 2013                                                 *
 *************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "error.h"

void error(int code, const char *s, const char *f) {
  char msg[1024];

  switch(code) {
    case ENOMEM:    strcpy(msg, " in memory allocation");
                    break;
    case ENOFUNC:   strcpy(msg, ", function not found");
                    break;
    case ENOFILE:   strcpy(msg, " to open stream data");
                    break;
    case EREAD:     strcpy(msg, " to read file");
                    break;
    case EWRITE:    strcpy(msg, " to write file");
                    break;
    case EARGS:     strcpy(msg, ", wrong arguments");
                    break;
    case MISMATCH:  strcpy(msg, " mismatched ");
                    break;
    case EMPTYLST:  strcpy(msg, " empty list");
                    break;
    case IDXBOUND:  strcpy(msg, " index out of bounds");
                    break;
    case NOFOUND:   strcpy(msg, " element not found");
                    break;
    case SYNTAX:    strcpy(msg, " invalid syntax");
                    break;
    case EINIT:     strcpy(msg, " element not initialized");
                    break;
    default:        strcpy(msg, "");
  }

  printf("\n%s: error%s [%.2d]: %s\n", f, msg, code, s);
  exit(1);
}

