/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: findExtents.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 02-Dec-2022 11:02:26
 */

/* Include Files */
#include "findExtents.h"
#include "eml_setop.h"
#include "find.h"
#include "ixfun.h"
#include "recalibrateHistory_emxutil.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void combineFullPeaks(const double y[4096], const int iPk_data[],
                             int iPk_size, const double bPk_data[],
                             int bPk_size, const int iLBw_data[], int iLBw_size,
                             const int iRBw_data[], int iRBw_size,
                             const emxArray_real_T *wPk, const int iInf_data[],
                             int iInf_size, int iPkOut_data[], int *iPkOut_size,
                             emxArray_real_T *bPkOut, emxArray_real_T *bxPkOut,
                             emxArray_real_T *byPkOut,
                             emxArray_real_T *wxPkOut);

static void getLeftBase(const double yTemp[4096], const int iPeak_data[],
                        int iPeak_size, const int iFinite_data[],
                        int iFinite_size, const int iInflect_data[],
                        int iBase_data[], int *iBase_size, int iSaddle_data[],
                        int *iSaddle_size);

static void getPeakBase(const double yTemp[4096], const int iPk_data[],
                        int iPk_size, const int iFin_data[], int iFin_size,
                        const int iInflect_data[], int iInflect_size,
                        double peakBase_data[], int *peakBase_size,
                        int iLeftSaddle_data[], int *iLeftSaddle_size,
                        int iRightSaddle_data[], int *iRightSaddle_size);

/* Function Definitions */
/*
 * Arguments    : const double y[4096]
 *                const int iPk_data[]
 *                int iPk_size
 *                const double bPk_data[]
 *                int bPk_size
 *                const int iLBw_data[]
 *                int iLBw_size
 *                const int iRBw_data[]
 *                int iRBw_size
 *                const emxArray_real_T *wPk
 *                const int iInf_data[]
 *                int iInf_size
 *                int iPkOut_data[]
 *                int *iPkOut_size
 *                emxArray_real_T *bPkOut
 *                emxArray_real_T *bxPkOut
 *                emxArray_real_T *byPkOut
 *                emxArray_real_T *wxPkOut
 * Return Type  : void
 */
