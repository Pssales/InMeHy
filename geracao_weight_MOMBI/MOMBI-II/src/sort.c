
#include <float.h>
#include <math.h>

/* Auxiliar function for sorting individuals in ascending order */
int EMO_compare_asc(const void **a, const void **b) {
  double *v1, *v2;

  v1 = (double *) *a;
  v2 = (double *) *b;

  if(v1[1] < v2[1]) return -1;
  else if(v1[1] > v2[1]) return 1;

  return 0;
}

/* Auxiliar function for sorting individuals in descending order */
int EMO_compare_desc(const void **a, const void **b) {
  double *v1, *v2;

  v1 = (double *) *a;
  v2 = (double *) *b;

  if(v1[1] > v2[1]) return -1;
  else if(v1[1] < v2[1]) return 1;

  return 0;
}


int EMO_compare_rows(const void *a, const void *b, int start, int n) {
  double *v1, *v2, max1, max2;
  int i, idx1 = 0, idx2 = 0;

  v1 = (double *) a;
  v2 = (double *) b;

  max1 = max2 = DBL_MIN;

  for(i = start; i < n; i++) {
    if(v1[i] > max1) {
      max1 = v1[i];
      idx1 = i;
    }

    if(v2[i] > max2) {
      max2 = v2[i];
      idx2 = i;
    }
  }

  if(idx1 < idx2)
    return -1;

  if(idx2 < idx1)
    return 1;

  return 0;
}


int EMO_compare_rows2(const void *a, const void *b, int start, int n) {
  double *v1, *v2, s1, s2;
  int i, c1, c2; //, idx1, idx2;

  v1 = (double *) a;
  v2 = (double *) b;

  s1 = s2 = 0.0;
  c1 = c2 = 0;

  for(i = start; i < n; i++) {
    if(fabs(v1[i]) > 1e-3) {
      //s1 += pow(2.0, (double) i);
      s1 += (double) i;
      c1 ++;
    }

    if(fabs(v2[i]) > 1e-3) {
      //s2 += pow(2.0, (double) i);
      s2 += (double) i;
      c2 ++;
    }
  }

  s1 /= c1;
  s2 /= c2;

  if(s1 < s2)
    return -1;

  if(s2 < s1)
    return 1;

  return 0;
}


/* Copyright (C) 1991,1992,1996,1997,1999,2004 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Written by Douglas C. Schmidt (schmidt@ics.uci.edu).

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* If you consider tuning this algorithm, you should consult first:
   Engineering a sort function; Jon Bentley and M. Douglas McIlroy;
   Software - Practice and Experience; Vol. 23 (11), 1249-1265, 1993.  */

#include <alloca.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/* Byte-wise swap two items of size SIZE. */
#define SWAP(a, b, size)                                                      \
  do                                                                          \
    {                                                                         \
      register size_t __size = (size);                                        \
      register char *__a = (a), *__b = (b);                                   \
      do                                                                      \
        {                                                                     \
          char __tmp = *__a;                                                  \
          *__a++ = *__b;                                                      \
          *__b++ = __tmp;                                                     \
        } while (--__size > 0);                                               \
    } while (0)

/* Discontinue quicksort algorithm when partition gets below this size.
   This particular magic number was chosen to work best on a Sun 4/260. */
#define MAX_THRESH 4

/* Stack node declarations used to store unfulfilled partition obligations. */
typedef struct
  {
    char *lo;
    char *hi;
  } stack_node;

/* The next 4 #defines implement a very fast in-line stack abstraction. */
/* The stack needs log (total_elements) entries (we could even subtract
   log(MAX_THRESH)).  Since total_elements has type size_t, we get as
   upper bound for log (total_elements):
   bits per byte (CHAR_BIT) * sizeof(size_t).  */
#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif
#define STACK_SIZE        (CHAR_BIT * sizeof(size_t))
#define PUSH(low, high)        ((void) ((top->lo = (low)), (top->hi = (high)), ++top))
#define        POP(low, high)        ((void) (--top, (low = top->lo), (high = top->hi)))
#define        STACK_NOT_EMPTY        (stack < top)


