/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: recalibrateHistory.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 25-Nov-2022 15:54:38
 */

/* Include Files */
#include "recalibrateHistory.h"
#include "dtw.h"
#include "findpeaks.h"
#include "interp1.h"
#include "recalibrateHistory_data.h"
#include "recalibrateHistory_initialize.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Variable Definitions */
static double counter;

/* Function Declarations */
static void b_binary_expand_op(double in1[2048], int in2, int in3, int in4,
                               int in5, const double in6_data[],
                               const int *in6_size);

/* Function Definitions */
/*
 * Arguments    : double in1[2048]
 *                int in2
 *                int in3
 *                int in4
 *                int in5
 *                const double in6_data[]
 *                const int *in6_size
 * Return Type  : void
 */
static void b_binary_expand_op(double in1[2048], int in2, int in3, int in4,
                               int in5, const double in6_data[],
                               const int *in6_size)
{
  double in1_data[2048];
  int i;
  int stride_0_1;
  int stride_1_1;
  int unnamed_idx_1;
  unnamed_idx_1 = in3 - in2;
  stride_0_1 = ((in5 - in4) + 1 != 1);
  stride_1_1 = (*in6_size != 1);
  for (i = 0; i < unnamed_idx_1; i++) {
    in1_data[i] = in1[in4 + i * stride_0_1] - in6_data[i * stride_1_1];
  }
  for (i = 0; i < unnamed_idx_1; i++) {
    in1[in2 + i] = in1_data[i];
  }
}

/*
 * Arguments    : double hist[2048]
 *                double moving_thre
 *                double last_tmp[100]
 *                double resp_waveform[100]
 *                double hist_out[2048]
 *                boolean_T is_body_moving[2048]
 *                double tmp[100]
 *                boolean_T *new_waveform
 *                double *resp_freq
 * Return Type  : void
 */