static void combineFullPeaks(const double y[4096], const int iPk_data[],
                             int iPk_size, const double bPk_data[],
                             int bPk_size, const int iLBw_data[], int iLBw_size,
                             const int iRBw_data[], int iRBw_size,
                             const emxArray_real_T *wPk, const int iInf_data[],
                             int iInf_size, int iPkOut_data[], int *iPkOut_size,
                             emxArray_real_T *bPkOut, emxArray_real_T *bxPkOut,
                             emxArray_real_T *byPkOut, emxArray_real_T *wxPkOut)
{
  const double *wPk_data;
  double *bPkOut_data;
  int c_data[4096];
  int iFinite_data[4096];
  int iInfL_data[4096];
  int iInfR_data[4096];
  int c_size;
  int i;
  int iInfL_size;
  int iInfR_size;
  wPk_data = wPk->data;
  do_vectors(iPk_data, iPk_size, iInf_data, iInf_size, iPkOut_data, iPkOut_size,
             iInfL_data, &iInfL_size, iInfR_data, &iInfR_size);
  b_do_vectors(iPkOut_data, *iPkOut_size, iPk_data, iPk_size, c_data, &c_size,
               iInfL_data, &iInfL_size, iInfR_data, &iInfR_size);
  if (iInfL_size - 1 >= 0) {
    memcpy(&iFinite_data[0], &iInfL_data[0],
           (unsigned int)iInfL_size * sizeof(int));
  }
  b_do_vectors(iPkOut_data, *iPkOut_size, iInf_data, iInf_size, c_data, &c_size,
               iInfL_data, &iInfL_size, iInfR_data, &iInfR_size);
  if (iInfL_size - 1 >= 0) {
    memcpy(&c_data[0], &iInfL_data[0], (unsigned int)iInfL_size * sizeof(int));
  }
  i = bPkOut->size[0];
  bPkOut->size[0] = *iPkOut_size;
  emxEnsureCapacity_real_T(bPkOut, i);
  bPkOut_data = bPkOut->data;
  for (i = 0; i < *iPkOut_size; i++) {
    bPkOut_data[i] = 0.0;
  }
  for (i = 0; i < bPk_size; i++) {
    bPkOut_data[iFinite_data[i] - 1] = bPk_data[i];
  }
  for (i = 0; i < iInfL_size; i++) {
    bPkOut_data[iInfL_data[i] - 1] = 0.0;
  }
  for (i = 0; i < iInf_size; i++) {
    iInfR_size = iInf_data[i] - 1;
    if (iInfR_size < 1) {
      iInfR_size = 1;
    }
    iInfL_data[i] = iInfR_size;
  }
  for (i = 0; i < iInf_size; i++) {
    iInfR_size = iInf_data[i] + 1;
    if (iInfR_size > 4096) {
      iInfR_data[i] = 4096;
    } else {
      iInfR_data[i] = iInfR_size;
    }
  }
  i = bxPkOut->size[0] * bxPkOut->size[1];
  bxPkOut->size[0] = *iPkOut_size;
  bxPkOut->size[1] = 2;
  emxEnsureCapacity_real_T(bxPkOut, i);
  bPkOut_data = bxPkOut->data;
  iInfR_size = *iPkOut_size << 1;
  for (i = 0; i < iInfR_size; i++) {
    bPkOut_data[i] = 0.0;
  }
  for (i = 0; i < iLBw_size; i++) {
    bPkOut_data[iFinite_data[i] - 1] = (short)((short)(iLBw_data[i] - 1) + 1);
  }
  for (i = 0; i < iRBw_size; i++) {
    bPkOut_data[(iFinite_data[i] + bxPkOut->size[0]) - 1] =
        (short)((short)(iRBw_data[i] - 1) + 1);
  }
  for (i = 0; i < iInf_size; i++) {
    bPkOut_data[c_data[i] - 1] =
        0.5 * (double)(short)((short)((short)(iInf_data[i] - 1) +
                                      (short)(iInfL_data[i] - 1)) +
                              2);
  }
  for (i = 0; i < iInf_size; i++) {
    bPkOut_data[(c_data[i] + bxPkOut->size[0]) - 1] =
        0.5 * (double)(short)((short)((short)(iInf_data[i] - 1) +
                                      (short)(iInfR_data[i] - 1)) +
                              2);
  }
  i = byPkOut->size[0] * byPkOut->size[1];
  byPkOut->size[0] = *iPkOut_size;
  byPkOut->size[1] = 2;
  emxEnsureCapacity_real_T(byPkOut, i);
  bPkOut_data = byPkOut->data;
  for (i = 0; i < iInfR_size; i++) {
    bPkOut_data[i] = 0.0;
  }
  for (i = 0; i < iLBw_size; i++) {
    bPkOut_data[iFinite_data[i] - 1] = y[iLBw_data[i] - 1];
  }
  for (i = 0; i < iRBw_size; i++) {
    bPkOut_data[(iFinite_data[i] + byPkOut->size[0]) - 1] = y[iRBw_data[i] - 1];
  }
  for (i = 0; i < iInf_size; i++) {
    bPkOut_data[c_data[i] - 1] = y[iInfL_data[i] - 1];
  }
  for (i = 0; i < iInf_size; i++) {
    bPkOut_data[(c_data[i] + byPkOut->size[0]) - 1] = y[iInfR_data[i] - 1];
  }
  i = wxPkOut->size[0] * wxPkOut->size[1];
  wxPkOut->size[0] = *iPkOut_size;
  wxPkOut->size[1] = 2;
  emxEnsureCapacity_real_T(wxPkOut, i);
  bPkOut_data = wxPkOut->data;
  for (i = 0; i < iInfR_size; i++) {
    bPkOut_data[i] = 0.0;
  }
  iInfR_size = wPk->size[0];
  for (i = 0; i < 2; i++) {
    for (c_size = 0; c_size < iInfR_size; c_size++) {
      bPkOut_data[(iFinite_data[c_size] + wxPkOut->size[0] * i) - 1] =
          wPk_data[c_size + wPk->size[0] * i];
    }
  }
  for (i = 0; i < iInf_size; i++) {
    bPkOut_data[c_data[i] - 1] =
        0.5 * (double)(short)((short)((short)(iInf_data[i] - 1) +
                                      (short)(iInfL_data[i] - 1)) +
                              2);
  }
  for (i = 0; i < iInf_size; i++) {
    bPkOut_data[(c_data[i] + wxPkOut->size[0]) - 1] =
        0.5 * (double)(short)((short)((short)(iInf_data[i] - 1) +
                                      (short)(iInfR_data[i] - 1)) +
                              2);
  }
}

