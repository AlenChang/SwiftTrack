/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: find.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 02-Dec-2022 11:02:26
 */

/* Include Files */
#include "find.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : int in1_data[]
 *                int *in1_size
 *                const double in2[4096]
 *                const int in3_data[]
 *                const int *in3_size
 *                const double in4_data[]
 *                const int *in4_size
 * Return Type  : void
 */
void c_binary_expand_op(int in1_data[], int *in1_size, const double in2[4096],
                        const int in3_data[], const int *in3_size,
                        const double in4_data[], const int *in4_size)
{
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  boolean_T in2_data[4096];
  stride_0_0 = (*in3_size != 1);
  stride_1_0 = (*in4_size != 1);
  if (*in4_size == 1) {
    loop_ub = *in3_size;
  } else {
    loop_ub = *in4_size;
  }
  for (i = 0; i < loop_ub; i++) {
    in2_data[i] =
        (in2[in3_data[i * stride_0_0] - 1] - in4_data[i * stride_1_0] >= 0.1);
  }
  if (*in4_size == 1) {
    i = *in3_size;
  } else {
    i = *in4_size;
  }
  eml_find(in2_data, i, in1_data, in1_size);
}

/*
 * Arguments    : const boolean_T x_data[]
 *                int x_size
 *                int i_data[]
 *                int *i_size
 * Return Type  : void
 */
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
  } else if (idx < 1) {
    *i_size = 0;
  } else {
    *i_size = idx;
  }
}

/*
 * File trailer for find.c
 *
 * [EOF]
 */
