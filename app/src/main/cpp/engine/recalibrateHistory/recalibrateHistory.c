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
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

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

void recalibrateHistory(double hist[2048], double moving_thre,
                        double hist_out[2048], boolean_T is_body_moving[2048],
                        double pks_data[], int pks_size[1], double locs_data[],
                        int locs_size[1])
{
  static double hist_data[2048];
  static double b_drift_data[2047];
  static double drift_data[2047];
  double d1;
  double delta1;
  double delta2;
  double n;
  double n_tmp;
  int b_i;
  int drift_size_idx_1;
  int drift_tmp_tmp;
  int i;
  int i1;
  int i2;
  int k;
  int ti;
  boolean_T b;
  boolean_T exitg1;
  findpeaks(hist, pks_data, &pks_size[0], locs_data, &locs_size[0]);
  if (pks_size[0] >= 2) {
    i = pks_size[0];
    for (ti = 0; ti <= i - 2; ti++) {
      d1 = pks_data[ti];
      n_tmp = locs_data[ti + 1];
      n = n_tmp - locs_data[ti];
      if (!(n >= 0.0)) {
        drift_size_idx_1 = 0;
      } else {
        b_i = (int)n;
        drift_size_idx_1 = (int)n;
        if ((int)n >= 1) {
          delta2 = pks_data[ti + 1];
          drift_tmp_tmp = (int)n - 1;
          drift_data[(int)n - 1] = delta2;
          if ((int)n >= 2) {
            drift_data[0] = d1;
            if ((int)n >= 3) {
              if ((d1 == -delta2) && ((int)n > 2)) {
                delta2 /= (double)(int)n - 1.0;
                for (k = 2; k <= drift_tmp_tmp; k++) {
                  drift_data[k - 1] =
                      ((double)((k << 1) - (int)n) - 1.0) * delta2;
                }
                if (((int)n & 1) == 1) {
                  drift_data[(int)n >> 1] = 0.0;
                }
              } else if (((d1 < 0.0) != (delta2 < 0.0)) &&
                         ((fabs(d1) > 8.9884656743115785E+307) ||
                          (fabs(delta2) > 8.9884656743115785E+307))) {
                delta1 = d1 / ((double)(int)n - 1.0);
                delta2 /= (double)(int)n - 1.0;
                for (k = 0; k <= b_i - 3; k++) {
                  drift_data[k + 1] = (d1 + delta2 * ((double)k + 1.0)) -
                                      delta1 * ((double)k + 1.0);
                }
              } else {
                delta1 = (delta2 - d1) / ((double)(int)n - 1.0);
                for (k = 0; k <= b_i - 3; k++) {
                  drift_data[k + 1] = d1 + ((double)k + 1.0) * delta1;
                }
              }
            }
          }
        }
      }
      delta2 = drift_data[0];
      for (i1 = 0; i1 < drift_size_idx_1; i1++) {
        b_drift_data[i1] = drift_data[i1] - delta2;
      }
      delta1 = locs_data[ti];
      if (delta1 + 1.0 > n_tmp) {
        i1 = 0;
        i2 = 0;
        drift_tmp_tmp = 0;
        b_i = -1;
      } else {
        i1 = (int)(delta1 + 1.0) - 1;
        i2 = (int)n_tmp;
        drift_tmp_tmp = (int)(delta1 + 1.0) - 1;
        b_i = (int)n_tmp - 1;
      }
      if (i2 - i1 == drift_size_idx_1) {
        b_i = (b_i - drift_tmp_tmp) + 1;
        for (i2 = 0; i2 < b_i; i2++) {
          hist_data[i2] = hist[i1 + i2] - b_drift_data[i2];
        }
        for (i1 = 0; i1 < b_i; i1++) {
          hist[drift_tmp_tmp + i1] = hist_data[i1];
        }
      } else {
        binary_expand_op(hist, drift_tmp_tmp, i1, i2 - 1, b_drift_data,
                         &drift_size_idx_1, b_i, drift_tmp_tmp - 1);
      }
      if (n_tmp + 1.0 > 2048.0) {
        i1 = 1;
        i2 = -1;
        drift_tmp_tmp = -1;
      } else {
        i1 = (int)(locs_data[ti + 1] + 1.0);
        i2 = i1 - 2;
        drift_tmp_tmp = 2047;
      }
      b_i = drift_tmp_tmp - i2;
      for (drift_tmp_tmp = 0; drift_tmp_tmp < b_i; drift_tmp_tmp++) {
        hist_data[drift_tmp_tmp] =
            hist[(i1 + drift_tmp_tmp) - 1] - b_drift_data[drift_size_idx_1 - 1];
      }
      for (i1 = 0; i1 < b_i; i1++) {
        hist[(i2 + i1) + 1] = hist_data[i1];
      }
    }
  }
  /*  hist_out = wdenoise(hist); */
  for (b_i = 0; b_i < 2048; b_i++) {
    hist_out[b_i] = hist[b_i];
    is_body_moving[b_i] = false;
  }
  if (pks_size[0] >= 2) {
    i = pks_size[0];
    for (ti = 0; ti <= i - 2; ti++) {
      delta1 = locs_data[ti + 1];
      delta2 = locs_data[ti];
      if (delta2 + 1.0 > delta1) {
        i1 = 0;
        i2 = 0;
      } else {
        i1 = (int)(delta2 + 1.0) - 1;
        i2 = (int)delta1;
      }
      drift_tmp_tmp = i2 - i1;
      if (drift_tmp_tmp <= 2) {
        if (drift_tmp_tmp == 1) {
          d1 = hist[i1];
          n = hist[i1];
        } else {
          n = hist[i2 - 1];
          if ((hist[i1] < n) || (rtIsNaN(hist[i1]) && (!rtIsNaN(n)))) {
            d1 = n;
          } else {
            d1 = hist[i1];
          }
          if ((!(hist[i1] > n)) &&
              ((!rtIsNaN(hist[i1])) || rtIsNaN(hist[i2 - 1]))) {
            n = hist[i1];
          }
        }
      } else {
        b = rtIsNaN(hist[i1]);
        if (!b) {
          b_i = 1;
        } else {
          b_i = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= drift_tmp_tmp)) {
            if (!rtIsNaN(hist[(i1 + k) - 1])) {
              b_i = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }
        if (b_i == 0) {
          d1 = hist[i1];
        } else {
          d1 = hist[(i1 + b_i) - 1];
          i2 = b_i + 1;
          for (k = i2; k <= drift_tmp_tmp; k++) {
            delta2 = hist[(i1 + k) - 1];
            if (d1 < delta2) {
              d1 = delta2;
            }
          }
        }
        if (!b) {
          b_i = 1;
        } else {
          b_i = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= drift_tmp_tmp)) {
            if (!rtIsNaN(hist[(i1 + k) - 1])) {
              b_i = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }
        if (b_i == 0) {
          n = hist[i1];
        } else {
          n = hist[(i1 + b_i) - 1];
          i2 = b_i + 1;
          for (k = i2; k <= drift_tmp_tmp; k++) {
            delta2 = hist[(i1 + k) - 1];
            if (n > delta2) {
              n = delta2;
            }
          }
        }
      }
      if (d1 - n > moving_thre) {
        delta2 = locs_data[ti];
        if (delta2 + 1.0 > delta1) {
          i1 = -1;
          i2 = 0;
        } else {
          i1 = (int)(delta2 + 1.0) - 2;
          i2 = (int)locs_data[ti + 1];
        }
        b_i = (i2 - i1) - 1;
        for (i2 = 0; i2 < b_i; i2++) {
          is_body_moving[(i1 + i2) + 1] = true;
        }
      }
    }
  }
}

/* End of code generation (recalibrateHistory.c) */