/*
 * Arguments    : const double yTemp[4096]
 *                const int iPeak_data[]
 *                int iPeak_size
 *                const int iFinite_data[]
 *                int iFinite_size
 *                const int iInflect_data[]
 *                int iBase_data[]
 *                int *iBase_size
 *                int iSaddle_data[]
 *                int *iSaddle_size
 * Return Type  : void
 */
static void getLeftBase(const double yTemp[4096], const int iPeak_data[],
                        int iPeak_size, const int iFinite_data[],
                        int iFinite_size, const int iInflect_data[],
                        int iBase_data[], int *iBase_size, int iSaddle_data[],
                        int *iSaddle_size)
{
  static double peak_data[4096];
  static double valley_data[4096];
  double v;
  int iValley_data[4096];
  int i;
  int iv;
  int j;
  int k;
  int n;
  *iBase_size = iPeak_size;
  if (iPeak_size - 1 >= 0) {
    memset(&iBase_data[0], 0, (unsigned int)iPeak_size * sizeof(int));
  }
  *iSaddle_size = iPeak_size;
  if (iPeak_size - 1 >= 0) {
    memset(&iSaddle_data[0], 0, (unsigned int)iPeak_size * sizeof(int));
  }
  if (iFinite_size - 1 >= 0) {
    memset(&peak_data[0], 0, (unsigned int)iFinite_size * sizeof(double));
  }
  if (iFinite_size - 1 >= 0) {
    memset(&valley_data[0], 0, (unsigned int)iFinite_size * sizeof(double));
  }
  if (iFinite_size - 1 >= 0) {
    memset(&iValley_data[0], 0, (unsigned int)iFinite_size * sizeof(int));
  }
  n = -1;
  i = 0;
  j = 0;
  k = 0;
  v = rtNaN;
  iv = 1;
  while (k + 1 <= iPeak_size) {
    double p;
    int isv;
    while (iInflect_data[i] != iFinite_data[j]) {
      v = yTemp[iInflect_data[i] - 1];
      iv = iInflect_data[i];
      if (rtIsNaN(yTemp[iInflect_data[i] - 1])) {
        n = -1;
      } else {
        while ((n + 1 > 0) && (valley_data[n] > v)) {
          n--;
        }
      }
      i++;
    }
    p = yTemp[iInflect_data[i] - 1];
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
    peak_data[n] = yTemp[iInflect_data[i] - 1];
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
}

/*
 * Arguments    : const double yTemp[4096]
 *                const int iPk_data[]
 *                int iPk_size
 *                const int iFin_data[]
 *                int iFin_size
 *                const int iInflect_data[]
 *                int iInflect_size
 *                double peakBase_data[]
 *                int *peakBase_size
 *                int iLeftSaddle_data[]
 *                int *iLeftSaddle_size
 *                int iRightSaddle_data[]
 *                int *iRightSaddle_size
 * Return Type  : void
 */
static void getPeakBase(const double yTemp[4096], const int iPk_data[],
                        int iPk_size, const int iFin_data[], int iFin_size,
                        const int iInflect_data[], int iInflect_size,
                        double peakBase_data[], int *peakBase_size,
                        int iLeftSaddle_data[], int *iLeftSaddle_size,
                        int iRightSaddle_data[], int *iRightSaddle_size)
{
  static double varargin_1_data[4096];
  static double varargin_2_data[4096];
  int b_x_data[4096];
  int c_x_data[4096];
  int iLeftBase_data[4096];
  int iRightBase_data[4096];
  int x_data[4096];
  int b_i;
  int i;
  int iLeftBase_size;
  int iRightBase_size;
  int x_tmp;
  int xtmp;
  getLeftBase(yTemp, iPk_data, iPk_size, iFin_data, iFin_size, iInflect_data,
              iLeftBase_data, &iLeftBase_size, iLeftSaddle_data,
              iLeftSaddle_size);
  if (iPk_size - 1 >= 0) {
    memcpy(&x_data[0], &iPk_data[0], (unsigned int)iPk_size * sizeof(int));
  }
  i = iPk_size >> 1;
  for (b_i = 0; b_i < i; b_i++) {
    xtmp = x_data[b_i];
    x_tmp = (iPk_size - b_i) - 1;
    x_data[b_i] = x_data[x_tmp];
    x_data[x_tmp] = xtmp;
  }
  if (iFin_size - 1 >= 0) {
    memcpy(&b_x_data[0], &iFin_data[0], (unsigned int)iFin_size * sizeof(int));
  }
  i = iFin_size >> 1;
  for (b_i = 0; b_i < i; b_i++) {
    xtmp = b_x_data[b_i];
    x_tmp = (iFin_size - b_i) - 1;
    b_x_data[b_i] = b_x_data[x_tmp];
    b_x_data[x_tmp] = xtmp;
  }
  if (iInflect_size - 1 >= 0) {
    memcpy(&c_x_data[0], &iInflect_data[0],
           (unsigned int)iInflect_size * sizeof(int));
  }
  i = iInflect_size >> 1;
  for (b_i = 0; b_i < i; b_i++) {
    xtmp = c_x_data[b_i];
    x_tmp = (iInflect_size - b_i) - 1;
    c_x_data[b_i] = c_x_data[x_tmp];
    c_x_data[x_tmp] = xtmp;
  }
  getLeftBase(yTemp, x_data, iPk_size, b_x_data, iFin_size, c_x_data,
              iRightBase_data, &iRightBase_size, iRightSaddle_data,
              iRightSaddle_size);
  i = iRightBase_size >> 1;
  for (b_i = 0; b_i < i; b_i++) {
    xtmp = iRightBase_data[b_i];
    x_tmp = (iRightBase_size - b_i) - 1;
    iRightBase_data[b_i] = iRightBase_data[x_tmp];
    iRightBase_data[x_tmp] = xtmp;
  }
  i = *iRightSaddle_size >> 1;
  for (b_i = 0; b_i < i; b_i++) {
    xtmp = iRightSaddle_data[b_i];
    x_tmp = (*iRightSaddle_size - b_i) - 1;
    iRightSaddle_data[b_i] = iRightSaddle_data[x_tmp];
    iRightSaddle_data[x_tmp] = xtmp;
  }
  for (i = 0; i < iLeftBase_size; i++) {
    varargin_1_data[i] = yTemp[iLeftBase_data[i] - 1];
  }
  for (i = 0; i < iRightBase_size; i++) {
    varargin_2_data[i] = yTemp[iRightBase_data[i] - 1];
  }
  if (iLeftBase_size == iRightBase_size) {
    *peakBase_size = iLeftBase_size;
    for (i = 0; i < iLeftBase_size; i++) {
      peakBase_data[i] = fmax(varargin_1_data[i], varargin_2_data[i]);
    }
  } else {
    expand_max(varargin_1_data, iLeftBase_size, varargin_2_data,
               iRightBase_size, peakBase_data, peakBase_size);
  }
}

/*
 * Arguments    : const double y[4096]
 *                int iPk_data[]
 *                int *iPk_size
 *                const int iFin_data[]
 *                int iFin_size
 *                const int iInf_data[]
 *                int iInf_size
 *                const int iInflect_data[]
 *                int iInflect_size
 *                emxArray_real_T *bPk
 *                emxArray_real_T *bxPk
 *                emxArray_real_T *byPk
 *                emxArray_real_T *wxPk
 * Return Type  : void
 */
void findExtents(const double y[4096], int iPk_data[], int *iPk_size,
                 const int iFin_data[], int iFin_size, const int iInf_data[],
                 int iInf_size, const int iInflect_data[], int iInflect_size,
                 emxArray_real_T *bPk, emxArray_real_T *bxPk,
                 emxArray_real_T *byPk, emxArray_real_T *wxPk)
{
  static double bPk_data[4096];
  static double base_data[4096];
  static double yFinite[4096];
  static int b_iPk_data[8192];
  emxArray_real_T *b_bPk;
  emxArray_real_T *b_bxPk;
  emxArray_real_T *b_wxPk;
  emxArray_real_T *w;
  double *b_bPk_data;
  double *bxPk_data;
  double *byPk_data;
  double *c_bPk_data;
  double *w_data;
  double *wxPk_data;
  int iLB_data[4096];
  int iLBh_data[4096];
  int iRB_data[4096];
  int bPk_size;
  int b_i;
  int i;
  int i1;
  int iLB_size;
  int iLBh_size;
  int iLeft;
  int iRB_size;
  int loop_ub;
  memcpy(&yFinite[0], &y[0], 4096U * sizeof(double));
  for (i = 0; i < iInf_size; i++) {
    yFinite[iInf_data[i] - 1] = rtNaN;
  }
  getPeakBase(yFinite, iPk_data, *iPk_size, iFin_data, iFin_size, iInflect_data,
              iInflect_size, base_data, &iLeft, iLB_data, &iLB_size, iRB_data,
              &iRB_size);
  if (*iPk_size == iLeft) {
    boolean_T yFinite_data[4096];
    for (i = 0; i < *iPk_size; i++) {
      yFinite_data[i] = (yFinite[iPk_data[i] - 1] - base_data[i] >= 0.1);
    }
    eml_find(yFinite_data, *iPk_size, b_iPk_data, &iRB_size);
  } else {
    c_binary_expand_op(b_iPk_data, &iRB_size, yFinite, iPk_data, iPk_size,
                       base_data, &iLeft);
  }
  emxInit_real_T(&w, 1);
  i = w->size[0];
  w->size[0] = iRB_size;
  emxEnsureCapacity_real_T(w, i);
  w_data = w->data;
  for (i = 0; i < iRB_size; i++) {
    w_data[i] = b_iPk_data[i];
  }
  bPk_size = w->size[0];
  loop_ub = w->size[0];
  for (i = 0; i < loop_ub; i++) {
    bPk_data[i] = base_data[(int)w_data[i] - 1];
  }
  if (w->size[0] == 0) {
    iLBh_size = 0;
    iLB_size = 0;
  } else {
    if (bPk_size - 1 >= 0) {
      memcpy(&base_data[0], &bPk_data[0],
             (unsigned int)bPk_size * sizeof(double));
    }
    iLBh_size = w->size[0];
    loop_ub = w->size[0];
    for (i = 0; i < loop_ub; i++) {
      iLBh_data[i] = iLB_data[(int)w_data[i] - 1];
    }
    iLB_size = w->size[0];
    loop_ub = w->size[0];
    for (i = 0; i < loop_ub; i++) {
      iLB_data[i] = iRB_data[(int)w_data[i] - 1];
    }
  }
  emxInit_real_T(&b_wxPk, 2);
  i = b_wxPk->size[0] * b_wxPk->size[1];
  b_wxPk->size[0] = w->size[0];
  b_wxPk->size[1] = 2;
  emxEnsureCapacity_real_T(b_wxPk, i);
  wxPk_data = b_wxPk->data;
  loop_ub = w->size[0] << 1;
  for (i = 0; i < loop_ub; i++) {
    wxPk_data[i] = 0.0;
  }
  i = w->size[0];
  for (b_i = 0; b_i < i; b_i++) {
    double refHeight;
    double refHeight_tmp;
    double xc_tmp;
    iRB_size = (int)w_data[b_i] - 1;
    loop_ub = iPk_data[iRB_size] - 1;
    refHeight_tmp = yFinite[loop_ub] + base_data[b_i];
    refHeight = refHeight_tmp / 2.0;
    iLeft = iPk_data[iRB_size];
    while ((iLeft >= iLBh_data[b_i]) && (yFinite[iLeft - 1] > refHeight)) {
      iLeft--;
    }
    i1 = iLBh_data[b_i];
    if (iLeft < i1) {
      wxPk_data[b_i] = ((double)i1 - 1.0) + 1.0;
    } else {
      xc_tmp = yFinite[iLeft - 1];
      refHeight_tmp =
          (((double)iLeft - 1.0) + 1.0) +
          (0.5 * refHeight_tmp - xc_tmp) / (yFinite[iLeft] - xc_tmp);
      if (rtIsNaN(refHeight_tmp)) {
        if (rtIsInf(base_data[b_i])) {
          refHeight_tmp = 0.5 * (double)((iLeft + iLeft) + 1);
        } else {
          refHeight_tmp = (double)iLeft + 1.0;
        }
      }
      wxPk_data[b_i] = refHeight_tmp;
    }
    iLeft = iPk_data[iRB_size];
    while ((iLeft <= iLB_data[b_i]) && (yFinite[iLeft - 1] > refHeight)) {
      iLeft++;
    }
    i1 = iLB_data[b_i];
    if (iLeft > i1) {
      wxPk_data[b_i + b_wxPk->size[0]] = ((double)i1 - 1.0) + 1.0;
    } else {
      xc_tmp = yFinite[iLeft - 1];
      refHeight = base_data[b_i];
      refHeight_tmp = (((double)iLeft - 1.0) + 1.0) +
                      -(0.5 * (yFinite[loop_ub] + refHeight) - xc_tmp) /
                          (yFinite[iLeft - 2] - xc_tmp);
      if (rtIsNaN(refHeight_tmp)) {
        if (rtIsInf(refHeight)) {
          refHeight_tmp = 0.5 * (double)((iLeft + iLeft) - 1);
        } else {
          refHeight_tmp = ((double)iLeft - 2.0) + 1.0;
        }
      }
      wxPk_data[b_i + b_wxPk->size[0]] = refHeight_tmp;
    }
  }
  iRB_size = w->size[0];
  loop_ub = w->size[0];
  for (i = 0; i < loop_ub; i++) {
    iRB_data[i] = iPk_data[(int)w_data[i] - 1];
  }
  combineFullPeaks(y, iRB_data, iRB_size, bPk_data, bPk_size, iLBh_data,
                   iLBh_size, iLB_data, iLB_size, b_wxPk, iInf_data, iInf_size,
                   iPk_data, iPk_size, bPk, bxPk, byPk, wxPk);
  wxPk_data = wxPk->data;
  byPk_data = byPk->data;
  bxPk_data = bxPk->data;
  b_bPk_data = bPk->data;
  emxFree_real_T(&b_wxPk);
  if (*iPk_size != 0) {
    boolean_T b_w_data[8192];
    i = w->size[0];
    w->size[0] = wxPk->size[0];
    emxEnsureCapacity_real_T(w, i);
    w_data = w->data;
    if (wxPk->size[0] != 0) {
      iLeft = wxPk->size[0];
      for (iRB_size = 0; iRB_size < iLeft; iRB_size++) {
        w_data[iRB_size] = wxPk_data[iRB_size + iLeft] - wxPk_data[iRB_size];
      }
    }
    iLeft = w->size[0];
    loop_ub = w->size[0];
    for (i = 0; i < loop_ub; i++) {
      b_w_data[i] = ((w_data[i] >= 20.0) && (w_data[i] <= rtInf));
    }
    eml_find(b_w_data, iLeft, b_iPk_data, &iRB_size);
    i = w->size[0];
    w->size[0] = iRB_size;
    emxEnsureCapacity_real_T(w, i);
    w_data = w->data;
    for (i = 0; i < iRB_size; i++) {
      w_data[i] = b_iPk_data[i];
    }
    iRB_size = w->size[0];
    loop_ub = w->size[0];
    for (i = 0; i < loop_ub; i++) {
      b_iPk_data[i] = iPk_data[(int)w_data[i] - 1];
    }
    *iPk_size = iRB_size;
    if (iRB_size - 1 >= 0) {
      memcpy(&iPk_data[0], &b_iPk_data[0],
             (unsigned int)iRB_size * sizeof(int));
    }
    emxInit_real_T(&b_bPk, 1);
    i = b_bPk->size[0];
    b_bPk->size[0] = w->size[0];
    emxEnsureCapacity_real_T(b_bPk, i);
    c_bPk_data = b_bPk->data;
    loop_ub = w->size[0];
    for (i = 0; i < loop_ub; i++) {
      c_bPk_data[i] = b_bPk_data[(int)w_data[i] - 1];
    }
    i = bPk->size[0];
    bPk->size[0] = b_bPk->size[0];
    emxEnsureCapacity_real_T(bPk, i);
    b_bPk_data = bPk->data;
    loop_ub = b_bPk->size[0];
    for (i = 0; i < loop_ub; i++) {
      b_bPk_data[i] = c_bPk_data[i];
    }
    emxFree_real_T(&b_bPk);
    emxInit_real_T(&b_bxPk, 2);
    i = b_bxPk->size[0] * b_bxPk->size[1];
    b_bxPk->size[0] = w->size[0];
    b_bxPk->size[1] = 2;
    emxEnsureCapacity_real_T(b_bxPk, i);
    b_bPk_data = b_bxPk->data;
    loop_ub = w->size[0];
    for (i = 0; i < 2; i++) {
      for (i1 = 0; i1 < loop_ub; i1++) {
        b_bPk_data[i1 + b_bxPk->size[0] * i] =
            bxPk_data[((int)w_data[i1] + bxPk->size[0] * i) - 1];
      }
    }
    i = bxPk->size[0] * bxPk->size[1];
    bxPk->size[0] = b_bxPk->size[0];
    bxPk->size[1] = 2;
    emxEnsureCapacity_real_T(bxPk, i);
    bxPk_data = bxPk->data;
    loop_ub = b_bxPk->size[0] * 2;
    for (i = 0; i < loop_ub; i++) {
      bxPk_data[i] = b_bPk_data[i];
    }
    i = b_bxPk->size[0] * b_bxPk->size[1];
    b_bxPk->size[0] = w->size[0];
    b_bxPk->size[1] = 2;
    emxEnsureCapacity_real_T(b_bxPk, i);
    b_bPk_data = b_bxPk->data;
    loop_ub = w->size[0];
    for (i = 0; i < 2; i++) {
      for (i1 = 0; i1 < loop_ub; i1++) {
        b_bPk_data[i1 + b_bxPk->size[0] * i] =
            byPk_data[((int)w_data[i1] + byPk->size[0] * i) - 1];
      }
    }
    i = byPk->size[0] * byPk->size[1];
    byPk->size[0] = b_bxPk->size[0];
    byPk->size[1] = 2;
    emxEnsureCapacity_real_T(byPk, i);
    byPk_data = byPk->data;
    loop_ub = b_bxPk->size[0] * 2;
    for (i = 0; i < loop_ub; i++) {
      byPk_data[i] = b_bPk_data[i];
    }
    i = b_bxPk->size[0] * b_bxPk->size[1];
    b_bxPk->size[0] = w->size[0];
    b_bxPk->size[1] = 2;
    emxEnsureCapacity_real_T(b_bxPk, i);
    b_bPk_data = b_bxPk->data;
    loop_ub = w->size[0];
    for (i = 0; i < 2; i++) {
      for (i1 = 0; i1 < loop_ub; i1++) {
        b_bPk_data[i1 + b_bxPk->size[0] * i] =
            wxPk_data[((int)w_data[i1] + wxPk->size[0] * i) - 1];
      }
    }
    i = wxPk->size[0] * wxPk->size[1];
    wxPk->size[0] = b_bxPk->size[0];
    wxPk->size[1] = 2;
    emxEnsureCapacity_real_T(wxPk, i);
    wxPk_data = wxPk->data;
    loop_ub = b_bxPk->size[0] * 2;
    for (i = 0; i < loop_ub; i++) {
      wxPk_data[i] = b_bPk_data[i];
    }
    emxFree_real_T(&b_bxPk);
  }
  emxFree_real_T(&w);
}

/*
 * File trailer for findExtents.c
 *
 * [EOF]
 */
