/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: findExtents.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 25-Nov-2022 15:36:47
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
static void getLeftBase(const double yTemp[2048], const int iPeak_data[],
                        int iPeak_size, const int iFinite_data[],
                        int iFinite_size, const int iInflect_data[],
                        int iBase_data[], int *iBase_size, int iSaddle_data[],
                        int *iSaddle_size);

/* Function Definitions */
/*
 * Arguments    : const double yTemp[2048]
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
static void getLeftBase(const double yTemp[2048], const int iPeak_data[],
                        int iPeak_size, const int iFinite_data[],
                        int iFinite_size, const int iInflect_data[],
                        int iBase_data[], int *iBase_size, int iSaddle_data[],
                        int *iSaddle_size)
{
  double peak_data[2048];
  double valley_data[2048];
  double v;
  int iValley_data[2048];
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
 * Arguments    : const double y[2048]
 *                int iPk_data[]
 *                int *iPk_size
 *                const int iFin_data[]
 *                int iFin_size
 *                const int iInf_data[]
 *                int iInf_size
 *                const int iInflect_data[]
 *                int iInflect_size
 *                double bPk_data[]
 *                int *bPk_size
 *                emxArray_real_T *bxPk
 *                emxArray_real_T *byPk
 *                emxArray_real_T *wxPk
 * Return Type  : void
 */