void recalibrateHistory(double hist[2048], double moving_thre,
                        double last_tmp[100], double resp_waveform[100],
                        double hist_out[2048], boolean_T is_body_moving[2048],
                        double tmp[100], boolean_T *new_waveform,
                        double *resp_freq)
{
  static double locs_data[2048];
  static double pks_data[2048];
  static double t2_data[2048];
  static double b_drift_data[2047];
  static double drift_data[2047];
  double d1;
  double delta1;
  double delta2;
  double n;
  double time_duration;
  int t2_size[2];
  int b_i;
  int drift_size_idx_1;
  int i;
  int i1;
  int k;
  int locs_size;
  int pks_size;
  int ti;
  boolean_T exitg1;
  if (!isInitialized_recalibrateHistory) {
    recalibrateHistory_initialize();
  }
  findpeaks(hist, pks_data, &pks_size, locs_data, &locs_size);
  /*  [~, locs2] = findpeaks(-hist, ... */
  /*      'MinPeakDistance',100, 'MinPeakWidth', 50, 'MinPeakProminence', 0.05);
   */
  time_duration = 0.0;
  *resp_freq = 0.0;
  if (pks_size >= 2) {
    for (ti = 0; ti <= pks_size - 2; ti++) {
      double n_tmp;
      d1 = pks_data[ti];
      n_tmp = locs_data[ti + 1];
      n = n_tmp - locs_data[ti];
      if (!(n >= 0.0)) {
        drift_size_idx_1 = 0;
      } else {
        i = (int)n;
        drift_size_idx_1 = (int)n;
        if ((int)n >= 1) {
          delta2 = pks_data[ti + 1];
          drift_data[(int)n - 1] = delta2;
          if ((int)n >= 2) {
            drift_data[0] = d1;
            if ((int)n >= 3) {
              if (d1 == -delta2) {
                delta2 /= (double)(int)n - 1.0;
                b_i = (int)n - 1;
                for (k = 2; k <= b_i; k++) {
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
                for (k = 0; k <= i - 3; k++) {
                  drift_data[k + 1] = (d1 + delta2 * ((double)k + 1.0)) -
                                      delta1 * ((double)k + 1.0);
                }
              } else {
                delta1 = (delta2 - d1) / ((double)(int)n - 1.0);
                for (k = 0; k <= i - 3; k++) {
                  drift_data[k + 1] = d1 + ((double)k + 1.0) * delta1;
                }
              }
            }
          }
        }
      }
      delta2 = drift_data[0];
      for (b_i = 0; b_i < drift_size_idx_1; b_i++) {
        b_drift_data[b_i] = drift_data[b_i] - delta2;
      }
      delta1 = locs_data[ti];
      if (delta1 + 1.0 > n_tmp) {
        b_i = 0;
        i1 = 0;
        k = 0;
        i = 0;
      } else {
        b_i = (int)(delta1 + 1.0) - 1;
        i1 = (int)n_tmp;
        k = (int)(delta1 + 1.0) - 1;
        i = (int)locs_data[ti + 1];
      }
      if (i1 - b_i == drift_size_idx_1) {
        i -= k;
        for (i1 = 0; i1 < i; i1++) {
          t2_data[i1] = hist[b_i + i1] - b_drift_data[i1];
        }
        for (b_i = 0; b_i < i; b_i++) {
          hist[k + b_i] = t2_data[b_i];
        }
      } else {
        b_binary_expand_op(hist, k, i, b_i, i1 - 1, b_drift_data,
                           &drift_size_idx_1);
      }
      if (n_tmp + 1.0 > 2048.0) {
        b_i = 1;
        i1 = -1;
        k = -1;
      } else {
        b_i = (int)(n_tmp + 1.0);
        i1 = (int)(n_tmp + 1.0) - 2;
        k = 2047;
      }
      i = k - i1;
      for (k = 0; k < i; k++) {
        t2_data[k] = hist[(b_i + k) - 1] - b_drift_data[drift_size_idx_1 - 1];
      }
      for (b_i = 0; b_i < i; b_i++) {
        hist[(i1 + b_i) + 1] = t2_data[b_i];
      }
    }
  }
  /*  hist_out = wdenoise(hist); */
  for (i = 0; i < 2048; i++) {
    hist_out[i] = hist[i];
    is_body_moving[i] = false;
  }
  if (pks_size >= 2) {
    for (ti = 0; ti <= pks_size - 2; ti++) {
      delta1 = locs_data[ti + 1];
      delta2 = locs_data[ti];
      if (delta2 + 1.0 > delta1) {
        b_i = 0;
        i1 = 0;
      } else {
        b_i = (int)(delta2 + 1.0) - 1;
        i1 = (int)delta1;
      }
      drift_size_idx_1 = i1 - b_i;
      if (drift_size_idx_1 <= 2) {
        if (drift_size_idx_1 == 1) {
          d1 = hist[b_i];
          n = hist[b_i];
        } else {
          n = hist[i1 - 1];
          if ((hist[b_i] < n) || (rtIsNaN(hist[b_i]) && (!rtIsNaN(n)))) {
            d1 = n;
          } else {
            d1 = hist[b_i];
          }
          if ((!(hist[b_i] > n)) &&
              ((!rtIsNaN(hist[b_i])) || rtIsNaN(hist[i1 - 1]))) {
            n = hist[b_i];
          }
        }
      } else {
        boolean_T b;
        b = rtIsNaN(hist[b_i]);
        if (!b) {
          i = 1;
        } else {
          i = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= drift_size_idx_1)) {
            if (!rtIsNaN(hist[(b_i + k) - 1])) {
              i = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }
        if (i == 0) {
          d1 = hist[b_i];
        } else {
          d1 = hist[(b_i + i) - 1];
          i1 = i + 1;
          for (k = i1; k <= drift_size_idx_1; k++) {
            delta2 = hist[(b_i + k) - 1];
            if (d1 < delta2) {
              d1 = delta2;
            }
          }
        }
        if (!b) {
          i = 1;
        } else {
          i = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= drift_size_idx_1)) {
            if (!rtIsNaN(hist[(b_i + k) - 1])) {
              i = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }
        if (i == 0) {
          n = hist[b_i];
        } else {
          n = hist[(b_i + i) - 1];
          i1 = i + 1;
          for (k = i1; k <= drift_size_idx_1; k++) {
            delta2 = hist[(b_i + k) - 1];
            if (n > delta2) {
              n = delta2;
            }
          }
        }
      }
      if (d1 - n > moving_thre) {
        delta2 = locs_data[ti];
        if (delta2 + 1.0 > delta1) {
          b_i = 0;
          i1 = 0;
        } else {
          b_i = (int)(delta2 + 1.0) - 1;
          i1 = (int)locs_data[ti + 1];
        }
        i = i1 - b_i;
        for (i1 = 0; i1 < i; i1++) {
          is_body_moving[b_i + i1] = true;
        }
      } else {
        time_duration = (time_duration + delta1) - locs_data[ti];
        (*resp_freq)++;
      }
    }
  }
  time_duration = time_duration * 960.0 / 48000.0;
  *resp_freq = *resp_freq / time_duration * 60.0;
  *new_waveform = false;
  if (locs_size > 2) {
    delta1 = locs_data[locs_size - 2];
    delta2 = locs_data[locs_size - 1];
    if (delta1 > delta2) {
      b_i = 0;
      i1 = 0;
    } else {
      b_i = (int)delta1 - 1;
      i1 = (int)delta2;
    }
    t2_size[0] = 1;
    i = i1 - b_i;
    t2_size[1] = i;
    if (i >= 1) {
      t2_data[i - 1] = 1.0;
      if (i >= 2) {
        t2_data[0] = 0.0;
        if (i >= 3) {
          delta1 = 1.0 / ((double)i - 1.0);
          for (k = 0; k <= i - 3; k++) {
            t2_data[k + 1] = ((double)k + 1.0) * delta1;
          }
        }
      }
    }
    for (i1 = 0; i1 < i; i1++) {
      pks_data[i1] = hist[b_i + i1];
    }
    interp1(t2_data, t2_size, pks_data, i, tmp);
    if (!rtIsNaN(tmp[0])) {
      i = 1;
    } else {
      i = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k < 101)) {
        if (!rtIsNaN(tmp[k - 1])) {
          i = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (i == 0) {
      d1 = tmp[0];
    } else {
      d1 = tmp[i - 1];
      b_i = i + 1;
      for (k = b_i; k < 101; k++) {
        delta1 = tmp[k - 1];
        if (d1 > delta1) {
          d1 = delta1;
        }
      }
    }
    for (b_i = 0; b_i < 100; b_i++) {
      tmp[b_i] -= d1;
    }
    if (!rtIsNaN(tmp[0])) {
      i = 1;
    } else {
      i = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k < 101)) {
        if (!rtIsNaN(tmp[k - 1])) {
          i = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (i == 0) {
      d1 = tmp[0];
    } else {
      d1 = tmp[i - 1];
      b_i = i + 1;
      for (k = b_i; k < 101; k++) {
        delta1 = tmp[k - 1];
        if (d1 < delta1) {
          d1 = delta1;
        }
      }
    }
    for (b_i = 0; b_i < 100; b_i++) {
      tmp[b_i] /= d1;
    }
    if (dtw(last_tmp, tmp) > 0.1) {
      for (b_i = 0; b_i < 100; b_i++) {
        delta1 = tmp[b_i];
        last_tmp[b_i] = delta1;
        resp_waveform[b_i] =
            (delta1 + resp_waveform[b_i] * counter) / (counter + 1.0);
      }
      counter++;
      /*          resp_waveform = normalize_waveform(resp_waveform); */
      *new_waveform = true;
    }
  } else {
    memset(&tmp[0], 0, 100U * sizeof(double));
  }
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void recalibrateHistory_init(void)
{
  counter = 0.0;
}

/*
 * File trailer for recalibrateHistory.c
 *
 * [EOF]
 */
