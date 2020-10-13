
#ifndef _SORT_H
#define _SORT_H


int EMO_compare_asc(const void **a, const void **b);
int EMO_compare_desc(const void **a, const void **b);
int EMO_compare_rows(const void *a, const void *b, int start, int n);

void EMO_quicksort (void *const pbase, size_t total_elems, int start, int dimension, size_t size, int(*cmp)(const void *, const void *, int, int));
int EMO_binsert(const void *A, const void *key, int total_elems, int start, int dimension, size_t size, int(*cmp)(const void *, const void *, int, int));

#endif

