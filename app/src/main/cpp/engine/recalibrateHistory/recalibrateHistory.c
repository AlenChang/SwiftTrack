/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: recalibrateHistory.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 12-Dec-2022 19:58:02
 */

/* Include Files */
#include "recalibrateHistory.h"
#include "dtw.h"
#include "findpeaks.h"
#include "interp1.h"
#include "recalibrateHistory_data.h"
#include "recalibrateHistory_emxutil.h"
#include "recalibrateHistory_initialize.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Variable Definitions */
static double counter;

/* Function Declarations */
static void b_binary_expand_op(emxArray_real_T *in1, int in2, int in3, int in4,
                               int in5, const emxArray_real_T *in6);

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *in1
 *                int in2
 *                int in3
 *                int in4
 *                int in5
 *                const emxArray_real_T *in6
 * Return Type  : void
 */
static void b_binary_expand_op(emxArray_real_T *in1, int in2, int in3, int in4,
                               int in5, const emxArray_real_T *in6)
{
  emxArray_real_T *b_in1;
  const double *in6_data;
  double *b_in1_data;
  double *in1_data;
  int i;
  int stride_0_1;
  int stride_1_1;
  int unnamed_idx_1;
  in6_data = in6->data;
  in1_data = in1->data;
  unnamed_idx_1 = in3 - in2;
  emxInit_real_T(&b_in1, 2);
  i = b_in1->size[0] * b_in1->size[1];
  b_in1->size[0] = 1;
  b_in1->size[1] = unnamed_idx_1;
  emxEnsureCapacity_real_T(b_in1, i);
  b_in1_data = b_in1->data;
  stride_0_1 = ((in5 - in4) + 1 != 1);
  stride_1_1 = (in6->size[0] != 1);
  for (i = 0; i < unnamed_idx_1; i++) {
    b_in1_data[i] = in1_data[in4 + i * stride_0_1] - in6_data[i * stride_1_1];
  }
  unnamed_idx_1 = b_in1->size[1];
  for (i = 0; i < unnamed_idx_1; i++) {
    in1_data[in2 + i] = b_in1_data[i];
  }
  emxFree_real_T(&b_in1);
}

/*
 * Arguments    : emxArray_real_T *hist
 *                double moving_thre
 *                double last_tmp[100]
 *                double resp_waveform[100]
 *                emxArray_real_T *hist_out
 *                emxArray_boolean_T *is_body_moving
 *                double tmp[100]
 *                boolean_T *new_waveform
 *                double *resp_freq
 * Return Type  : void
 */