void findExtents(const double y[2048], int iPk_data[], int *iPk_size,
                 const int iFin_data[], int iFin_size, const int iInf_data[],
                 int iInf_size, const int iInflect_data[], int iInflect_size,
                 double bPk_data[], int *bPk_size, emxArray_real_T *bxPk,
                 emxArray_real_T *byPk, emxArray_real_T *wxPk)
{
  static double d_bPk_data[4096];
  static double w_data[4096];
  static double b_bPk_data[2048];
  static double base_data[2048];
  static double varargin_2_data[2048];
  static double yFinite[2048];
  static int tmp_data[4096];
  emxArray_real_T *b_bxPk;
  double c_bPk_data[2048];
  double d;
  double *b_bxPk_data;
  double *bxPk_data;
  double *byPk_data;
  double *wxPk_data;
  int c_iPk_data[4096];
  int b_iPk_data[2048];
  int iInfL_data[2048];
  int iInfR_data[2048];
  int iLeftSaddle_data[2048];
  int iRightBase_data[2048];
  int iRightSaddle_data[2048];
  int x_data[2048];
  int b_i;
  int i;
  int iInfL_size;
  int iPk_tmp;
  int iRightSaddle_size;
  int m;
  int w_size;
  int xtmp;
  short idx_size_idx_0;
  boolean_T yFinite_data[2048];
  memcpy(&yFinite[0], &y[0], 2048U * sizeof(double));
  for (i = 0; i < iInf_size; i++) {
    yFinite[iInf_data[i] - 1] = rtNaN;
  }
  m = *iPk_size;
  if (m - 1 >= 0) {
    memcpy(&b_iPk_data[0], &iPk_data[0], (unsigned int)m * sizeof(int));
  }
  getLeftBase(yFinite, iPk_data, *iPk_size, iFin_data, iFin_size, iInflect_data,
              iInfL_data, &iInfL_size, iLeftSaddle_data, &xtmp);
  m = *iPk_size - 1;
  i = *iPk_size >> 1;
  for (b_i = 0; b_i < i; b_i++) {
    xtmp = b_iPk_data[b_i];
    iPk_tmp = m - b_i;
    b_iPk_data[b_i] = b_iPk_data[iPk_tmp];
    b_iPk_data[iPk_tmp] = xtmp;
  }
  if (iFin_size - 1 >= 0) {
    memcpy(&iInfR_data[0], &iFin_data[0],
           (unsigned int)iFin_size * sizeof(int));
  }
  i = iFin_size >> 1;
  for (b_i = 0; b_i < i; b_i++) {
    xtmp = iInfR_data[b_i];
    m = (iFin_size - b_i) - 1;
    iInfR_data[b_i] = iInfR_data[m];
    iInfR_data[m] = xtmp;
  }
  if (iInflect_size - 1 >= 0) {
    memcpy(&x_data[0], &iInflect_data[0],
           (unsigned int)iInflect_size * sizeof(int));
  }
  i = iInflect_size >> 1;
  for (b_i = 0; b_i < i; b_i++) {
    xtmp = x_data[b_i];
    m = (iInflect_size - b_i) - 1;
    x_data[b_i] = x_data[m];
    x_data[m] = xtmp;
  }
  getLeftBase(yFinite, b_iPk_data, *iPk_size, iInfR_data, iFin_size, x_data,
              iRightBase_data, &iPk_tmp, iRightSaddle_data, &iRightSaddle_size);
  i = iPk_tmp >> 1;
  for (b_i = 0; b_i < i; b_i++) {
    xtmp = iRightBase_data[b_i];
    m = (iPk_tmp - b_i) - 1;
    iRightBase_data[b_i] = iRightBase_data[m];
    iRightBase_data[m] = xtmp;
  }
  i = iRightSaddle_size >> 1;
  for (b_i = 0; b_i < i; b_i++) {
    xtmp = iRightSaddle_data[b_i];
    m = (iRightSaddle_size - b_i) - 1;
    iRightSaddle_data[b_i] = iRightSaddle_data[m];
    iRightSaddle_data[m] = xtmp;
  }
  for (i = 0; i < iInfL_size; i++) {
    base_data[i] = yFinite[iInfL_data[i] - 1];
  }
  for (i = 0; i < iPk_tmp; i++) {
    varargin_2_data[i] = yFinite[iRightBase_data[i] - 1];
  }
  if (iInfL_size == iPk_tmp) {
    *bPk_size = iInfL_size;
    for (i = 0; i < iInfL_size; i++) {
      b_bPk_data[i] = fmax(base_data[i], varargin_2_data[i]);
    }
  } else {
    expand_max(base_data, iInfL_size, varargin_2_data, iPk_tmp, b_bPk_data,
               bPk_size);
  }
  if (*iPk_size == *bPk_size) {
    for (i = 0; i < *iPk_size; i++) {
      yFinite_data[i] = (yFinite[iPk_data[i] - 1] - b_bPk_data[i] >= 0.0);
    }
    eml_find(yFinite_data, *iPk_size, tmp_data, &iPk_tmp);
  } else {
    c_binary_expand_op(tmp_data, &iPk_tmp, yFinite, iPk_data, iPk_size,
                       b_bPk_data, bPk_size);
  }
  w_size = iPk_tmp;
  for (i = 0; i < iPk_tmp; i++) {
    w_data[i] = tmp_data[i];
  }
  if (*iPk_size == *bPk_size) {
    for (i = 0; i < *iPk_size; i++) {
      yFinite_data[i] = (yFinite[iPk_data[i] - 1] - b_bPk_data[i] >= 0.0);
    }
    eml_find(yFinite_data, *iPk_size, tmp_data, &iPk_tmp);
  } else {
    c_binary_expand_op(tmp_data, &iPk_tmp, yFinite, iPk_data, iPk_size,
                       b_bPk_data, bPk_size);
  }
  idx_size_idx_0 = (short)iPk_tmp;
  for (i = 0; i < w_size; i++) {
    b_iPk_data[i] = iPk_data[(int)w_data[i] - 1];
  }
  for (i = 0; i < w_size; i++) {
    c_bPk_data[i] = b_bPk_data[(int)w_data[i] - 1];
  }
  for (i = 0; i < w_size; i++) {
    iInfL_data[i] = iLeftSaddle_data[(int)w_data[i] - 1];
  }
  xtmp = w_size;
  if (w_size - 1 >= 0) {
    memcpy(&iLeftSaddle_data[0], &iInfL_data[0],
           (unsigned int)w_size * sizeof(int));
  }
  for (i = 0; i < w_size; i++) {
    iInfL_data[i] = iRightSaddle_data[(int)w_data[i] - 1];
  }
  iRightSaddle_size = w_size;
  if (w_size - 1 >= 0) {
    memcpy(&iRightSaddle_data[0], &iInfL_data[0],
           (unsigned int)w_size * sizeof(int));
  }
  if (*iPk_size == *bPk_size) {
    for (i = 0; i < *iPk_size; i++) {
      yFinite_data[i] = (yFinite[iPk_data[i] - 1] - b_bPk_data[i] >= 0.0);
    }
    eml_find(yFinite_data, *iPk_size, tmp_data, &iPk_tmp);
  } else {
    c_binary_expand_op(tmp_data, &iPk_tmp, yFinite, iPk_data, iPk_size,
                       b_bPk_data, bPk_size);
  }
  for (i = 0; i < iPk_tmp; i++) {
    c_iPk_data[i] = iPk_data[tmp_data[i] - 1];
  }
  *iPk_size = iPk_tmp;
  if (iPk_tmp - 1 >= 0) {
    memcpy(&iPk_data[0], &c_iPk_data[0], (unsigned int)iPk_tmp * sizeof(int));
  }
  if (w_size - 1 >= 0) {
    memcpy(&base_data[0], &c_bPk_data[0],
           (unsigned int)w_size * sizeof(double));
  }
  if (idx_size_idx_0 == 0) {
    xtmp = 0;
    iRightSaddle_size = 0;
  }
  m = w_size << 1;
  if (m - 1 >= 0) {
    memset(&w_data[0], 0, (unsigned int)m * sizeof(double));
  }
  for (b_i = 0; b_i < w_size; b_i++) {
    double refHeight;
    double xc_tmp;
    refHeight = (yFinite[iPk_data[b_i] - 1] + base_data[b_i]) / 2.0;
    m = iPk_data[b_i];
    while ((m >= iLeftSaddle_data[b_i]) && (yFinite[m - 1] > refHeight)) {
      m--;
    }
    i = iLeftSaddle_data[b_i];
    if (m < i) {
      w_data[b_i] = ((double)i - 1.0) + 1.0;
    } else {
      xc_tmp = yFinite[m - 1];
      d = base_data[b_i];
      xc_tmp = (((double)m - 1.0) + 1.0) +
               (0.5 * (yFinite[b_iPk_data[b_i] - 1] + d) - xc_tmp) /
                   (yFinite[m] - xc_tmp);
      if (rtIsNaN(xc_tmp)) {
        if (rtIsInf(d)) {
          xc_tmp = 0.5 * (double)((m + m) + 1);
        } else {
          xc_tmp = (double)m + 1.0;
        }
      }
      w_data[b_i] = xc_tmp;
    }
    m = iPk_data[b_i];
    while ((m <= iRightSaddle_data[b_i]) && (yFinite[m - 1] > refHeight)) {
      m++;
    }
    i = iRightSaddle_data[b_i];
    if (m > i) {
      w_data[b_i + w_size] = ((double)i - 1.0) + 1.0;
    } else {
      xc_tmp = yFinite[m - 1];
      d = base_data[b_i];
      xc_tmp = (((double)m - 1.0) + 1.0) +
               -(0.5 * (yFinite[b_iPk_data[b_i] - 1] + d) - xc_tmp) /
                   (yFinite[m - 2] - xc_tmp);
      if (rtIsNaN(xc_tmp)) {
        if (rtIsInf(d)) {
          xc_tmp = 0.5 * (double)((m + m) - 1);
        } else {
          xc_tmp = ((double)m - 2.0) + 1.0;
        }
      }
      w_data[b_i + w_size] = xc_tmp;
    }
  }
  do_vectors(b_iPk_data, w_size, iInf_data, iInf_size, iPk_data, iPk_size,
             iInfL_data, &iInfL_size, iInfR_data, &m);
  b_do_vectors(iPk_data, *iPk_size, b_iPk_data, w_size, x_data, &iPk_tmp,
               iInfL_data, &iInfL_size, iInfR_data, &m);
  for (i = 0; i < iInfL_size; i++) {
    base_data[i] = iInfL_data[i];
  }
  b_do_vectors(iPk_data, *iPk_size, iInf_data, iInf_size, x_data, &iPk_tmp,
               iInfL_data, &iInfL_size, iInfR_data, &m);
  for (i = 0; i < iInfL_size; i++) {
    varargin_2_data[i] = iInfL_data[i];
  }
  idx_size_idx_0 = (short)*iPk_size;
  *bPk_size = idx_size_idx_0;
  m = idx_size_idx_0;
  if (m - 1 >= 0) {
    memset(&bPk_data[0], 0, (unsigned int)m * sizeof(double));
  }
  for (i = 0; i < w_size; i++) {
    bPk_data[(int)base_data[i] - 1] = c_bPk_data[i];
  }
  for (i = 0; i < iInfL_size; i++) {
    iInfL_data[i] = (int)varargin_2_data[i];
  }
  for (i = 0; i < iInfL_size; i++) {
    bPk_data[iInfL_data[i] - 1] = 0.0;
  }
  for (i = 0; i < iInf_size; i++) {
    m = iInf_data[i] - 1;
    if (m < 1) {
      m = 1;
    }
    iInfL_data[i] = m;
  }
  for (i = 0; i < iInf_size; i++) {
    m = iInf_data[i] + 1;
    if (m > 2048) {
      iInfR_data[i] = 2048;
    } else {
      iInfR_data[i] = m;
    }
  }
  i = bxPk->size[0] * bxPk->size[1];
  bxPk->size[0] = *iPk_size;
  bxPk->size[1] = 2;
  emxEnsureCapacity_real_T(bxPk, i);
  bxPk_data = bxPk->data;
  m = *iPk_size << 1;
  for (i = 0; i < m; i++) {
    bxPk_data[i] = 0.0;
  }
  for (i = 0; i < xtmp; i++) {
    bxPk_data[(int)base_data[i] - 1] =
        (short)((short)(iLeftSaddle_data[i] - 1) + 1);
  }
  for (i = 0; i < iRightSaddle_size; i++) {
    bxPk_data[((int)base_data[i] + bxPk->size[0]) - 1] =
        (short)((short)(iRightSaddle_data[i] - 1) + 1);
  }
  for (i = 0; i < iInf_size; i++) {
    bxPk_data[(int)varargin_2_data[i] - 1] =
        0.5 * (double)(short)((short)((short)(iInf_data[i] - 1) +
                                      (short)(iInfL_data[i] - 1)) +
                              2);
  }
  for (i = 0; i < iInf_size; i++) {
    bxPk_data[((int)varargin_2_data[i] + bxPk->size[0]) - 1] =
        0.5 * (double)(short)((short)((short)(iInf_data[i] - 1) +
                                      (short)(iInfR_data[i] - 1)) +
                              2);
  }
  i = byPk->size[0] * byPk->size[1];
  byPk->size[0] = *iPk_size;
  byPk->size[1] = 2;
  emxEnsureCapacity_real_T(byPk, i);
  byPk_data = byPk->data;
  for (i = 0; i < m; i++) {
    byPk_data[i] = 0.0;
  }
  for (i = 0; i < xtmp; i++) {
    byPk_data[(int)base_data[i] - 1] = y[iLeftSaddle_data[i] - 1];
  }
  for (i = 0; i < iRightSaddle_size; i++) {
    byPk_data[((int)base_data[i] + byPk->size[0]) - 1] =
        y[iRightSaddle_data[i] - 1];
  }
  for (i = 0; i < iInf_size; i++) {
    byPk_data[(int)varargin_2_data[i] - 1] = y[iInfL_data[i] - 1];
  }
  for (i = 0; i < iInf_size; i++) {
    byPk_data[((int)varargin_2_data[i] + byPk->size[0]) - 1] =
        y[iInfR_data[i] - 1];
  }
  i = wxPk->size[0] * wxPk->size[1];
  wxPk->size[0] = *iPk_size;
  wxPk->size[1] = 2;
  emxEnsureCapacity_real_T(wxPk, i);
  wxPk_data = wxPk->data;
  for (i = 0; i < m; i++) {
    wxPk_data[i] = 0.0;
  }
  for (i = 0; i < 2; i++) {
    for (m = 0; m < w_size; m++) {
      wxPk_data[((int)base_data[m] + wxPk->size[0] * i) - 1] =
          w_data[m + w_size * i];
    }
  }
  for (i = 0; i < iInf_size; i++) {
    wxPk_data[(int)varargin_2_data[i] - 1] =
        0.5 * (double)(short)((short)((short)(iInf_data[i] - 1) +
                                      (short)(iInfL_data[i] - 1)) +
                              2);
  }
  for (i = 0; i < iInf_size; i++) {
    wxPk_data[((int)varargin_2_data[i] + wxPk->size[0]) - 1] =
        0.5 * (double)(short)((short)((short)(iInf_data[i] - 1) +
                                      (short)(iInfR_data[i] - 1)) +
                              2);
  }
  if (*iPk_size != 0) {
    boolean_T b_w_data[4096];
    w_size = wxPk->size[0];
    if (wxPk->size[0] != 0) {
      m = wxPk->size[0];
      for (iPk_tmp = 0; iPk_tmp < m; iPk_tmp++) {
        w_data[iPk_tmp] = wxPk_data[iPk_tmp + m] - wxPk_data[iPk_tmp];
      }
    }
    for (i = 0; i < w_size; i++) {
      d = w_data[i];
      b_w_data[i] = ((d >= 30.0) && (d <= rtInf));
    }
    eml_find(b_w_data, w_size, tmp_data, &iPk_tmp);
    for (i = 0; i < iPk_tmp; i++) {
      w_data[i] = tmp_data[i];
    }
    for (i = 0; i < iPk_tmp; i++) {
      c_iPk_data[i] = iPk_data[(int)w_data[i] - 1];
    }
    *iPk_size = iPk_tmp;
    if (iPk_tmp - 1 >= 0) {
      memcpy(&iPk_data[0], &c_iPk_data[0], (unsigned int)iPk_tmp * sizeof(int));
    }
    for (i = 0; i < iPk_tmp; i++) {
      d_bPk_data[i] = bPk_data[(int)w_data[i] - 1];
    }
    *bPk_size = iPk_tmp;
    if (iPk_tmp - 1 >= 0) {
      memcpy(&bPk_data[0], &d_bPk_data[0],
             (unsigned int)iPk_tmp * sizeof(double));
    }
    emxInit_real_T(&b_bxPk);
    i = b_bxPk->size[0] * b_bxPk->size[1];
    b_bxPk->size[0] = iPk_tmp;
    b_bxPk->size[1] = 2;
    emxEnsureCapacity_real_T(b_bxPk, i);
    b_bxPk_data = b_bxPk->data;
    for (i = 0; i < 2; i++) {
      for (m = 0; m < iPk_tmp; m++) {
        b_bxPk_data[m + b_bxPk->size[0] * i] =
            bxPk_data[((int)w_data[m] + bxPk->size[0] * i) - 1];
      }
    }
    i = bxPk->size[0] * bxPk->size[1];
    bxPk->size[0] = b_bxPk->size[0];
    bxPk->size[1] = 2;
    emxEnsureCapacity_real_T(bxPk, i);
    bxPk_data = bxPk->data;
    m = b_bxPk->size[0] * 2;
    for (i = 0; i < m; i++) {
      bxPk_data[i] = b_bxPk_data[i];
    }
    i = b_bxPk->size[0] * b_bxPk->size[1];
    b_bxPk->size[0] = iPk_tmp;
    b_bxPk->size[1] = 2;
    emxEnsureCapacity_real_T(b_bxPk, i);
    b_bxPk_data = b_bxPk->data;
    for (i = 0; i < 2; i++) {
      for (m = 0; m < iPk_tmp; m++) {
        b_bxPk_data[m + b_bxPk->size[0] * i] =
            byPk_data[((int)w_data[m] + byPk->size[0] * i) - 1];
      }
    }
    i = byPk->size[0] * byPk->size[1];
    byPk->size[0] = b_bxPk->size[0];
    byPk->size[1] = 2;
    emxEnsureCapacity_real_T(byPk, i);
    byPk_data = byPk->data;
    m = b_bxPk->size[0] * 2;
    for (i = 0; i < m; i++) {
      byPk_data[i] = b_bxPk_data[i];
    }
    i = b_bxPk->size[0] * b_bxPk->size[1];
    b_bxPk->size[0] = iPk_tmp;
    b_bxPk->size[1] = 2;
    emxEnsureCapacity_real_T(b_bxPk, i);
    b_bxPk_data = b_bxPk->data;
    for (i = 0; i < 2; i++) {
      for (m = 0; m < iPk_tmp; m++) {
        b_bxPk_data[m + b_bxPk->size[0] * i] =
            wxPk_data[((int)w_data[m] + wxPk->size[0] * i) - 1];
      }
    }
    i = wxPk->size[0] * wxPk->size[1];
    wxPk->size[0] = b_bxPk->size[0];
    wxPk->size[1] = 2;
    emxEnsureCapacity_real_T(wxPk, i);
    wxPk_data = wxPk->data;
    m = b_bxPk->size[0] * 2;
    for (i = 0; i < m; i++) {
      wxPk_data[i] = b_bxPk_data[i];
    }
    emxFree_real_T(&b_bxPk);
  }
}

/*
 * File trailer for findExtents.c
 *
 * [EOF]
 */
