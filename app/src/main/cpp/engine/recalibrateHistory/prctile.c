/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: prctile.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 29-Jan-2023 18:06:51
 */

/* Include Files */
#include "prctile.h"
#include "recalibrateHistory_emxutil.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_roundd_snf(double u);

/* Function Definitions */
/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }
  return y;
}

/*
 * Arguments    : const emxArray_real_T *x
 * Return Type  : double
 */
double b_prctile(const emxArray_real_T *x)
{
  emxArray_int32_T *idx;
  const double *x_data;
  double y;
  int *idx_data;
  x_data = x->data;
  if (x->size[0] == 0) {
    y = rtNaN;
  } else {
    int nj;
    emxInit_int32_T(&idx);
    sortIdx(x, idx);
    idx_data = idx->data;
    nj = x->size[0];
    while ((nj > 0) && rtIsNaN(x_data[idx_data[nj - 1] - 1])) {
      nj--;
    }
    if (nj < 1) {
      y = rtNaN;
    } else if (nj == 1) {
      y = x_data[idx_data[0] - 1];
    } else {
      double r;
      int i;
      r = 0.95 * (double)nj;
      i = (int)rt_roundd_snf(r);
      if (i >= nj) {
        y = x_data[idx_data[nj - 1] - 1];
      } else {
        r -= (double)i;
        y = (0.5 - r) * x_data[idx_data[i - 1] - 1] +
            (r + 0.5) * x_data[idx_data[i] - 1];
      }
    }
    emxFree_int32_T(&idx);
  }
  return y;
}

/*
 * Arguments    : const emxArray_real_T *x
 * Return Type  : double
 */
double prctile(const emxArray_real_T *x)
{
  emxArray_int32_T *idx;
  const double *x_data;
  double y;
  int *idx_data;
  x_data = x->data;
  if (x->size[0] == 0) {
    y = rtNaN;
  } else {
    int nj;
    emxInit_int32_T(&idx);
    sortIdx(x, idx);
    idx_data = idx->data;
    nj = x->size[0];
    while ((nj > 0) && rtIsNaN(x_data[idx_data[nj - 1] - 1])) {
      nj--;
    }
    if (nj < 1) {
      y = rtNaN;
    } else if (nj == 1) {
      y = x_data[idx_data[0] - 1];
    } else {
      double r;
      int i;
      r = 0.05 * (double)nj;
      i = (int)rt_roundd_snf(r);
      if (i < 1) {
        y = x_data[idx_data[0] - 1];
      } else if (i >= nj) {
        y = x_data[idx_data[nj - 1] - 1];
      } else {
        r -= (double)i;
        y = (0.5 - r) * x_data[idx_data[i - 1] - 1] +
            (r + 0.5) * x_data[idx_data[i] - 1];
      }
    }
    emxFree_int32_T(&idx);
  }
  return y;
}

/*
 * File trailer for prctile.c
 *
 * [EOF]
 */
