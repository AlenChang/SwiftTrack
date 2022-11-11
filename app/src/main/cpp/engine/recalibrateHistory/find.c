/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * find.c
 *
 * Code generation for function 'find'
 *
 */

/* Include files */
#include "find.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void eml_find(const boolean_T x_data[], int x_size, int i_data[], int *i_size)
{
  int idx;
  int ii;
  boolean_T exitg1;
  idx = 0;
  *i_size = x_size;
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii <= x_size - 1)) {
    if (x_data[ii]) {
      idx++;
      i_data[idx - 1] = ii + 1;
      if (idx >= x_size) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }
  if (x_size == 1) {
    if (idx == 0) {
      *i_size = 0;
    }
  } else if (1 > idx) {
    *i_size = 0;
  } else {
    *i_size = idx;
  }
}

/* End of code generation (find.c) */
