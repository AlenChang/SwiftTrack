/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: findExtents.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 29-Jan-2023 18:06:51
 */

/* Include Files */
#include "findExtents.h"
#include "eml_mtimes_helper.h"
#include "eml_setop.h"
#include "find.h"
#include "ixfun.h"
#include "recalibrateHistory_emxutil.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static void getLeftBase(const emxArray_real_T *yTemp,
                        const emxArray_int32_T *iPeak,
                        const emxArray_int32_T *iFinite,
                        const emxArray_int32_T *iInflect,
                        emxArray_int32_T *iBase, emxArray_int32_T *iSaddle);

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *yTemp
 *                const emxArray_int32_T *iPeak
 *                const emxArray_int32_T *iFinite
 *                const emxArray_int32_T *iInflect
 *                emxArray_int32_T *iBase
 *                emxArray_int32_T *iSaddle
 * Return Type  : void
 */
static void getLeftBase(const emxArray_real_T *yTemp,
                        const emxArray_int32_T *iPeak,
                        const emxArray_int32_T *iFinite,
                        const emxArray_int32_T *iInflect,
                        emxArray_int32_T *iBase, emxArray_int32_T *iSaddle)
{
  emxArray_int32_T *iValley;
  emxArray_real_T *peak;
  emxArray_real_T *valley;
  const double *yTemp_data;
  double v;
  double *peak_data;
  double *valley_data;
  const int *iFinite_data;
  const int *iInflect_data;
  const int *iPeak_data;
  int i;
  int iv;
  int j;
  int k;
  int n;
  int *iBase_data;
  int *iSaddle_data;
  int *iValley_data;
  iInflect_data = iInflect->data;
  iFinite_data = iFinite->data;
  iPeak_data = iPeak->data;
  yTemp_data = yTemp->data;
  i = iBase->size[0];
  iBase->size[0] = iPeak->size[0];
  emxEnsureCapacity_int32_T(iBase, i);
  iBase_data = iBase->data;
  n = iPeak->size[0];
  for (i = 0; i < n; i++) {
    iBase_data[i] = 0;
  }
  i = iSaddle->size[0];
  iSaddle->size[0] = iPeak->size[0];
  emxEnsureCapacity_int32_T(iSaddle, i);
  iSaddle_data = iSaddle->data;
  n = iPeak->size[0];
  for (i = 0; i < n; i++) {
    iSaddle_data[i] = 0;
  }
  emxInit_real_T(&peak, 1);
  i = peak->size[0];
  peak->size[0] = iFinite->size[0];
  emxEnsureCapacity_real_T(peak, i);
  peak_data = peak->data;
  n = iFinite->size[0];
  for (i = 0; i < n; i++) {
    peak_data[i] = 0.0;
  }
  emxInit_real_T(&valley, 1);
  i = valley->size[0];
  valley->size[0] = iFinite->size[0];
  emxEnsureCapacity_real_T(valley, i);
  valley_data = valley->data;
  n = iFinite->size[0];
  for (i = 0; i < n; i++) {
    valley_data[i] = 0.0;
  }
  emxInit_int32_T(&iValley);
  i = iValley->size[0];
  iValley->size[0] = iFinite->size[0];
  emxEnsureCapacity_int32_T(iValley, i);
  iValley_data = iValley->data;
  n = iFinite->size[0];
  for (i = 0; i < n; i++) {
    iValley_data[i] = 0;
  }
  n = -1;
  i = 0;
  j = 0;
  k = 0;
  v = rtNaN;
  iv = 1;
  while (k + 1 <= iPeak->size[0]) {
    double p;
    int isv;
    while (iInflect_data[i] != iFinite_data[j]) {
      v = yTemp_data[iInflect_data[i] - 1];
      iv = iInflect_data[i];
      if (rtIsNaN(yTemp_data[iInflect_data[i] - 1])) {
        n = -1;
      } else {
        while ((n + 1 > 0) && (valley_data[n] > v)) {
          n--;
        }
      }
      i++;
    }
    p = yTemp_data[iInflect_data[i] - 1];
    while ((n + 1 > 0) && (peak_data[n] < p)) {
      if (valley_data[n] < v) {
        v = valley_data[n];
        iv = iValley_data[n];
      }
      n--;
    }
    isv = iv;
    while ((n + 1 > 0) && (peak_data[n] <= p)) {
      if (valley_data[n] < v) {
        v = valley_data[n];
        iv = iValley_data[n];
      }
      n--;
    }
    n++;
    peak_data[n] = yTemp_data[iInflect_data[i] - 1];
    valley_data[n] = v;
    iValley_data[n] = iv;
    if (iInflect_data[i] == iPeak_data[k]) {
      iBase_data[k] = iv;
      iSaddle_data[k] = isv;
      k++;
    }
    i++;
    j++;
  }
  emxFree_int32_T(&iValley);
  emxFree_real_T(&valley);
  emxFree_real_T(&peak);
}

