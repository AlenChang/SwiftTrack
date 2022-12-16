/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ixfun.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 16-Dec-2022 15:26:49
 */

/* Include Files */
#include "ixfun.h"
#include "recalibrateHistory_emxutil.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *a
 *                const emxArray_real_T *b
 *                emxArray_real_T *c
 * Return Type  : void
 */
void expand_max(const emxArray_real_T *a, const emxArray_real_T *b,
                emxArray_real_T *c)
{
  const double *a_data;
  const double *b_data;
  double *c_data;
  int csz_idx_0;
  int i;
  int u0;
  int u1;
  b_data = b->data;
  a_data = a->data;
  u0 = a->size[0];
  u1 = b->size[0];
  if (u0 <= u1) {
    u1 = u0;
  }
  if (b->size[0] == 1) {
    csz_idx_0 = a->size[0];
  } else if (a->size[0] == 1) {
    csz_idx_0 = b->size[0];
  } else {
    csz_idx_0 = u1;
  }
  i = c->size[0];
  u0 = a->size[0];
  u1 = b->size[0];
  if (u0 <= u1) {
    u1 = u0;
  }
  if (b->size[0] == 1) {
    c->size[0] = a->size[0];
  } else if (a->size[0] == 1) {
    c->size[0] = b->size[0];
  } else {
    c->size[0] = u1;
  }
  emxEnsureCapacity_real_T(c, i);
  c_data = c->data;
  if (csz_idx_0 != 0) {
    boolean_T b1;
    boolean_T b_b;
    b_b = (a->size[0] != 1);
    b1 = (b->size[0] != 1);
    i = csz_idx_0 - 1;
    for (u0 = 0; u0 <= i; u0++) {
      c_data[u0] = fmax(a_data[b_b * u0], b_data[b1 * u0]);
    }
  }
}

/*
 * File trailer for ixfun.c
 *
 * [EOF]
 */
