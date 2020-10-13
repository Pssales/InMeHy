/**************************************************************
 * io.h   Toolkit functions for input/output.                 *
 *                                                            *
 * September 2014                                             *
 *************************************************************/

double *EMO_File_read(double *data, int *row, int *col, const char *str, int start);
void EMO_File_write(double *data, int *filter, int row, int col, const char *str, const char *fmt);

