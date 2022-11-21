/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ixfun.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 18-Nov-2022 21:41:51
 */

/* Include Files */
#include "ixfun.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

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
  int i;
  int k;
  int varargin_3;
  int varargin_5;
  short csz_idx_0;
  boolean_T b;
  boolean_T b1;
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
    b = (a_size != 1);
    b1 = (b_size != 1);
    i = csz_idx_0 - 1;
    for (k = 0; k <= i; k++) {
      varargin_3 = b * k;
      varargin_5 = b1 * k;
      if ((a_data[varargin_3] >= b_data[varargin_5]) ||
          rtIsNaN(b_data[varargin_5])) {
        c_data[k] = a_data[varargin_3];
      } else {
        c_data[k] = b_data[varargin_5];
      }
    }
  }
}

/*
 * File trailer for ixfun.c
 *
 * [EOF]
 */