/* Order size using quicksort.  This implementation incorporates
   four optimizations discussed in Sedgewick:

   1. Non-recursive, using an explicit stack of pointer that store the
      next array partition to sort.  To save time, this maximum amount
      of space required to store an array of SIZE_MAX is allocated on the
      stack.  Assuming a 32-bit (64 bit) integer for size_t, this needs
      only 32 * sizeof(stack_node) == 256 bytes (for 64 bit: 1024 bytes).
      Pretty cheap, actually.

   2. Chose the pivot element using a median-of-three decision tree.
      This reduces the probability of selecting a bad pivot value and
      eliminates certain extraneous comparisons.

   3. Only quicksorts TOTAL_ELEMS / MAX_THRESH partitions, leaving
      insertion sort to order the MAX_THRESH items within each partition.
      This is a big win, since insertion sort is faster for small, mostly
      sorted array segments.

   4. The larger of the two sub-partitions is always pushed onto the
      stack first, with the algorithm then concentrating on the
      smaller partition.  This *guarantees* no more than log (total_elems)
      stack size is needed (actually O(1) in this case)!  */

/* Example of use:
    double *x; // bidimentional array of tot_elem * dim
    start = 0;
    ...
    EMO_quicksort(x, tot_elem, start, dim, sizeof(double), compare_vector);

    int compare_vector(const void *p1, const void *p2, size_t start, size_t dim) {
      int i;

      double *x1 = (double *)p1;
      double *x2 = (double *)p2;

      for(i = start; i < dim; i++) {
        if(x1[i] < x2[i])
          return 1;
        if(x1[i] > x2[i])
          return -1;
       }

       return 0;
    }

 */