void recalibrateHistory(emxArray_real_T *hist, double moving_thre,
                        double last_tmp[100], double resp_waveform[100],
                        emxArray_real_T *hist_out,
                        emxArray_boolean_T *is_body_moving, double tmp[100],
                        boolean_T *new_waveform, double *resp_freq)
{
  emxArray_real_T *b_drift;
  emxArray_real_T *drift;
  emxArray_real_T *locs;
  emxArray_real_T *pks;
  double d1;
  double delta1;
  double delta2;
  double drift_tmp;
  double time_duration;
  double *b_drift_data;
  double *drift_data;
  double *hist_data;
  double *hist_out_data;
  double *locs_data;
  int b_last_tmp;
  int i;
  int i1;
  int i2;
  int idx;
  int k;
  int ti;
  boolean_T exitg1;
  boolean_T *is_body_moving_data;
  if (!isInitialized_recalibrateHistory) {
    recalibrateHistory_initialize();
  }
  hist_data = hist->data;
  emxInit_real_T(&pks, 1);
  emxInit_real_T(&locs, 1);
  findpeaks(hist, pks, locs);
  locs_data = locs->data;
  hist_out_data = pks->data;
  /*  [~, locs2] = findpeaks(-hist, ... */
  /*      'MinPeakDistance',100, 'MinPeakWidth', 50, 'MinPeakProminence', 0.05);
   */
  time_duration = 0.0;
  *resp_freq = 0.0;
  emxInit_real_T(&drift, 2);
  drift_data = drift->data;
  emxInit_real_T(&b_drift, 1);
  if (pks->size[0] >= 2) {
    i = pks->size[0];
    for (ti = 0; ti <= i - 2; ti++) {
      double n_tmp;
      d1 = hist_out_data[ti];
      n_tmp = locs_data[ti + 1];
      delta2 = n_tmp - locs_data[ti];
      if (!(delta2 >= 0.0)) {
        drift->size[0] = 1;
        drift->size[1] = 0;
      } else {
        i1 = drift->size[0] * drift->size[1];
        drift->size[0] = 1;
        drift->size[1] = (int)delta2;
        emxEnsureCapacity_real_T(drift, i1);
        drift_data = drift->data;
        if ((int)delta2 >= 1) {
          drift_tmp = hist_out_data[ti + 1];
          drift_data[(int)delta2 - 1] = drift_tmp;
          if (drift->size[1] >= 2) {
            drift_data[0] = hist_out_data[ti];
            if (drift->size[1] >= 3) {
              if (hist_out_data[ti] == -drift_tmp) {
                delta2 = drift_tmp / ((double)drift->size[1] - 1.0);
                i1 = drift->size[1] - 1;
                for (k = 2; k <= i1; k++) {
                  drift_data[k - 1] =
                      (double)(((k << 1) - drift->size[1]) - 1) * delta2;
                }
                if ((drift->size[1] & 1) == 1) {
                  drift_data[drift->size[1] >> 1] = 0.0;
                }
              } else if (((hist_out_data[ti] < 0.0) != (drift_tmp < 0.0)) &&
                         ((fabs(hist_out_data[ti]) > 8.9884656743115785E+307) ||
                          (fabs(drift_tmp) > 8.9884656743115785E+307))) {
                delta1 = hist_out_data[ti] / ((double)drift->size[1] - 1.0);
                delta2 = drift_tmp / ((double)drift->size[1] - 1.0);
                i1 = drift->size[1];
                for (k = 0; k <= i1 - 3; k++) {
                  drift_data[k + 1] = (d1 + delta2 * ((double)k + 1.0)) -
                                      delta1 * ((double)k + 1.0);
                }
              } else {
                delta1 = (drift_tmp - hist_out_data[ti]) /
                         ((double)drift->size[1] - 1.0);
                i1 = drift->size[1];
                for (k = 0; k <= i1 - 3; k++) {
                  drift_data[k + 1] = d1 + ((double)k + 1.0) * delta1;
                }
              }
            }
          }
        }
      }
      delta2 = drift_data[0];
      i1 = b_drift->size[0];
      b_drift->size[0] = drift->size[1];
      emxEnsureCapacity_real_T(b_drift, i1);
      b_drift_data = b_drift->data;
      idx = drift->size[1];
      for (i1 = 0; i1 < idx; i1++) {
        b_drift_data[i1] = drift_data[i1] - delta2;
      }
      if (locs_data[ti] + 1.0 > n_tmp) {
        i1 = 0;
        i2 = 0;
        b_last_tmp = 0;
        idx = 0;
      } else {
        i1 = (int)(locs_data[ti] + 1.0) - 1;
        i2 = (int)n_tmp;
        b_last_tmp = i1;
        idx = (int)locs_data[ti + 1];
      }
      if (i2 - i1 == b_drift->size[0]) {
        idx -= b_last_tmp;
        i2 = drift->size[0] * drift->size[1];
        drift->size[0] = 1;
        drift->size[1] = idx;
        emxEnsureCapacity_real_T(drift, i2);
        drift_data = drift->data;
        for (i2 = 0; i2 < idx; i2++) {
          drift_data[i2] = hist_data[i1 + i2] - b_drift_data[i2];
        }
        idx = drift->size[1];
        for (i1 = 0; i1 < idx; i1++) {
          hist_data[b_last_tmp + i1] = drift_data[i1];
        }
      } else {
        b_binary_expand_op(hist, b_last_tmp, idx, i1, i2 - 1, b_drift);
        hist_data = hist->data;
      }
      if (n_tmp + 1.0 > hist->size[0]) {
        i1 = 1;
      } else {
        i1 = (int)(n_tmp + 1.0);
      }
      if (n_tmp + 1.0 > hist->size[0]) {
        i2 = 0;
        b_last_tmp = 0;
      } else {
        i2 = (int)(locs_data[ti + 1] + 1.0) - 1;
        b_last_tmp = hist->size[0];
      }
      idx = b_last_tmp - i2;
      b_last_tmp = drift->size[0] * drift->size[1];
      drift->size[0] = 1;
      drift->size[1] = idx;
      emxEnsureCapacity_real_T(drift, b_last_tmp);
      drift_data = drift->data;
      for (b_last_tmp = 0; b_last_tmp < idx; b_last_tmp++) {
        drift_data[b_last_tmp] = hist_data[(i1 + b_last_tmp) - 1] -
                                 b_drift_data[b_drift->size[0] - 1];
      }
      idx = drift->size[1];
      for (i1 = 0; i1 < idx; i1++) {
        hist_data[i2 + i1] = drift_data[i1];
      }
    }
  }
  /*  hist_out = wdenoise(hist); */
  i = hist_out->size[0];
  hist_out->size[0] = hist->size[0];
  emxEnsureCapacity_real_T(hist_out, i);
  hist_out_data = hist_out->data;
  idx = hist->size[0];
  for (i = 0; i < idx; i++) {
    hist_out_data[i] = hist_data[i];
  }
  i = is_body_moving->size[0];
  is_body_moving->size[0] = hist->size[0];
  emxEnsureCapacity_boolean_T(is_body_moving, i);
  is_body_moving_data = is_body_moving->data;
  idx = hist->size[0];
  for (i = 0; i < idx; i++) {
    is_body_moving_data[i] = false;
  }
  if (pks->size[0] >= 2) {
    i = pks->size[0];
    for (ti = 0; ti <= i - 2; ti++) {
      delta2 = locs_data[ti + 1];
      if (locs_data[ti] + 1.0 > delta2) {
        i1 = 0;
        i2 = 0;
      } else {
        i1 = (int)(locs_data[ti] + 1.0) - 1;
        i2 = (int)delta2;
      }
      b_last_tmp = i2 - i1;
      if (b_last_tmp <= 2) {
        if (b_last_tmp == 1) {
          drift_tmp = hist_data[i1];
          d1 = hist_data[i1];
        } else {
          drift_tmp = hist_data[i2 - 1];
          if ((!(hist_data[i1] < drift_tmp)) &&
              ((!rtIsNaN(hist_data[i1])) || rtIsNaN(drift_tmp))) {
            drift_tmp = hist_data[i1];
          }
          d1 = hist_data[i2 - 1];
          if ((!(hist_data[i1] > d1)) &&
              ((!rtIsNaN(hist_data[i1])) || rtIsNaN(hist_data[i2 - 1]))) {
            d1 = hist_data[i1];
          }
        }
      } else {
        if (!rtIsNaN(hist_data[i1])) {
          idx = 1;
        } else {
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= b_last_tmp)) {
            if (!rtIsNaN(hist_data[(i1 + k) - 1])) {
              idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }
        if (idx == 0) {
          drift_tmp = hist_data[i1];
        } else {
          drift_tmp = hist_data[(i1 + idx) - 1];
          i2 = idx + 1;
          for (k = i2; k <= b_last_tmp; k++) {
            delta1 = hist_data[(i1 + k) - 1];
            if (drift_tmp < delta1) {
              drift_tmp = delta1;
            }
          }
        }
        if (!rtIsNaN(hist_data[i1])) {
          idx = 1;
        } else {
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= b_last_tmp)) {
            if (!rtIsNaN(hist_data[(i1 + k) - 1])) {
              idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }
        if (idx == 0) {
          d1 = hist_data[i1];
        } else {
          d1 = hist_data[(i1 + idx) - 1];
          i2 = idx + 1;
          for (k = i2; k <= b_last_tmp; k++) {
            delta1 = hist_data[(i1 + k) - 1];
            if (d1 > delta1) {
              d1 = delta1;
            }
          }
        }
      }
      if (drift_tmp - d1 > moving_thre) {
        if (locs_data[ti] + 1.0 > delta2) {
          i1 = 0;
          i2 = 0;
        } else {
          i1 = (int)(locs_data[ti] + 1.0) - 1;
          i2 = (int)locs_data[ti + 1];
        }
        idx = i2 - i1;
        for (i2 = 0; i2 < idx; i2++) {
          is_body_moving_data[i1 + i2] = true;
        }
      } else {
        time_duration = (time_duration + delta2) - locs_data[ti];
        (*resp_freq)++;
      }
    }
  }
  emxFree_real_T(&pks);
  time_duration = time_duration * 960.0 / 48000.0;
  *resp_freq = *resp_freq / time_duration * 60.0;
  *new_waveform = false;
  if (locs->size[0] > 2) {
    if (locs_data[locs->size[0] - 2] > locs_data[locs->size[0] - 1]) {
      i = 0;
      i1 = 0;
    } else {
      i = (int)locs_data[locs->size[0] - 2] - 1;
      i1 = (int)locs_data[locs->size[0] - 1];
    }
    i2 = drift->size[0] * drift->size[1];
    drift->size[0] = 1;
    idx = i1 - i;
    drift->size[1] = idx;
    emxEnsureCapacity_real_T(drift, i2);
    drift_data = drift->data;
    if (idx >= 1) {
      drift_data[idx - 1] = 1.0;
      if (drift->size[1] >= 2) {
        drift_data[0] = 0.0;
        if (drift->size[1] >= 3) {
          delta1 = 1.0 / ((double)drift->size[1] - 1.0);
          i1 = drift->size[1];
          for (k = 0; k <= i1 - 3; k++) {
            drift_data[k + 1] = ((double)k + 1.0) * delta1;
          }
        }
      }
    }
    i1 = b_drift->size[0];
    b_drift->size[0] = idx;
    emxEnsureCapacity_real_T(b_drift, i1);
    b_drift_data = b_drift->data;
    for (i1 = 0; i1 < idx; i1++) {
      b_drift_data[i1] = hist_data[i + i1];
    }
    interp1(drift, b_drift, tmp);
    if (!rtIsNaN(tmp[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k < 101)) {
        if (!rtIsNaN(tmp[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (idx == 0) {
      drift_tmp = tmp[0];
    } else {
      drift_tmp = tmp[idx - 1];
      i = idx + 1;
      for (k = i; k < 101; k++) {
        delta2 = tmp[k - 1];
        if (drift_tmp > delta2) {
          drift_tmp = delta2;
        }
      }
    }
    for (i = 0; i < 100; i++) {
      tmp[i] -= drift_tmp;
    }
    if (!rtIsNaN(tmp[0])) {
      idx = 1;
    } else {
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k < 101)) {
        if (!rtIsNaN(tmp[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (idx == 0) {
      drift_tmp = tmp[0];
    } else {
      drift_tmp = tmp[idx - 1];
      i = idx + 1;
      for (k = i; k < 101; k++) {
        delta2 = tmp[k - 1];
        if (drift_tmp < delta2) {
          drift_tmp = delta2;
        }
      }
    }
    for (i = 0; i < 100; i++) {
      tmp[i] /= drift_tmp;
    }
    if (dtw(last_tmp, tmp) > 0.1) {
      for (i = 0; i < 100; i++) {
        delta2 = tmp[i];
        last_tmp[i] = delta2;
        resp_waveform[i] =
            (delta2 + resp_waveform[i] * counter) / (counter + 1.0);
      }
      counter++;
      /*          resp_waveform = normalize_waveform(resp_waveform); */
      *new_waveform = true;
    }
  } else {
    memset(&tmp[0], 0, 100U * sizeof(double));
  }
  emxFree_real_T(&b_drift);
  emxFree_real_T(&locs);
  emxFree_real_T(&drift);
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
