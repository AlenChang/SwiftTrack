/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ixfun.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 25-Nov-2022 15:36:47
 */

/* Include Files */
#include "ixfun.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const double a_data[]
 *                int a_size
 *                const double b_data[]
 *                int b_size
 *                double c_data[]
 *                int *c_size
 * Return Type  : void
 */
void expand_max(const double a_data[], int a_size, const double b_data[],
                int b_size, double c_data[], int *c_size)
{
  int k;
  short csz_idx_0;
  if (b_size == 1) {
    csz_idx_0 = (short)a_size;
  } else if (a_size == 1) {
    csz_idx_0 = (short)b_size;
  } else if (a_size <= b_size) {
    csz_idx_0 = (short)a_size;
  } else {
    csz_idx_0 = (short)b_size;
  }
  if (b_size == 1) {
    *c_size = (short)a_size;
  } else if (a_size == 1) {
    *c_size = (short)b_size;
  } else if (a_size <= b_size) {
    *c_size = (short)a_size;
  } else {
    *c_size = (short)b_size;
  }
  if (csz_idx_0 != 0) {
    int i;
    boolean_T b;
    boolean_T b1;
    b = (a_size != 1);
    b1 = (b_size != 1);
    i = csz_idx_0 - 1;
    for (k = 0; k <= i; k++) {
      c_data[k] = fmax(a_data[b * k], b_data[b1 * k]);
    }
  }
}

/*
 * File trailer for ixfun.c
 *
 * [EOF]
 */