/*
 * Arguments    : const emxArray_real_T *y
 *                const emxArray_real_T *x
 *                emxArray_int32_T *iPk
 *                const emxArray_int32_T *iFin
 *                const emxArray_int32_T *iInf
 *                const emxArray_int32_T *iInflect
 *                emxArray_real_T *bPk
 *                emxArray_real_T *bxPk
 *                emxArray_real_T *byPk
 *                emxArray_real_T *wxPk
 * Return Type  : void
 */
void findExtents(const emxArray_real_T *y, const emxArray_real_T *x,
                 emxArray_int32_T *iPk, const emxArray_int32_T *iFin,
                 const emxArray_int32_T *iInf, const emxArray_int32_T *iInflect,
                 emxArray_real_T *bPk, emxArray_real_T *bxPk,
                 emxArray_real_T *byPk, emxArray_real_T *wxPk)
{
  emxArray_boolean_T *b_yFinite;
  emxArray_int32_T *b_x;
  emxArray_int32_T *c_x;
  emxArray_int32_T *iInfL;
  emxArray_int32_T *iInfR;
  emxArray_int32_T *iLeftSaddle;
  emxArray_int32_T *iRightBase;
  emxArray_int32_T *iRightSaddle;
  emxArray_real_T *b_bPk;
  emxArray_real_T *b_wxPk;
  emxArray_real_T *idx;
  emxArray_real_T *varargin_2;
  emxArray_real_T *yFinite;
  const double *x_data;
  const double *y_data;
  double refHeight;
  double varargin_1;
  double *bPk_data;
  double *b_bPk_data;
  double *bxPk_data;
  double *byPk_data;
  double *idx_data;
  double *varargin_2_data;
  double *wxPk_data;
  double *yFinite_data;
  const int *iFin_data;
  const int *iInf_data;
  const int *iInflect_data;
  int i;
  int iLeft;
  int m;
  int md2;
  int xtmp;
  int *b_x_data;
  int *iInfL_data;
  int *iInfR_data;
  int *iLeftSaddle_data;
  int *iPk_data;
  int *iRightSaddle_data;
  boolean_T *b_yFinite_data;
  iInflect_data = iInflect->data;
  iInf_data = iInf->data;
  iFin_data = iFin->data;
  iPk_data = iPk->data;
  x_data = x->data;
  y_data = y->data;
  emxInit_real_T(&yFinite, 1);
  xtmp = yFinite->size[0];
  yFinite->size[0] = y->size[0];
  emxEnsureCapacity_real_T(yFinite, xtmp);
  yFinite_data = yFinite->data;
  md2 = y->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    yFinite_data[xtmp] = y_data[xtmp];
  }
  md2 = iInf->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    yFinite_data[iInf_data[xtmp] - 1] = rtNaN;
  }
  emxInit_int32_T(&iLeftSaddle);
  emxInit_int32_T(&iInfL);
  getLeftBase(yFinite, iPk, iFin, iInflect, iInfL, iLeftSaddle);
  iLeftSaddle_data = iLeftSaddle->data;
  iInfL_data = iInfL->data;
  emxInit_int32_T(&iInfR);
  xtmp = iInfR->size[0];
  iInfR->size[0] = iPk->size[0];
  emxEnsureCapacity_int32_T(iInfR, xtmp);
  iInfR_data = iInfR->data;
  md2 = iPk->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    iInfR_data[xtmp] = iPk_data[xtmp];
  }
  m = iPk->size[0] - 1;
  md2 = iPk->size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = iInfR_data[i];
    iLeft = m - i;
    iInfR_data[i] = iInfR_data[iLeft];
    iInfR_data[iLeft] = xtmp;
  }
  emxInit_int32_T(&b_x);
  xtmp = b_x->size[0];
  b_x->size[0] = iFin->size[0];
  emxEnsureCapacity_int32_T(b_x, xtmp);
  iInfR_data = b_x->data;
  md2 = iFin->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    iInfR_data[xtmp] = iFin_data[xtmp];
  }
  m = iFin->size[0] - 1;
  md2 = iFin->size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = iInfR_data[i];
    iLeft = m - i;
    iInfR_data[i] = iInfR_data[iLeft];
    iInfR_data[iLeft] = xtmp;
  }
  emxInit_int32_T(&c_x);
  xtmp = c_x->size[0];
  c_x->size[0] = iInflect->size[0];
  emxEnsureCapacity_int32_T(c_x, xtmp);
  b_x_data = c_x->data;
  md2 = iInflect->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    b_x_data[xtmp] = iInflect_data[xtmp];
  }
  m = iInflect->size[0] - 1;
  md2 = iInflect->size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = b_x_data[i];
    iLeft = m - i;
    b_x_data[i] = b_x_data[iLeft];
    b_x_data[iLeft] = xtmp;
  }
  emxInit_int32_T(&iRightBase);
  emxInit_int32_T(&iRightSaddle);
  getLeftBase(yFinite, iInfR, b_x, c_x, iRightBase, iRightSaddle);
  iRightSaddle_data = iRightSaddle->data;
  iInfR_data = iRightBase->data;
  m = iRightBase->size[0] - 1;
  md2 = iRightBase->size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = iInfR_data[i];
    iLeft = m - i;
    iInfR_data[i] = iInfR_data[iLeft];
    iInfR_data[iLeft] = xtmp;
  }
  m = iRightSaddle->size[0] - 1;
  md2 = iRightSaddle->size[0] >> 1;
  for (i = 0; i < md2; i++) {
    xtmp = iRightSaddle_data[i];
    iLeft = m - i;
    iRightSaddle_data[i] = iRightSaddle_data[iLeft];
    iRightSaddle_data[iLeft] = xtmp;
  }
  emxInit_real_T(&idx, 1);
  xtmp = idx->size[0];
  idx->size[0] = iInfL->size[0];
  emxEnsureCapacity_real_T(idx, xtmp);
  idx_data = idx->data;
  md2 = iInfL->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    idx_data[xtmp] = yFinite_data[iInfL_data[xtmp] - 1];
  }
  emxInit_real_T(&varargin_2, 1);
  xtmp = varargin_2->size[0];
  varargin_2->size[0] = iRightBase->size[0];
  emxEnsureCapacity_real_T(varargin_2, xtmp);
  varargin_2_data = varargin_2->data;
  md2 = iRightBase->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    varargin_2_data[xtmp] = yFinite_data[iInfR_data[xtmp] - 1];
  }
  emxInit_real_T(&b_bPk, 1);
  if (iInfL->size[0] == iRightBase->size[0]) {
    xtmp = b_bPk->size[0];
    b_bPk->size[0] = idx->size[0];
    emxEnsureCapacity_real_T(b_bPk, xtmp);
    bPk_data = b_bPk->data;
    md2 = idx->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      varargin_1 = idx_data[xtmp];
      refHeight = varargin_2_data[xtmp];
      bPk_data[xtmp] = fmax(varargin_1, refHeight);
    }
  } else {
    expand_max(idx, varargin_2, b_bPk);
    bPk_data = b_bPk->data;
  }
  emxFree_int32_T(&iRightBase);
  emxInit_boolean_T(&b_yFinite, 1);
  if (iPk->size[0] == b_bPk->size[0]) {
    xtmp = b_yFinite->size[0];
    b_yFinite->size[0] = iPk->size[0];
    emxEnsureCapacity_boolean_T(b_yFinite, xtmp);
    b_yFinite_data = b_yFinite->data;
    md2 = iPk->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      b_yFinite_data[xtmp] =
          (yFinite_data[iPk_data[xtmp] - 1] - bPk_data[xtmp] >= 0.3);
    }
    eml_find(b_yFinite, iInfL);
    iInfL_data = iInfL->data;
  } else {
    e_binary_expand_op(iInfL, yFinite, iPk, b_bPk);
    iInfL_data = iInfL->data;
  }
  xtmp = idx->size[0];
  idx->size[0] = iInfL->size[0];
  emxEnsureCapacity_real_T(idx, xtmp);
  idx_data = idx->data;
  md2 = iInfL->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    idx_data[xtmp] = iInfL_data[xtmp];
  }
  xtmp = iInfL->size[0];
  iInfL->size[0] = idx->size[0];
  emxEnsureCapacity_int32_T(iInfL, xtmp);
  iInfL_data = iInfL->data;
  md2 = idx->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    iInfL_data[xtmp] = iPk_data[(int)idx_data[xtmp] - 1];
  }
  xtmp = iPk->size[0];
  iPk->size[0] = iInfL->size[0];
  emxEnsureCapacity_int32_T(iPk, xtmp);
  iPk_data = iPk->data;
  md2 = iInfL->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    iPk_data[xtmp] = iInfL_data[xtmp];
  }
  xtmp = bPk->size[0];
  bPk->size[0] = idx->size[0];
  emxEnsureCapacity_real_T(bPk, xtmp);
  b_bPk_data = bPk->data;
  md2 = idx->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    b_bPk_data[xtmp] = bPk_data[(int)idx_data[xtmp] - 1];
  }
  xtmp = b_bPk->size[0];
  b_bPk->size[0] = bPk->size[0];
  emxEnsureCapacity_real_T(b_bPk, xtmp);
  bPk_data = b_bPk->data;
  md2 = bPk->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    bPk_data[xtmp] = b_bPk_data[xtmp];
  }
  xtmp = iInfL->size[0];
  iInfL->size[0] = idx->size[0];
  emxEnsureCapacity_int32_T(iInfL, xtmp);
  iInfL_data = iInfL->data;
  md2 = idx->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    iInfL_data[xtmp] = iLeftSaddle_data[(int)idx_data[xtmp] - 1];
  }
  xtmp = iLeftSaddle->size[0];
  iLeftSaddle->size[0] = iInfL->size[0];
  emxEnsureCapacity_int32_T(iLeftSaddle, xtmp);
  iLeftSaddle_data = iLeftSaddle->data;
  md2 = iInfL->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    iLeftSaddle_data[xtmp] = iInfL_data[xtmp];
  }
  xtmp = iInfL->size[0];
  iInfL->size[0] = idx->size[0];
  emxEnsureCapacity_int32_T(iInfL, xtmp);
  iInfL_data = iInfL->data;
  md2 = idx->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    iInfL_data[xtmp] = iRightSaddle_data[(int)idx_data[xtmp] - 1];
  }
  xtmp = iRightSaddle->size[0];
  iRightSaddle->size[0] = iInfL->size[0];
  emxEnsureCapacity_int32_T(iRightSaddle, xtmp);
  iRightSaddle_data = iRightSaddle->data;
  md2 = iInfL->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    iRightSaddle_data[xtmp] = iInfL_data[xtmp];
  }
  if (iPk->size[0] == 0) {
    idx->size[0] = 0;
    iLeftSaddle->size[0] = 0;
    iRightSaddle->size[0] = 0;
  } else {
    xtmp = idx->size[0];
    idx->size[0] = b_bPk->size[0];
    emxEnsureCapacity_real_T(idx, xtmp);
    idx_data = idx->data;
    md2 = b_bPk->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      idx_data[xtmp] = bPk_data[xtmp];
    }
  }
  emxInit_real_T(&b_wxPk, 2);
  xtmp = b_wxPk->size[0] * b_wxPk->size[1];
  b_wxPk->size[0] = iPk->size[0];
  b_wxPk->size[1] = 2;
  emxEnsureCapacity_real_T(b_wxPk, xtmp);
  wxPk_data = b_wxPk->data;
  md2 = iPk->size[0] << 1;
  for (xtmp = 0; xtmp < md2; xtmp++) {
    wxPk_data[xtmp] = 0.0;
  }
  xtmp = iPk->size[0];
  for (i = 0; i < xtmp; i++) {
    double xc_tmp;
    varargin_1 = yFinite_data[iPk_data[i] - 1] + idx_data[i];
    refHeight = varargin_1 / 2.0;
    iLeft = iPk_data[i];
    while ((iLeft >= iLeftSaddle_data[i]) &&
           (yFinite_data[iLeft - 1] > refHeight)) {
      iLeft--;
    }
    if (iLeft < iLeftSaddle_data[i]) {
      wxPk_data[i] = x_data[iLeftSaddle_data[i] - 1];
    } else {
      double b_xc_tmp;
      xc_tmp = yFinite_data[iLeft - 1];
      b_xc_tmp = x_data[iLeft - 1];
      varargin_1 = b_xc_tmp + (x_data[iLeft] - b_xc_tmp) *
                                  (0.5 * varargin_1 - xc_tmp) /
                                  (yFinite_data[iLeft] - xc_tmp);
      if (rtIsNaN(varargin_1)) {
        if (rtIsInf(idx_data[i])) {
          varargin_1 = 0.5 * (b_xc_tmp + x_data[iLeft]);
        } else {
          varargin_1 = x_data[iLeft];
        }
      }
      wxPk_data[i] = varargin_1;
    }
    iLeft = iPk_data[i] - 1;
    while ((iLeft + 1 <= iRightSaddle_data[i]) &&
           (yFinite_data[iLeft] > refHeight)) {
      iLeft++;
    }
    if (iLeft + 1 > iRightSaddle_data[i]) {
      wxPk_data[i + b_wxPk->size[0]] = x_data[iRightSaddle_data[i] - 1];
    } else {
      xc_tmp = x_data[iLeft - 1];
      varargin_1 = x_data[iLeft] +
                   (xc_tmp - x_data[iLeft]) *
                       (0.5 * (yFinite_data[iPk_data[i] - 1] + idx_data[i]) -
                        yFinite_data[iLeft]) /
                       (yFinite_data[iLeft - 1] - yFinite_data[iLeft]);
      if (rtIsNaN(varargin_1)) {
        if (rtIsInf(idx_data[i])) {
          varargin_1 = 0.5 * (x_data[iLeft] + xc_tmp);
        } else {
          varargin_1 = xc_tmp;
        }
      }
      wxPk_data[i + b_wxPk->size[0]] = varargin_1;
    }
  }
  emxFree_real_T(&yFinite);
  do_vectors(iPk, iInf, c_x, iInfL, iInfR);
  b_x_data = c_x->data;
  b_do_vectors(c_x, iPk, b_x, iInfL, iInfR);
  iInfL_data = iInfL->data;
  xtmp = varargin_2->size[0];
  varargin_2->size[0] = iInfL->size[0];
  emxEnsureCapacity_real_T(varargin_2, xtmp);
  varargin_2_data = varargin_2->data;
  md2 = iInfL->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    varargin_2_data[xtmp] = iInfL_data[xtmp];
  }
  b_do_vectors(c_x, iInf, b_x, iInfL, iInfR);
  iInfL_data = iInfL->data;
  xtmp = idx->size[0];
  idx->size[0] = iInfL->size[0];
  emxEnsureCapacity_real_T(idx, xtmp);
  idx_data = idx->data;
  md2 = iInfL->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    idx_data[xtmp] = iInfL_data[xtmp];
  }
  xtmp = bPk->size[0];
  bPk->size[0] = c_x->size[0];
  emxEnsureCapacity_real_T(bPk, xtmp);
  b_bPk_data = bPk->data;
  md2 = c_x->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    b_bPk_data[xtmp] = 0.0;
  }
  md2 = b_bPk->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    b_bPk_data[(int)varargin_2_data[xtmp] - 1] = bPk_data[xtmp];
  }
  xtmp = iInfL->size[0];
  iInfL->size[0] = idx->size[0];
  emxEnsureCapacity_int32_T(iInfL, xtmp);
  iInfL_data = iInfL->data;
  md2 = idx->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    iInfL_data[xtmp] = (int)idx_data[xtmp];
  }
  md2 = iInfL->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    b_bPk_data[iInfL_data[xtmp] - 1] = 0.0;
  }
  xtmp = iInfL->size[0];
  iInfL->size[0] = iInf->size[0];
  emxEnsureCapacity_int32_T(iInfL, xtmp);
  iInfL_data = iInfL->data;
  md2 = iInf->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    iLeft = iInf_data[xtmp] - 1;
    if (iLeft < 1) {
      iLeft = 1;
    }
    iInfL_data[xtmp] = iLeft;
  }
  xtmp = iInfR->size[0];
  iInfR->size[0] = iInf->size[0];
  emxEnsureCapacity_int32_T(iInfR, xtmp);
  iInfR_data = iInfR->data;
  md2 = iInf->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    m = iInf_data[xtmp] + 1;
    iLeft = x->size[0];
    if (m > iLeft) {
      iInfR_data[xtmp] = iLeft;
    } else {
      iInfR_data[xtmp] = m;
    }
  }
  xtmp = bxPk->size[0] * bxPk->size[1];
  bxPk->size[0] = c_x->size[0];
  bxPk->size[1] = 2;
  emxEnsureCapacity_real_T(bxPk, xtmp);
  bxPk_data = bxPk->data;
  md2 = c_x->size[0] << 1;
  for (xtmp = 0; xtmp < md2; xtmp++) {
    bxPk_data[xtmp] = 0.0;
  }
  md2 = iLeftSaddle->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    bxPk_data[(int)varargin_2_data[xtmp] - 1] =
        x_data[iLeftSaddle_data[xtmp] - 1];
  }
  md2 = iRightSaddle->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    bxPk_data[((int)varargin_2_data[xtmp] + bxPk->size[0]) - 1] =
        x_data[iRightSaddle_data[xtmp] - 1];
  }
  if (iInf->size[0] == iInfL->size[0]) {
    md2 = iInf->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      bxPk_data[(int)idx_data[xtmp] - 1] =
          0.5 * (x_data[iInf_data[xtmp] - 1] + x_data[iInfL_data[xtmp] - 1]);
    }
  } else {
    d_binary_expand_op(bxPk, idx, x, iInf, iInfL);
    bxPk_data = bxPk->data;
  }
  if (iInf->size[0] == iInfR->size[0]) {
    md2 = iInf->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      bxPk_data[((int)idx_data[xtmp] + bxPk->size[0]) - 1] =
          0.5 * (x_data[iInf_data[xtmp] - 1] + x_data[iInfR_data[xtmp] - 1]);
    }
  } else {
    c_binary_expand_op(bxPk, idx, x, iInf, iInfR);
    bxPk_data = bxPk->data;
  }
  xtmp = byPk->size[0] * byPk->size[1];
  byPk->size[0] = c_x->size[0];
  byPk->size[1] = 2;
  emxEnsureCapacity_real_T(byPk, xtmp);
  byPk_data = byPk->data;
  md2 = c_x->size[0] << 1;
  for (xtmp = 0; xtmp < md2; xtmp++) {
    byPk_data[xtmp] = 0.0;
  }
  md2 = iLeftSaddle->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    byPk_data[(int)varargin_2_data[xtmp] - 1] =
        y_data[iLeftSaddle_data[xtmp] - 1];
  }
  emxFree_int32_T(&iLeftSaddle);
  md2 = iRightSaddle->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    byPk_data[((int)varargin_2_data[xtmp] + byPk->size[0]) - 1] =
        y_data[iRightSaddle_data[xtmp] - 1];
  }
  emxFree_int32_T(&iRightSaddle);
  md2 = iInfL->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    byPk_data[(int)idx_data[xtmp] - 1] = y_data[iInfL_data[xtmp] - 1];
  }
  md2 = iInfR->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    byPk_data[((int)idx_data[xtmp] + byPk->size[0]) - 1] =
        y_data[iInfR_data[xtmp] - 1];
  }
  xtmp = wxPk->size[0] * wxPk->size[1];
  wxPk->size[0] = c_x->size[0];
  wxPk->size[1] = 2;
  emxEnsureCapacity_real_T(wxPk, xtmp);
  yFinite_data = wxPk->data;
  md2 = c_x->size[0] << 1;
  for (xtmp = 0; xtmp < md2; xtmp++) {
    yFinite_data[xtmp] = 0.0;
  }
  md2 = b_wxPk->size[0];
  for (xtmp = 0; xtmp < 2; xtmp++) {
    for (iLeft = 0; iLeft < md2; iLeft++) {
      yFinite_data[((int)varargin_2_data[iLeft] + wxPk->size[0] * xtmp) - 1] =
          wxPk_data[iLeft + b_wxPk->size[0] * xtmp];
    }
  }
  emxFree_real_T(&varargin_2);
  if (iInf->size[0] == iInfL->size[0]) {
    md2 = iInf->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      yFinite_data[(int)idx_data[xtmp] - 1] =
          0.5 * (x_data[iInf_data[xtmp] - 1] + x_data[iInfL_data[xtmp] - 1]);
    }
  } else {
    d_binary_expand_op(wxPk, idx, x, iInf, iInfL);
    yFinite_data = wxPk->data;
  }
  if (iInf->size[0] == iInfR->size[0]) {
    md2 = iInf->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      yFinite_data[((int)idx_data[xtmp] + wxPk->size[0]) - 1] =
          0.5 * (x_data[iInf_data[xtmp] - 1] + x_data[iInfR_data[xtmp] - 1]);
    }
  } else {
    c_binary_expand_op(wxPk, idx, x, iInf, iInfR);
    yFinite_data = wxPk->data;
  }
  emxFree_int32_T(&iInfR);
  if (c_x->size[0] != 0) {
    xtmp = idx->size[0];
    idx->size[0] = wxPk->size[0];
    emxEnsureCapacity_real_T(idx, xtmp);
    idx_data = idx->data;
    if (wxPk->size[0] != 0) {
      iLeft = wxPk->size[0];
      for (m = 0; m < iLeft; m++) {
        idx_data[m] = yFinite_data[m + iLeft] - yFinite_data[m];
      }
    }
    xtmp = b_yFinite->size[0];
    b_yFinite->size[0] = idx->size[0];
    emxEnsureCapacity_boolean_T(b_yFinite, xtmp);
    b_yFinite_data = b_yFinite->data;
    md2 = idx->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      b_yFinite_data[xtmp] =
          ((idx_data[xtmp] >= 20.0) && (idx_data[xtmp] <= rtInf));
    }
    eml_find(b_yFinite, iInfL);
    iInfL_data = iInfL->data;
    xtmp = idx->size[0];
    idx->size[0] = iInfL->size[0];
    emxEnsureCapacity_real_T(idx, xtmp);
    idx_data = idx->data;
    md2 = iInfL->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      idx_data[xtmp] = iInfL_data[xtmp];
    }
    xtmp = b_x->size[0];
    b_x->size[0] = idx->size[0];
    emxEnsureCapacity_int32_T(b_x, xtmp);
    iInfR_data = b_x->data;
    md2 = idx->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      iInfR_data[xtmp] = b_x_data[(int)idx_data[xtmp] - 1];
    }
    xtmp = c_x->size[0];
    c_x->size[0] = b_x->size[0];
    emxEnsureCapacity_int32_T(c_x, xtmp);
    b_x_data = c_x->data;
    md2 = b_x->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      b_x_data[xtmp] = iInfR_data[xtmp];
    }
    xtmp = b_bPk->size[0];
    b_bPk->size[0] = idx->size[0];
    emxEnsureCapacity_real_T(b_bPk, xtmp);
    bPk_data = b_bPk->data;
    md2 = idx->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      bPk_data[xtmp] = b_bPk_data[(int)idx_data[xtmp] - 1];
    }
    xtmp = bPk->size[0];
    bPk->size[0] = b_bPk->size[0];
    emxEnsureCapacity_real_T(bPk, xtmp);
    b_bPk_data = bPk->data;
    md2 = b_bPk->size[0];
    for (xtmp = 0; xtmp < md2; xtmp++) {
      b_bPk_data[xtmp] = bPk_data[xtmp];
    }
    xtmp = b_wxPk->size[0] * b_wxPk->size[1];
    b_wxPk->size[0] = idx->size[0];
    b_wxPk->size[1] = 2;
    emxEnsureCapacity_real_T(b_wxPk, xtmp);
    wxPk_data = b_wxPk->data;
    md2 = idx->size[0];
    for (xtmp = 0; xtmp < 2; xtmp++) {
      for (iLeft = 0; iLeft < md2; iLeft++) {
        wxPk_data[iLeft + b_wxPk->size[0] * xtmp] =
            bxPk_data[((int)idx_data[iLeft] + bxPk->size[0] * xtmp) - 1];
      }
    }
    xtmp = bxPk->size[0] * bxPk->size[1];
    bxPk->size[0] = b_wxPk->size[0];
    bxPk->size[1] = 2;
    emxEnsureCapacity_real_T(bxPk, xtmp);
    bxPk_data = bxPk->data;
    md2 = b_wxPk->size[0] * 2;
    for (xtmp = 0; xtmp < md2; xtmp++) {
      bxPk_data[xtmp] = wxPk_data[xtmp];
    }
    xtmp = b_wxPk->size[0] * b_wxPk->size[1];
    b_wxPk->size[0] = idx->size[0];
    b_wxPk->size[1] = 2;
    emxEnsureCapacity_real_T(b_wxPk, xtmp);
    wxPk_data = b_wxPk->data;
    md2 = idx->size[0];
    for (xtmp = 0; xtmp < 2; xtmp++) {
      for (iLeft = 0; iLeft < md2; iLeft++) {
        wxPk_data[iLeft + b_wxPk->size[0] * xtmp] =
            byPk_data[((int)idx_data[iLeft] + byPk->size[0] * xtmp) - 1];
      }
    }
    xtmp = byPk->size[0] * byPk->size[1];
    byPk->size[0] = b_wxPk->size[0];
    byPk->size[1] = 2;
    emxEnsureCapacity_real_T(byPk, xtmp);
    byPk_data = byPk->data;
    md2 = b_wxPk->size[0] * 2;
    for (xtmp = 0; xtmp < md2; xtmp++) {
      byPk_data[xtmp] = wxPk_data[xtmp];
    }
    xtmp = b_wxPk->size[0] * b_wxPk->size[1];
    b_wxPk->size[0] = idx->size[0];
    b_wxPk->size[1] = 2;
    emxEnsureCapacity_real_T(b_wxPk, xtmp);
    wxPk_data = b_wxPk->data;
    md2 = idx->size[0];
    for (xtmp = 0; xtmp < 2; xtmp++) {
      for (iLeft = 0; iLeft < md2; iLeft++) {
        wxPk_data[iLeft + b_wxPk->size[0] * xtmp] =
            yFinite_data[((int)idx_data[iLeft] + wxPk->size[0] * xtmp) - 1];
      }
    }
    xtmp = wxPk->size[0] * wxPk->size[1];
    wxPk->size[0] = b_wxPk->size[0];
    wxPk->size[1] = 2;
    emxEnsureCapacity_real_T(wxPk, xtmp);
    yFinite_data = wxPk->data;
    md2 = b_wxPk->size[0] * 2;
    for (xtmp = 0; xtmp < md2; xtmp++) {
      yFinite_data[xtmp] = wxPk_data[xtmp];
    }
  }
  emxFree_boolean_T(&b_yFinite);
  emxFree_int32_T(&b_x);
  emxFree_int32_T(&iInfL);
  emxFree_real_T(&idx);
  emxFree_real_T(&b_wxPk);
  emxFree_real_T(&b_bPk);
  xtmp = iPk->size[0];
  iPk->size[0] = c_x->size[0];
  emxEnsureCapacity_int32_T(iPk, xtmp);
  iPk_data = iPk->data;
  md2 = c_x->size[0];
  for (xtmp = 0; xtmp < md2; xtmp++) {
    iPk_data[xtmp] = b_x_data[xtmp];
  }
  emxFree_int32_T(&c_x);
}

/*
 * File trailer for findExtents.c
 *
 * [EOF]
 */
