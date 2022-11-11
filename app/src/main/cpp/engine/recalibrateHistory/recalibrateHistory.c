/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * recalibrateHistory.c
 *
 * Code generation for function 'recalibrateHistory'
 *
 */

/* Include files */
#include "recalibrateHistory.h"
#include "findpeaks.h"
#include "recalibrateHistory_emxutil.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void binary_expand_op(double hist[2048], int i1, int i2, int i3,
                             const double drift_data[], const int *drift_size,
                             int i4, int i5);

/* Function Definitions */
static void binary_expand_op(double hist[2048], int i1, int i2, int i3,
                             const double drift_data[], const int *drift_size,
                             int i4, int i5)
{
  double hist_data[2048];
  int i;
  int stride_0_1;
  int stride_1_1;
  int unnamed_idx_1;
  unnamed_idx_1 = i4 - i5;
  stride_0_1 = ((i3 - i2) + 1 != 1);
  stride_1_1 = (*drift_size != 1);
  for (i = 0; i < unnamed_idx_1; i++) {
    hist_data[i] = hist[i2 + i * stride_0_1] - drift_data[i * stride_1_1];
  }
  for (i = 0; i < unnamed_idx_1; i++) {
    hist[i1 + i] = hist_data[i];
  }
}

void recalibrateHistory(double hist[2048], double hist_out[2048],
                        double pks_data[], int pks_size[1], double locs_data[],
                        int locs_size[1])
{
  static double bPk_data[4096];
  static double hist_data[2048];
  static double b_drift_data[2047];
  static double drift_data[2047];
  static int c_iPk_data[4096];
  static int iPk_data[4096];
  static int idx_data[4096];
  emxArray_real_T *bxPk;
  emxArray_real_T *byPk;
  emxArray_real_T *wxPk;
  double b_n_tmp;
  double delta1;
  double n;
  double n_tmp;
  double yk;
  double ykfirst;
  int iFinite_data[2048];
  int iInfinite_data[2048];
  int iInflect_data[2048];
  int i;
  int k;
  int kfirst;
  int loop_ub;
  int nInf;
  int nInflect;
  int nPk;
  int ti;
  short b_iPk_data[2048];
  char dir;
  char previousdir;
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
    yk = hist[k];
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
    loop_ub = -1;
  } else {
    loop_ub = nPk;
  }
  nPk = 0;
  kfirst = loop_ub + 1;
  for (k = 0; k < kfirst; k++) {
    i = iFinite_data[k];
    ykfirst = hist[i - 1];
    if ((ykfirst > rtMinusInf) &&
        (ykfirst - fmax(hist[i - 2], hist[i]) >= 0.0)) {
      nPk++;
      b_iPk_data[nPk - 1] = (short)i;
    }
  }
  if (1 > nPk) {
    nPk = 0;
  }
  for (i = 0; i < nPk; i++) {
    iPk_data[i] = b_iPk_data[i];
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
  findExtents(hist, iPk_data, &nPk, iFinite_data, loop_ub + 1, iInfinite_data,
              nInf + 1, iInflect_data, nInflect + 1, bPk_data, &kfirst, bxPk,
              byPk, wxPk);
  c_findPeaksSeparatedByMoreThanM(hist, iPk_data, nPk, idx_data, &nInflect);
  emxFree_real_T(&wxPk);
  emxFree_real_T(&byPk);
  emxFree_real_T(&bxPk);
  if (nInflect > 2048) {
    nInflect = 2048;
  }
  for (i = 0; i < nInflect; i++) {
    c_iPk_data[i] = iPk_data[idx_data[i] - 1];
  }
  pks_size[0] = nInflect;
  for (i = 0; i < nInflect; i++) {
    pks_data[i] = hist[c_iPk_data[i] - 1];
  }
  locs_size[0] = nInflect;
  for (i = 0; i < nInflect; i++) {
    locs_data[i] = (short)((short)(c_iPk_data[i] - 1) + 1);
  }
  if (nInflect >= 2) {
    for (ti = 0; ti <= nInflect - 2; ti++) {
      yk = pks_data[ti];
      n_tmp = (double)(iPk_data[idx_data[ti] - 1] - 1) + 1.0;
      b_n_tmp = (double)(iPk_data[idx_data[ti + 1] - 1] - 1) + 1.0;
      n = b_n_tmp - n_tmp;
      if (!(n >= 0.0)) {
        loop_ub = 0;
      } else {
        kfirst = (int)n;
        loop_ub = (int)n;
        if ((int)n >= 1) {
          ykfirst = pks_data[ti + 1];
          nPk = (int)n - 1;
          drift_data[(int)n - 1] = ykfirst;
          if ((int)n >= 2) {
            drift_data[0] = yk;
            if ((int)n >= 3) {
              if ((yk == -ykfirst) && ((int)n > 2)) {
                ykfirst /= (double)(int)n - 1.0;
                for (k = 2; k <= nPk; k++) {
                  drift_data[k - 1] =
                      ((double)((k << 1) - (int)n) - 1.0) * ykfirst;
                }
                if (((int)n & 1) == 1) {
                  drift_data[(int)n >> 1] = 0.0;
                }
              } else if (((yk < 0.0) != (ykfirst < 0.0)) &&
                         ((fabs(yk) > 8.9884656743115785E+307) ||
                          (fabs(ykfirst) > 8.9884656743115785E+307))) {
                delta1 = yk / ((double)(int)n - 1.0);
                ykfirst /= (double)(int)n - 1.0;
                for (k = 0; k <= kfirst - 3; k++) {
                  drift_data[k + 1] = (yk + ykfirst * ((double)k + 1.0)) -
                                      delta1 * ((double)k + 1.0);
                }
              } else {
                delta1 = (ykfirst - yk) / ((double)(int)n - 1.0);
                for (k = 0; k <= kfirst - 3; k++) {
                  drift_data[k + 1] = yk + ((double)k + 1.0) * delta1;
                }
              }
            }
          }
        }
      }
      ykfirst = drift_data[0];
      for (i = 0; i < loop_ub; i++) {
        b_drift_data[i] = drift_data[i] - ykfirst;
      }
      if (n_tmp + 1.0 > b_n_tmp) {
        i = 0;
        nPk = 0;
        nInf = 0;
        kfirst = -1;
      } else {
        i = (int)(((double)(c_iPk_data[ti] - 1) + 1.0) + 1.0) - 1;
        nPk = c_iPk_data[ti + 1];
        nInf = i;
        kfirst = nPk - 1;
      }
      if (nPk - i == loop_ub) {
        kfirst = (kfirst - nInf) + 1;
        for (nPk = 0; nPk < kfirst; nPk++) {
          hist_data[nPk] = hist[i + nPk] - b_drift_data[nPk];
        }
        for (i = 0; i < kfirst; i++) {
          hist[nInf + i] = hist_data[i];
        }
      } else {
        binary_expand_op(hist, nInf, i, nPk - 1, b_drift_data, &loop_ub, kfirst,
                         nInf - 1);
      }
      if (b_n_tmp + 1.0 > 2048.0) {
        i = 1;
        nPk = -1;
        nInf = -1;
      } else {
        i = (int)(((double)(c_iPk_data[ti + 1] - 1) + 1.0) + 1.0);
        nPk = i - 2;
        nInf = 2047;
      }
      kfirst = nInf - nPk;
      for (nInf = 0; nInf < kfirst; nInf++) {
        hist_data[nInf] = hist[(i + nInf) - 1] - b_drift_data[loop_ub - 1];
      }
      for (i = 0; i < kfirst; i++) {
        hist[(nPk + i) + 1] = hist_data[i];
      }
    }
  }
  /*  hist_out = wdenoise(hist);r\ */
  memcpy(&hist_out[0], &hist[0], 2048U * sizeof(double));
}

/* End of code generation (recalibrateHistory.c) */
