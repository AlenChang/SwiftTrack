/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * findpeaks.c
 *
 * Code generation for function 'findpeaks'
 *
 */

/* Include files */
#include "findpeaks.h"
#include "eml_setop.h"
#include "find.h"
#include "recalibrateHistory_emxutil.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void findExtents(const double y[2048], int iPk_data[], int *iPk_size,
                        const int iFin_data[], int iFin_size,
                        const int iInf_data[], int iInf_size,
                        const int iInflect_data[], int iInflect_size,
                        double bPk_data[], int *bPk_size, emxArray_real_T *bxPk,
                        emxArray_real_T *byPk, emxArray_real_T *wxPk);

static void getLeftBase(const double yTemp[2048], const int iPeak_data[],
                        int iPeak_size, const int iFinite_data[],
                        int iFinite_size, const int iInflect_data[],
                        int iBase_data[], int *iBase_size, int iSaddle_data[],
                        int *iSaddle_size);

/* Function Definitions */
static void findExtents(const double y[2048], int iPk_data[], int *iPk_size,
                        const int iFin_data[], int iFin_size,
                        const int iInf_data[], int iInf_size,
                        const int iInflect_data[], int iInflect_size,
                        double bPk_data[], int *bPk_size, emxArray_real_T *bxPk,
                        emxArray_real_T *byPk, emxArray_real_T *wxPk)
{
  static double c_bPk_data[4096];
  static double w_data[4096];
  static double b_bPk_data[2048];
  static double base_data[2048];
  static double yFinite[2048];
  static int tmp_data[4096];
  emxArray_real_T *b_bxPk;
  double refHeight;
  double xc_tmp;
  double *b_bxPk_data;
  double *bxPk_data;
  double *byPk_data;
  double *wxPk_data;
  int c_iPk_data[4096];
  int b_iPk_data[2048];
  int iInfL_data[2048];
  int iInfR_data[2048];
  int iLeftBase_data[2048];
  int iLeftSaddle_data[2048];
  int iRightBase_data[2048];
  int iRightSaddle_data[2048];
  int idx_data[2048];
  int x_data[2048];
  int b_i;
  int i;
  int iLeftBase_size;
  int iPk_tmp;
  int iRightSaddle_size;
  int m;
  int md2;
  int w_size;
  int xtmp;
  short csz_idx_0;
  boolean_T b_tmp_data[4096];
  boolean_T yFinite_data[2048];
  memcpy(&yFinite[0], &y[0], 2048U * sizeof(double));
  for (i = 0; i < iInf_size; i++) {
    yFinite[iInf_data[i] - 1] = rtNaN;
  }
  m = *iPk_size;
  if (0 <= m - 1) {
    memcpy(&b_iPk_data[0], &iPk_data[0], m * sizeof(int));
  }
  getLeftBase(yFinite, iPk_data, *iPk_size, iFin_data, iFin_size, iInflect_data,
              iLeftBase_data, &iLeftBase_size, iLeftSaddle_data, &xtmp);
  m = *iPk_size - 1;
  md2 = *iPk_size >> 1;
  for (b_i = 0; b_i < md2; b_i++) {
    xtmp = b_iPk_data[b_i];
    iPk_tmp = m - b_i;
    b_iPk_data[b_i] = b_iPk_data[iPk_tmp];
    b_iPk_data[iPk_tmp] = xtmp;
  }
  if (0 <= iFin_size - 1) {
    memcpy(&x_data[0], &iFin_data[0], iFin_size * sizeof(int));
  }
  md2 = iFin_size >> 1;
  for (b_i = 0; b_i < md2; b_i++) {
    xtmp = x_data[b_i];
    m = (iFin_size - b_i) - 1;
    x_data[b_i] = x_data[m];
    x_data[m] = xtmp;
  }
  if (0 <= iInflect_size - 1) {
    memcpy(&idx_data[0], &iInflect_data[0], iInflect_size * sizeof(int));
  }
  md2 = iInflect_size >> 1;
  for (b_i = 0; b_i < md2; b_i++) {
    xtmp = idx_data[b_i];
    m = (iInflect_size - b_i) - 1;
    idx_data[b_i] = idx_data[m];
    idx_data[m] = xtmp;
  }
  getLeftBase(yFinite, b_iPk_data, *iPk_size, x_data, iFin_size, idx_data,
              iRightBase_data, &iPk_tmp, iRightSaddle_data, &iRightSaddle_size);
  md2 = iPk_tmp >> 1;
  for (b_i = 0; b_i < md2; b_i++) {
    xtmp = iRightBase_data[b_i];
    m = (iPk_tmp - b_i) - 1;
    iRightBase_data[b_i] = iRightBase_data[m];
    iRightBase_data[m] = xtmp;
  }
  md2 = iRightSaddle_size >> 1;
  for (b_i = 0; b_i < md2; b_i++) {
    xtmp = iRightSaddle_data[b_i];
    m = (iRightSaddle_size - b_i) - 1;
    iRightSaddle_data[b_i] = iRightSaddle_data[m];
    iRightSaddle_data[m] = xtmp;
  }
  if (iLeftBase_size <= iPk_tmp) {
    m = (short)iLeftBase_size;
  } else {
    m = (short)iPk_tmp;
  }
  for (iPk_tmp = 0; iPk_tmp < m; iPk_tmp++) {
    base_data[iPk_tmp] = fmax(yFinite[iLeftBase_data[iPk_tmp] - 1],
                              yFinite[iRightBase_data[iPk_tmp] - 1]);
  }
  for (i = 0; i < *iPk_size; i++) {
    yFinite_data[i] = (yFinite[iPk_data[i] - 1] - base_data[i] >= 0.0);
  }
  eml_find(yFinite_data, *iPk_size, tmp_data, &iPk_tmp);
  w_size = iPk_tmp;
  for (i = 0; i < iPk_tmp; i++) {
    w_data[i] = tmp_data[i];
  }
  for (i = 0; i < *iPk_size; i++) {
    yFinite_data[i] = (yFinite[iPk_data[i] - 1] - base_data[i] >= 0.0);
  }
  eml_find(yFinite_data, *iPk_size, tmp_data, &iPk_tmp);
  md2 = iPk_tmp;
  for (i = 0; i < w_size; i++) {
    b_iPk_data[i] = iPk_data[(int)w_data[i] - 1];
  }
  for (i = 0; i < w_size; i++) {
    b_bPk_data[i] = base_data[(int)w_data[i] - 1];
  }
  for (i = 0; i < w_size; i++) {
    x_data[i] = iLeftSaddle_data[(int)w_data[i] - 1];
  }
  xtmp = w_size;
  if (0 <= w_size - 1) {
    memcpy(&iLeftSaddle_data[0], &x_data[0], w_size * sizeof(int));
  }
  for (i = 0; i < w_size; i++) {
    x_data[i] = iRightSaddle_data[(int)w_data[i] - 1];
  }
  iRightSaddle_size = w_size;
  if (0 <= w_size - 1) {
    memcpy(&iRightSaddle_data[0], &x_data[0], w_size * sizeof(int));
  }
  for (i = 0; i < *iPk_size; i++) {
    yFinite_data[i] = (yFinite[iPk_data[i] - 1] - base_data[i] >= 0.0);
  }
  eml_find(yFinite_data, *iPk_size, tmp_data, &iPk_tmp);
  for (i = 0; i < iPk_tmp; i++) {
    c_iPk_data[i] = iPk_data[tmp_data[i] - 1];
  }
  *iPk_size = iPk_tmp;
  if (0 <= iPk_tmp - 1) {
    memcpy(&iPk_data[0], &c_iPk_data[0], iPk_tmp * sizeof(int));
  }
  if (0 <= w_size - 1) {
    memcpy(&base_data[0], &b_bPk_data[0], w_size * sizeof(double));
  }
  if (md2 == 0) {
    xtmp = 0;
    iRightSaddle_size = 0;
  }
  m = w_size << 1;
  if (0 <= m - 1) {
    memset(&w_data[0], 0, m * sizeof(double));
  }
  for (b_i = 0; b_i < w_size; b_i++) {
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
      xc_tmp =
          (((double)m - 1.0) + 1.0) +
          (0.5 * (yFinite[b_iPk_data[b_i] - 1] + base_data[b_i]) - xc_tmp) /
              (yFinite[m] - xc_tmp);
      if (rtIsNaN(xc_tmp)) {
        if (rtIsInf(base_data[b_i])) {
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
      xc_tmp =
          (((double)m - 1.0) + 1.0) +
          -(0.5 * (yFinite[b_iPk_data[b_i] - 1] + base_data[b_i]) - xc_tmp) /
              (yFinite[m - 2] - xc_tmp);
      if (rtIsNaN(xc_tmp)) {
        if (rtIsInf(base_data[b_i])) {
          xc_tmp = 0.5 * (double)((m + m) - 1);
        } else {
          xc_tmp = ((double)m - 2.0) + 1.0;
        }
      }
      w_data[b_i + w_size] = xc_tmp;
    }
  }
  do_vectors(b_iPk_data, w_size, iInf_data, iInf_size, iPk_data, iPk_size,
             iLeftBase_data, &iLeftBase_size, x_data, &m);
  b_do_vectors(iPk_data, *iPk_size, b_iPk_data, w_size, idx_data, &md2,
               iLeftBase_data, &iLeftBase_size, x_data, &m);
  for (i = 0; i < iLeftBase_size; i++) {
    base_data[i] = iLeftBase_data[i];
  }
  b_do_vectors(iPk_data, *iPk_size, iInf_data, iInf_size, idx_data, &md2,
               iLeftBase_data, &iLeftBase_size, x_data, &m);
  if (0 <= iLeftBase_size - 1) {
    memcpy(&idx_data[0], &iLeftBase_data[0], iLeftBase_size * sizeof(int));
  }
  csz_idx_0 = (short)*iPk_size;
  *bPk_size = csz_idx_0;
  m = csz_idx_0;
  if (0 <= m - 1) {
    memset(&bPk_data[0], 0, m * sizeof(double));
  }
  for (i = 0; i < w_size; i++) {
    bPk_data[(int)base_data[i] - 1] = b_bPk_data[i];
  }
  for (i = 0; i < iLeftBase_size; i++) {
    bPk_data[iLeftBase_data[i] - 1] = 0.0;
  }
  for (i = 0; i < iInf_size; i++) {
    iLeftBase_data[i] = iInf_data[i] - 1;
  }
  for (iPk_tmp = 0; iPk_tmp < iInf_size; iPk_tmp++) {
    i = iLeftBase_data[iPk_tmp];
    if (1 < i) {
      iInfL_data[iPk_tmp] = i;
    } else {
      iInfL_data[iPk_tmp] = 1;
    }
  }
  for (i = 0; i < iInf_size; i++) {
    iLeftBase_data[i] = iInf_data[i] + 1;
  }
  for (iPk_tmp = 0; iPk_tmp < iInf_size; iPk_tmp++) {
    i = iLeftBase_data[iPk_tmp];
    if (i > 2048) {
      iInfR_data[iPk_tmp] = 2048;
    } else {
      iInfR_data[iPk_tmp] = i;
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
    bxPk_data[idx_data[i] - 1] =
        0.5 * (double)(short)((short)((short)(iInf_data[i] - 1) +
                                      (short)(iInfL_data[i] - 1)) +
                              2);
  }
  for (i = 0; i < iInf_size; i++) {
    bxPk_data[(idx_data[i] + bxPk->size[0]) - 1] =
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
    byPk_data[idx_data[i] - 1] = y[iInfL_data[i] - 1];
  }
  for (i = 0; i < iInf_size; i++) {
    byPk_data[(idx_data[i] + byPk->size[0]) - 1] = y[iInfR_data[i] - 1];
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
    wxPk_data[idx_data[i] - 1] =
        0.5 * (double)(short)((short)((short)(iInf_data[i] - 1) +
                                      (short)(iInfL_data[i] - 1)) +
                              2);
  }
  for (i = 0; i < iInf_size; i++) {
    wxPk_data[(idx_data[i] + wxPk->size[0]) - 1] =
        0.5 * (double)(short)((short)((short)(iInf_data[i] - 1) +
                                      (short)(iInfR_data[i] - 1)) +
                              2);
  }
  if (*iPk_size != 0) {
    w_size = (short)wxPk->size[0];
    if ((short)wxPk->size[0] != 0) {
      m = wxPk->size[0];
      for (iPk_tmp = 0; iPk_tmp < m; iPk_tmp++) {
        w_data[iPk_tmp] = wxPk_data[iPk_tmp + m] - wxPk_data[iPk_tmp];
      }
    }
    for (i = 0; i < w_size; i++) {
      xc_tmp = w_data[i];
      b_tmp_data[i] = ((50.0 <= xc_tmp) && (xc_tmp <= rtInf));
    }
    eml_find(b_tmp_data, w_size, tmp_data, &iPk_tmp);
    for (i = 0; i < iPk_tmp; i++) {
      w_data[i] = tmp_data[i];
    }
    for (i = 0; i < iPk_tmp; i++) {
      c_iPk_data[i] = iPk_data[(int)w_data[i] - 1];
    }
    *iPk_size = iPk_tmp;
    if (0 <= iPk_tmp - 1) {
      memcpy(&iPk_data[0], &c_iPk_data[0], iPk_tmp * sizeof(int));
    }
    for (i = 0; i < iPk_tmp; i++) {
      c_bPk_data[i] = bPk_data[(int)w_data[i] - 1];
    }
    *bPk_size = iPk_tmp;
    if (0 <= iPk_tmp - 1) {
      memcpy(&bPk_data[0], &c_bPk_data[0], iPk_tmp * sizeof(double));
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

static void getLeftBase(const double yTemp[2048], const int iPeak_data[],
                        int iPeak_size, const int iFinite_data[],
                        int iFinite_size, const int iInflect_data[],
                        int iBase_data[], int *iBase_size, int iSaddle_data[],
                        int *iSaddle_size)
{
  double peak_data[2048];
  double valley_data[2048];
  double p;
  double v;
  int iValley_data[2048];
  int i;
  int isv;
  int iv;
  int j;
  int k;
  int n;
  n = (short)iPeak_size;
  *iBase_size = (short)iPeak_size;
  if (0 <= n - 1) {
    memset(&iBase_data[0], 0, n * sizeof(int));
  }
  *iSaddle_size = (short)iPeak_size;
  if (0 <= n - 1) {
    memset(&iSaddle_data[0], 0, n * sizeof(int));
  }
  n = (short)iFinite_size;
  if (0 <= n - 1) {
    memset(&peak_data[0], 0, n * sizeof(double));
  }
  if (0 <= n - 1) {
    memset(&valley_data[0], 0, n * sizeof(double));
  }
  if (0 <= n - 1) {
    memset(&iValley_data[0], 0, n * sizeof(int));
  }
  n = -1;
  i = 0;
  j = 0;
  k = 0;
  v = rtNaN;
  iv = 1;
  while (k + 1 <= iPeak_size) {
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

void findpeaks(const double Yin[2048], double Ypk_data[], int *Ypk_size,
               double Xpk_data[], int *Xpk_size)
{
  static double bPk_data[4096];
  static int b_iPk_data[4096];
  static int idx_data[4096];
  static int iwork_data[4096];
  static int sortIdx_data[4096];
  emxArray_real_T *bxPk;
  emxArray_real_T *byPk;
  emxArray_real_T *wxPk;
  double yk;
  double ykfirst;
  int iFinite_data[2048];
  int iInfinite_data[2048];
  int iInflect_data[2048];
  int b_i;
  int i;
  int k;
  int kEnd;
  int kfirst;
  int loop_ub;
  int nInf;
  int nInflect;
  int nPk;
  int pEnd;
  int qEnd;
  int sortIdx_size_tmp;
  short b_tmp_data[4096];
  short locs_temp_data[4096];
  short iPk_data[2048];
  short i1;
  char dir;
  char previousdir;
  boolean_T idelete_data[4096];
  boolean_T tmp_data[4096];
  boolean_T isinfyk;
  boolean_T isinfykfirst;
  nPk = -1;
  nInf = -1;
  nInflect = -1;
  dir = 'n';
  kfirst = 0;
  ykfirst = rtInf;
  isinfykfirst = true;
  for (k = 0; k < 2048; k++) {
    yk = Yin[k];
    if (rtIsNaN(yk)) {
      yk = rtInf;
      isinfyk = true;
    } else if (rtIsInf(yk) && (yk > 0.0)) {
      isinfyk = true;
      nInf++;
      iInfinite_data[nInf] = k + 1;
    } else {
      isinfyk = false;
    }
    if (yk != ykfirst) {
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
        if (kfirst >= 1) {
          nInflect++;
          iInflect_data[nInflect] = kfirst;
        }
      } else if (yk < ykfirst) {
        dir = 'd';
        if ('d' != previousdir) {
          nInflect++;
          iInflect_data[nInflect] = kfirst;
          if (previousdir == 'i') {
            nPk++;
            iFinite_data[nPk] = kfirst;
          }
        }
      } else {
        dir = 'i';
        if ('i' != previousdir) {
          nInflect++;
          iInflect_data[nInflect] = kfirst;
        }
      }
      ykfirst = yk;
      kfirst = k + 1;
      isinfykfirst = isinfyk;
    }
  }
  if ((!isinfykfirst) &&
      ((nInflect + 1 == 0) || (iInflect_data[nInflect] < 2048))) {
    nInflect++;
    iInflect_data[nInflect] = 2048;
  }
  if (1 > nPk + 1) {
    pEnd = -1;
  } else {
    pEnd = nPk;
  }
  nPk = 0;
  kfirst = pEnd + 1;
  for (k = 0; k < kfirst; k++) {
    i = iFinite_data[k];
    ykfirst = Yin[i - 1];
    if ((ykfirst > rtMinusInf) && (ykfirst - fmax(Yin[i - 2], Yin[i]) >= 0.0)) {
      nPk++;
      iPk_data[nPk - 1] = (short)i;
    }
  }
  if (1 > nPk) {
    loop_ub = 0;
  } else {
    loop_ub = nPk;
  }
  for (i = 0; i < loop_ub; i++) {
    b_iPk_data[i] = iPk_data[i];
  }
  emxInit_real_T(&bxPk);
  emxInit_real_T(&byPk);
  emxInit_real_T(&wxPk);
  if (1 > nInf + 1) {
    nInf = -1;
  }
  if (1 > nInflect + 1) {
    nInflect = -1;
  }
  findExtents(Yin, b_iPk_data, &loop_ub, iFinite_data, pEnd + 1, iInfinite_data,
              nInf + 1, iInflect_data, nInflect + 1, bPk_data, &nPk, bxPk, byPk,
              wxPk);
  emxFree_real_T(&wxPk);
  emxFree_real_T(&byPk);
  emxFree_real_T(&bxPk);
  if (loop_ub == 0) {
    *Ypk_size = 0;
  } else {
    sortIdx_size_tmp = (short)loop_ub;
    if (0 <= sortIdx_size_tmp - 1) {
      memset(&sortIdx_data[0], 0, sortIdx_size_tmp * sizeof(int));
    }
    i = loop_ub - 1;
    for (k = 1; k <= i; k += 2) {
      ykfirst = Yin[b_iPk_data[k - 1] - 1];
      if ((ykfirst >= Yin[b_iPk_data[k] - 1]) || rtIsNaN(ykfirst)) {
        sortIdx_data[k - 1] = k;
        sortIdx_data[k] = k + 1;
      } else {
        sortIdx_data[k - 1] = k + 1;
        sortIdx_data[k] = k;
      }
    }
    if ((loop_ub & 1) != 0) {
      sortIdx_data[loop_ub - 1] = loop_ub;
    }
    b_i = 2;
    while (b_i < loop_ub) {
      kfirst = b_i << 1;
      nPk = 1;
      for (pEnd = b_i + 1; pEnd < loop_ub + 1; pEnd = qEnd + b_i) {
        nInf = nPk - 1;
        nInflect = pEnd;
        qEnd = nPk + kfirst;
        if (qEnd > loop_ub + 1) {
          qEnd = loop_ub + 1;
        }
        k = 0;
        kEnd = qEnd - nPk;
        while (k + 1 <= kEnd) {
          ykfirst = Yin[b_iPk_data[sortIdx_data[nInf] - 1] - 1];
          i = sortIdx_data[nInflect - 1];
          if ((ykfirst >= Yin[b_iPk_data[i - 1] - 1]) || rtIsNaN(ykfirst)) {
            iwork_data[k] = sortIdx_data[nInf];
            nInf++;
            if (nInf + 1 == pEnd) {
              while (nInflect < qEnd) {
                k++;
                iwork_data[k] = sortIdx_data[nInflect - 1];
                nInflect++;
              }
            }
          } else {
            iwork_data[k] = i;
            nInflect++;
            if (nInflect == qEnd) {
              while (nInf + 1 < pEnd) {
                k++;
                iwork_data[k] = sortIdx_data[nInf];
                nInf++;
              }
            }
          }
          k++;
        }
        for (k = 0; k < kEnd; k++) {
          sortIdx_data[(nPk + k) - 1] = iwork_data[k];
        }
        nPk = qEnd;
      }
      b_i = kfirst;
    }
    for (i = 0; i < sortIdx_size_tmp; i++) {
      locs_temp_data[i] =
          (short)((short)(b_iPk_data[sortIdx_data[i] - 1] - 1) + 1);
    }
    if (0 <= sortIdx_size_tmp - 1) {
      memset(&idelete_data[0], 0, sortIdx_size_tmp * sizeof(boolean_T));
    }
    for (b_i = 0; b_i < sortIdx_size_tmp; b_i++) {
      if (!idelete_data[b_i]) {
        i = sortIdx_data[b_i];
        for (nPk = 0; nPk < sortIdx_size_tmp; nPk++) {
          kfirst = (short)((short)(b_iPk_data[i - 1] - 1) + 1);
          i1 = locs_temp_data[nPk];
          tmp_data[nPk] = ((i1 >= kfirst - 100) && (i1 <= kfirst + 100));
        }
        for (i = 0; i < sortIdx_size_tmp; i++) {
          idelete_data[i] = (idelete_data[i] || tmp_data[i]);
        }
        idelete_data[b_i] = false;
      }
    }
    kfirst = (short)loop_ub - 1;
    *Ypk_size = 0;
    nPk = 0;
    for (b_i = 0; b_i <= kfirst; b_i++) {
      if (!idelete_data[b_i]) {
        (*Ypk_size)++;
        b_tmp_data[nPk] = (short)(b_i + 1);
        nPk++;
      }
    }
    for (i = 0; i < *Ypk_size; i++) {
      idx_data[i] = sortIdx_data[b_tmp_data[i] - 1];
    }
    sort(idx_data, Ypk_size);
  }
  if (*Ypk_size > 2048) {
    *Ypk_size = 2048;
  }
  for (i = 0; i < *Ypk_size; i++) {
    iwork_data[i] = b_iPk_data[idx_data[i] - 1];
  }
  for (i = 0; i < *Ypk_size; i++) {
    Ypk_data[i] = Yin[iwork_data[i] - 1];
  }
  *Xpk_size = *Ypk_size;
  for (i = 0; i < *Ypk_size; i++) {
    Xpk_data[i] = (short)((short)(iwork_data[i] - 1) + 1);
  }
}

/* End of code generation (findpeaks.c) */
