/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: find.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 02-Dec-2022 12:50:48
 */

/* Include Files */
#include "find.h"
#include "recalibrateHistory_emxutil.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : emxArray_int32_T *in1
 *                const emxArray_real_T *in2
 *                const emxArray_int32_T *in3
 *                const emxArray_real_T *in4
 * Return Type  : void
 */
void e_binary_expand_op(emxArray_int32_T *in1, const emxArray_real_T *in2,
                        const emxArray_int32_T *in3, const emxArray_real_T *in4)
{
  emxArray_boolean_T *b_in2;
  const double *in2_data;
  const double *in4_data;
  const int *in3_data;
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  boolean_T *b_in2_data;
  in4_data = in4->data;
  in3_data = in3->data;
  in2_data = in2->data;
  emxInit_boolean_T(&b_in2, 1);
  i = b_in2->size[0];
  if (in4->size[0] == 1) {
    b_in2->size[0] = in3->size[0];
  } else {
    b_in2->size[0] = in4->size[0];
  }
  emxEnsureCapacity_boolean_T(b_in2, i);
  b_in2_data = b_in2->data;
  stride_0_0 = (in3->size[0] != 1);
  stride_1_0 = (in4->size[0] != 1);
  if (in4->size[0] == 1) {
    loop_ub = in3->size[0];
  } else {
    loop_ub = in4->size[0];
  }
  for (i = 0; i < loop_ub; i++) {
    b_in2_data[i] =
        (in2_data[in3_data[i * stride_0_0] - 1] - in4_data[i * stride_1_0] >=
         0.1);
  }
  eml_find(b_in2, in1);
  emxFree_boolean_T(&b_in2);
}

/*
 * Arguments    : const emxArray_boolean_T *x
 *                emxArray_int32_T *i
 * Return Type  : void
 */
void eml_find(const emxArray_boolean_T *x, emxArray_int32_T *i)
{
  int idx;
  int ii;
  int nx;
  int *i_data;
  const boolean_T *x_data;
  boolean_T exitg1;
  x_data = x->data;
  nx = x->size[0];
  idx = 0;
  ii = i->size[0];
  i->size[0] = x->size[0];
  emxEnsureCapacity_int32_T(i, ii);
  i_data = i->data;
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx - 1)) {
    if (x_data[ii]) {
      idx++;
      i_data[idx - 1] = ii + 1;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }
  if (x->size[0] == 1) {
    if (idx == 0) {
      i->size[0] = 0;
    }
  } else {
    ii = i->size[0];
    if (idx < 1) {
      i->size[0] = 0;
    } else {
      i->size[0] = idx;
    }
    emxEnsureCapacity_int32_T(i, ii);
  }
}

/*
 * File trailer for find.c
 *
 * [EOF]
 */
