/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: eml_mtimes_helper.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 07-Feb-2023 10:03:28
 */

/* Include Files */
#include "eml_mtimes_helper.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *in1
 *                const emxArray_real_T *in2
 *                const emxArray_real_T *in3
 *                const emxArray_int32_T *in4
 *                const emxArray_int32_T *in5
 * Return Type  : void
 */
void c_binary_expand_op(emxArray_real_T *in1, const emxArray_real_T *in2,
                        const emxArray_real_T *in3, const emxArray_int32_T *in4,
                        const emxArray_int32_T *in5)
{
  const double *in2_data;
  const double *in3_data;
  double *in1_data;
  const int *in4_data;
  const int *in5_data;
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  in5_data = in5->data;
  in4_data = in4->data;
  in3_data = in3->data;
  in2_data = in2->data;
  in1_data = in1->data;
  stride_0_0 = (in4->size[0] != 1);
  stride_1_0 = (in5->size[0] != 1);
  if (in5->size[0] == 1) {
    loop_ub = in4->size[0];
  } else {
    loop_ub = in5->size[0];
  }
  for (i = 0; i < loop_ub; i++) {
    in1_data[((int)in2_data[i] + in1->size[0]) - 1] =
        0.5 * (in3_data[in4_data[i * stride_0_0] - 1] +
               in3_data[in5_data[i * stride_1_0] - 1]);
  }
}

/*
 * Arguments    : emxArray_real_T *in1
 *                const emxArray_real_T *in2
 *                const emxArray_real_T *in3
 *                const emxArray_int32_T *in4
 *                const emxArray_int32_T *in5
 * Return Type  : void
 */
void d_binary_expand_op(emxArray_real_T *in1, const emxArray_real_T *in2,
                        const emxArray_real_T *in3, const emxArray_int32_T *in4,
                        const emxArray_int32_T *in5)
{
  const double *in2_data;
  const double *in3_data;
  double *in1_data;
  const int *in4_data;
  const int *in5_data;
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  in5_data = in5->data;
  in4_data = in4->data;
  in3_data = in3->data;
  in2_data = in2->data;
  in1_data = in1->data;
  stride_0_0 = (in4->size[0] != 1);
  stride_1_0 = (in5->size[0] != 1);
  if (in5->size[0] == 1) {
    loop_ub = in4->size[0];
  } else {
    loop_ub = in5->size[0];
  }
  for (i = 0; i < loop_ub; i++) {
    in1_data[(int)in2_data[i] - 1] =
        0.5 * (in3_data[in4_data[i * stride_0_0] - 1] +
               in3_data[in5_data[i * stride_1_0] - 1]);
  }
}

/*
 * File trailer for eml_mtimes_helper.c
 *
 * [EOF]
 */
