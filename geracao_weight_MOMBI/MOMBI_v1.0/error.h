/**************************************************************
 * error.h      Code error definitions.                       *
 *                                                            *
 * Computer Science Department, CINVESTAV-IPN                 *
 *                                                            *
 * Professor:   Dr. Carlos A. Coello Coello                   *
 * Author:      Raquel Hernandez Gomez                        *
 *                                                            *
 * March 2013                                                 *
 *************************************************************/

#ifndef _ERROR_
#define _ERROR_

#define GENERIC  0   /* Generic error */
#define ENOMEM   1   /* No memory */
#define ENOFUNC  2   /* Function not found */
#define ENOFILE  3   /* Error to open a file */
#define EREAD    4   /* Error to read a file */
#define EWRITE   5   /* Error to write a file */
#define EARGS    6   /* Wrong arguments */
#define MISMATCH 7   /* Mismatched values */
#define EMPTYLST 8   /* Empty list */
#define IDXBOUND 9   /* Index out of bounds */
#define NOFOUND 10   /* Item not found */
#define SYNTAX  11   /* Invalid syntax */
#define EINIT   12   /* Element not initialized */

void error(int code, const char *s, const char *f);

#endif