void EMO_quicksort(void *const pbase, int total_elems, int start, int dimension, size_t size, int(*cmp)(const void *, const void *, int, int))
//__compar_fn_t cmp)
{
  size_t max_thresh;
  register char *base_ptr = (char *) pbase;

  max_thresh = MAX_THRESH * size;

  if (total_elems == 0)
    /* Avoid lossage with unsigned arithmetic below.  */
    return;

  if (total_elems > MAX_THRESH)
    {
      char *lo = base_ptr;
      char *hi = &lo[size * (total_elems - 1)];
      stack_node stack[STACK_SIZE];
      stack_node *top = stack;

      PUSH (NULL, NULL);

      while (STACK_NOT_EMPTY)
        {
          char *left_ptr;
          char *right_ptr;

          /* Select median value from among LO, MID, and HI. Rearrange
             LO and HI so the three values are sorted. This lowers the
             probability of picking a pathological pivot value and
             skips a comparison for both the LEFT_PTR and RIGHT_PTR in
             the while loops. */

          char *mid = lo + size * ((hi - lo) / size >> 1);
          //printf("lo %p, mi %p, hi %p\n", lo, mid, hi);
          if ((*cmp) ((void *) mid, (void *) lo, start, dimension) < 0)
            SWAP (mid, lo, size);

          if ((*cmp) ((void *) hi, (void *) mid, start, dimension) < 0)
            SWAP (mid, hi, size);
          else
            goto jump_over;
          if ((*cmp) ((void *) mid, (void *) lo, start, dimension) < 0)
            SWAP (mid, lo, size);
        jump_over:;

          left_ptr  = lo + size;
          right_ptr = hi - size;


          /* Here's the famous ``collapse the walls'' section of quicksort.
             Gotta like those tight inner loops!  They are the main reason
             that this algorithm runs much faster than others. */
          do
            {

              while ((*cmp) ((void *) left_ptr, (void *) mid, start, dimension) < 0)
                left_ptr += size;

              while ((*cmp) ((void *) mid, (void *) right_ptr, start, dimension) < 0)
                right_ptr -= size;

              if (left_ptr < right_ptr)
                {
                  SWAP (left_ptr, right_ptr, size);
                  if (mid == left_ptr)
                    mid = right_ptr;
                  else if (mid == right_ptr)
                    mid = left_ptr;
                  left_ptr += size;
                  right_ptr -= size;
                }
              else if (left_ptr == right_ptr)
                {
                  left_ptr += size;
                  right_ptr -= size;
                  break;
                }
            }
          while (left_ptr <= right_ptr);

          /* Set up pointers for next iteration.  First determine whether
             left and right partitions are below the threshold size.  If so,
             ignore one or both.  Otherwise, push the larger partition's
             bounds on the stack and continue sorting the smaller one. */

          if ((size_t) (right_ptr - lo) <= max_thresh)
            {
              if ((size_t) (hi - left_ptr) <= max_thresh)
                /* Ignore both small partitions. */
                POP (lo, hi);
              else
                /* Ignore small left partition. */
                lo = left_ptr;
            }
          else if ((size_t) (hi - left_ptr) <= max_thresh)
            /* Ignore small right partition. */
            hi = right_ptr;
          else if ((right_ptr - lo) > (hi - left_ptr))
            {
              /* Push larger left partition indices. */
              PUSH (lo, right_ptr);
              lo = left_ptr;
            }
          else
            {
              /* Push larger right partition indices. */
              PUSH (left_ptr, hi);
              hi = right_ptr;
            }
        }
    }

  /* Once the BASE_PTR array is partially sorted by quicksort the rest
     is completely sorted using insertion sort, since this is efficient
     for partitions below MAX_THRESH size. BASE_PTR points to the beginning
     of the array to sort, and END_PTR points at the very last element in
     the array (*not* one beyond it!). */

#define min(x, y) ((x) < (y) ? (x) : (y))

  {
    char *const end_ptr = &base_ptr[size * (total_elems - 1)];
    char *tmp_ptr = base_ptr;
    char *thresh = min(end_ptr, base_ptr + max_thresh);
    register char *run_ptr;

    /* Find smallest element in first threshold and place it at the
       array's beginning.  This is the smallest array element,
       and the operation speeds up insertion sort's inner loop. */

    for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size)
      if ((*cmp) ((void *) run_ptr, (void *) tmp_ptr, start, dimension) < 0)
        tmp_ptr = run_ptr;

    if (tmp_ptr != base_ptr)
      SWAP (tmp_ptr, base_ptr, size);

    /* Insertion sort, running from left-hand-side up to right-hand-side.  */

    run_ptr = base_ptr + size;
    while ((run_ptr += size) <= end_ptr)
      {
        tmp_ptr = run_ptr - size;
        while ((*cmp) ((void *) run_ptr, (void *) tmp_ptr, start, dimension) < 0)
          tmp_ptr -= size;

        tmp_ptr += size;
        if (tmp_ptr != run_ptr)
          {
            char *trav;

            trav = run_ptr + size;
            while (--trav >= run_ptr)
              {
                char c = *trav;
                char *hi, *lo;

                for (hi = lo = trav; (lo -= size) >= tmp_ptr; hi = lo)
                  *hi = *lo;
                *hi = c;
              }
          }
      }
  }
}


int EMO_binsert(const void *A, const void *key, int total_elems, int start, int dimension, size_t size, int(*cmp)(const void *, const void *, int, int)) {
  void *base = (void *) A;
  int imin, imax, imid, c;

  c = imid = 0;
  imin = 0;
  imax = total_elems - 1;
  size = size * dimension;


  while (imax >= imin) {                     // continue searching while [imin,imax] is not empty
    imid = imin + (int) (imax - imin) / 2;   // calculate the midpoint for roughly equal partition

    c = (*cmp)(key, (void *) base + imid * size, start, dimension);

    if(imax == imin && c < 0)                // key should be inserted at imid
      return imid;
    else if (c < 0)
      imax = imid - 1;                       // change max index to search lower subarray
    else if (c >= 0)                 
      imin = imid + 1;                       // change min index to search upper subarray
   }

  return (c < 0)? imid : imid + 1;
}

