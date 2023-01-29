/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: recalibrateHistory.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 29-Jan-2023 18:06:51
 */

/* Include Files */
#include "recalibrateHistory.h"
#include "dtw.h"
#include "filter.h"
#include "findpeaks.h"
#include "interp1.h"
#include "minOrMax.h"
#include "prctile.h"
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
 * Arguments    : const emxArray_real_T *hist
 *                double moving_thre
 *                double last_tmp[100]
 *                double resp_waveform[100]
 *                boolean_T need_bandpass
 *                emxArray_real_T *hist_out
 *                emxArray_boolean_T *is_body_moving
 *                double tmp[100]
 *                boolean_T *new_waveform
 *                double *resp_freq
 * Return Type  : void
 */
void recalibrateHistory(const emxArray_real_T *hist, double moving_thre,
                        double last_tmp[100], double resp_waveform[100],
                        boolean_T need_bandpass, emxArray_real_T *hist_out,
                        emxArray_boolean_T *is_body_moving, double tmp[100],
                        boolean_T *new_waveform, double *resp_freq)
{
  emxArray_real_T *b_drift;
  emxArray_real_T *b_hist2;
  emxArray_real_T *drift;
  emxArray_real_T *hist2;
  emxArray_real_T *locs;
  double amax;
  double amin;
  double d1_tmp;
  double delta1;
  double time_duration;
  double *b_drift_data;
  double *drift_data;
  double *hist2_data;
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
  (void)need_bandpass;
  if (!isInitialized_recalibrateHistory) {
    recalibrateHistory_initialize();
  }
  /*  hist = bandpass(hist, [0.2, 1.5], 50, 'StopbandAttenuation', 30,
   * 'Steepness', 0.6); */
  /*  if(need_bandpass) */
  /*      filter_b = getfiltercoeffs; */
  /*      hist = filter(filter_b, 1, hist); */
  /*  end */
  /*  hist_out = wdenoise(hist); */
  emxInit_real_T(&hist2, 1);
  filter(hist, hist2);
  hist2_data = hist2->data;
  if (hist2->size[0] < 286) {
    i = 0;
    i1 = 0;
  } else {
    i = 285;
    i1 = hist2->size[0];
  }
  idx = i1 - i;
  for (i1 = 0; i1 < idx; i1++) {
    hist2_data[i1] = hist2_data[i + i1];
  }
  i = hist2->size[0];
  hist2->size[0] = idx;
  emxEnsureCapacity_real_T(hist2, i);
  hist2_data = hist2->data;
  /*   */
  /*  filter_b = getfiltercoeffs; */
  /*  b = zeros(size(hist_out)); */
  /*  assert(length(hist_out) > length(filter_b)); */
  /*  b(1:length(filter_b)) = filter_b; */
  /*   */
  /*  hist = real(ifft(fft(hist_out) .* fft(b))); */
  amin = prctile(hist2);
  amax = b_prctile(hist2) - amin;
  emxInit_real_T(&b_hist2, 1);
  i = b_hist2->size[0];
  b_hist2->size[0] = idx;
  emxEnsureCapacity_real_T(b_hist2, i);
  drift_data = b_hist2->data;
  for (i = 0; i < idx; i++) {
    drift_data[i] = (hist2_data[i] - amin) / amax;
  }
  emxInit_real_T(&locs, 1);
  emxInit_real_T(&drift, 1);
  findpeaks(b_hist2, drift, locs);
  locs_data = locs->data;
  emxFree_real_T(&b_hist2);
  /*  [~, locs2] = findpeaks(-hist, ... */
  /*      'MinPeakDistance',100, 'MinPeakWidth', 50, 'MinPeakProminence', 0.05);
   */
  time_duration = 0.0;
  *resp_freq = 0.0;
  emxInit_real_T(&b_drift, 2);
  drift_data = b_drift->data;
  if (locs->size[0] >= 2) {
    i = locs->size[0];
    for (ti = 0; ti <= i - 2; ti++) {
      double n_tmp;
      d1_tmp = hist2_data[(int)locs_data[ti] - 1];
      n_tmp = locs_data[ti + 1];
      amin = n_tmp - locs_data[ti];
      if (!(amin >= 0.0)) {
        b_drift->size[0] = 1;
        b_drift->size[1] = 0;
      } else {
        i1 = b_drift->size[0] * b_drift->size[1];
        b_drift->size[0] = 1;
        b_drift->size[1] = (int)amin;
        emxEnsureCapacity_real_T(b_drift, i1);
        drift_data = b_drift->data;
        if ((int)amin >= 1) {
          amax = hist2_data[(int)n_tmp - 1];
          drift_data[(int)amin - 1] = amax;
          if (b_drift->size[1] >= 2) {
            drift_data[0] = d1_tmp;
            if (b_drift->size[1] >= 3) {
              if (d1_tmp == -amax) {
                amin = amax / ((double)b_drift->size[1] - 1.0);
                i1 = b_drift->size[1] - 1;
                for (k = 2; k <= i1; k++) {
                  drift_data[k - 1] =
                      (double)(((k << 1) - b_drift->size[1]) - 1) * amin;
                }
                if ((b_drift->size[1] & 1) == 1) {
                  drift_data[b_drift->size[1] >> 1] = 0.0;
                }
              } else if (((d1_tmp < 0.0) != (amax < 0.0)) &&
                         ((fabs(d1_tmp) > 8.9884656743115785E+307) ||
                          (fabs(amax) > 8.9884656743115785E+307))) {
                delta1 = d1_tmp / ((double)b_drift->size[1] - 1.0);
                amin = amax / ((double)b_drift->size[1] - 1.0);
                i1 = b_drift->size[1];
                for (k = 0; k <= i1 - 3; k++) {
                  drift_data[k + 1] = (d1_tmp + amin * ((double)k + 1.0)) -
                                      delta1 * ((double)k + 1.0);
                }
              } else {
                delta1 = (amax - d1_tmp) / ((double)b_drift->size[1] - 1.0);
                i1 = b_drift->size[1];
                for (k = 0; k <= i1 - 3; k++) {
                  drift_data[k + 1] = d1_tmp + ((double)k + 1.0) * delta1;
                }
              }
            }
          }
        }
      }
      amin = drift_data[0];
      i1 = drift->size[0];
      drift->size[0] = b_drift->size[1];
      emxEnsureCapacity_real_T(drift, i1);
      b_drift_data = drift->data;
      idx = b_drift->size[1];
      for (i1 = 0; i1 < idx; i1++) {
        b_drift_data[i1] = drift_data[i1] - amin;
      }
      if (locs_data[ti] + 1.0 > n_tmp) {
        i1 = 0;
        i2 = 0;
        b_last_tmp = 0;
        idx = 0;
      } else {
        i1 = (int)(locs_data[ti] + 1.0) - 1;
        i2 = (int)locs_data[ti + 1];
        b_last_tmp = i1;
        idx = (int)locs_data[ti + 1];
      }
      if (i2 - i1 == drift->size[0]) {
        idx -= b_last_tmp;
        i2 = b_drift->size[0] * b_drift->size[1];
        b_drift->size[0] = 1;
        b_drift->size[1] = idx;
        emxEnsureCapacity_real_T(b_drift, i2);
        drift_data = b_drift->data;
        for (i2 = 0; i2 < idx; i2++) {
          drift_data[i2] = hist2_data[i1 + i2] - b_drift_data[i2];
        }
        idx = b_drift->size[1];
        for (i1 = 0; i1 < idx; i1++) {
          hist2_data[b_last_tmp + i1] = drift_data[i1];
        }
      } else {
        b_binary_expand_op(hist2, b_last_tmp, idx, i1, i2 - 1, drift);
        hist2_data = hist2->data;
      }
      if (n_tmp + 1.0 > hist2->size[0]) {
        i1 = 1;
        i2 = 0;
        b_last_tmp = 0;
      } else {
        i1 = (int)(n_tmp + 1.0);
        i2 = (int)(n_tmp + 1.0) - 1;
        b_last_tmp = hist2->size[0];
      }
      idx = b_last_tmp - i2;
      b_last_tmp = b_drift->size[0] * b_drift->size[1];
      b_drift->size[0] = 1;
      b_drift->size[1] = idx;
      emxEnsureCapacity_real_T(b_drift, b_last_tmp);
      drift_data = b_drift->data;
      for (b_last_tmp = 0; b_last_tmp < idx; b_last_tmp++) {
        drift_data[b_last_tmp] = hist2_data[(i1 + b_last_tmp) - 1] -
                                 b_drift_data[drift->size[0] - 1];
      }
      idx = b_drift->size[1];
      for (i1 = 0; i1 < idx; i1++) {
        hist2_data[i2 + i1] = drift_data[i1];
      }
    }
  }
  emxFree_real_T(&drift);
  i = hist_out->size[0];
  hist_out->size[0] = hist->size[0];
  emxEnsureCapacity_real_T(hist_out, i);
  drift_data = hist_out->data;
  idx = hist->size[0];
  for (i = 0; i < idx; i++) {
    drift_data[i] = 0.0;
  }
  if (hist2->size[0] < 1) {
    idx = 0;
  } else {
    idx = hist2->size[0];
  }
  for (i = 0; i < idx; i++) {
    drift_data[i] = hist2_data[i];
  }
  i = is_body_moving->size[0];
  is_body_moving->size[0] = hist2->size[0];
  emxEnsureCapacity_boolean_T(is_body_moving, i);
  is_body_moving_data = is_body_moving->data;
  idx = hist2->size[0];
  for (i = 0; i < idx; i++) {
    is_body_moving_data[i] = false;
  }
  if (locs->size[0] >= 2) {
    i = locs->size[0];
    for (ti = 0; ti <= i - 2; ti++) {
      amin = locs_data[ti + 1];
      if (locs_data[ti] + 1.0 > amin) {
        i1 = 0;
        i2 = 0;
      } else {
        i1 = (int)(locs_data[ti] + 1.0) - 1;
        i2 = (int)amin;
      }
      b_last_tmp = i2 - i1;
      if (b_last_tmp <= 2) {
        if (b_last_tmp == 1) {
          amax = hist2_data[i1];
          d1_tmp = hist2_data[i1];
        } else {
          amax = hist2_data[i2 - 1];
          if ((!(hist2_data[i1] < amax)) &&
              ((!rtIsNaN(hist2_data[i1])) || rtIsNaN(amax))) {
            amax = hist2_data[i1];
          }
          d1_tmp = hist2_data[i2 - 1];
          if ((!(hist2_data[i1] > d1_tmp)) &&
              ((!rtIsNaN(hist2_data[i1])) || rtIsNaN(hist2_data[i2 - 1]))) {
            d1_tmp = hist2_data[i1];
          }
        }
      } else {
        if (!rtIsNaN(hist2_data[i1])) {
          idx = 1;
        } else {
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= b_last_tmp)) {
            if (!rtIsNaN(hist2_data[(i1 + k) - 1])) {
              idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }
        if (idx == 0) {
          amax = hist2_data[i1];
        } else {
          amax = hist2_data[(i1 + idx) - 1];
          i2 = idx + 1;
          for (k = i2; k <= b_last_tmp; k++) {
            delta1 = hist2_data[(i1 + k) - 1];
            if (amax < delta1) {
              amax = delta1;
            }
          }
        }
        if (!rtIsNaN(hist2_data[i1])) {
          idx = 1;
        } else {
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= b_last_tmp)) {
            if (!rtIsNaN(hist2_data[(i1 + k) - 1])) {
              idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }
        if (idx == 0) {
          d1_tmp = hist2_data[i1];
        } else {
          d1_tmp = hist2_data[(i1 + idx) - 1];
          i2 = idx + 1;
          for (k = i2; k <= b_last_tmp; k++) {
            delta1 = hist2_data[(i1 + k) - 1];
            if (d1_tmp > delta1) {
              d1_tmp = delta1;
            }
          }
        }
      }
      if (amax - d1_tmp > moving_thre) {
        if (locs_data[ti] + 1.0 > amin) {
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
      }
      time_duration = (time_duration + amin) - locs_data[ti];
      (*resp_freq)++;
    }
  }
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
    i2 = b_drift->size[0] * b_drift->size[1];
    b_drift->size[0] = 1;
    idx = i1 - i;
    b_drift->size[1] = idx;
    emxEnsureCapacity_real_T(b_drift, i2);
    drift_data = b_drift->data;
    if (idx >= 1) {
      drift_data[idx - 1] = 1.0;
      if (b_drift->size[1] >= 2) {
        drift_data[0] = 0.0;
        if (b_drift->size[1] >= 3) {
          delta1 = 1.0 / ((double)b_drift->size[1] - 1.0);
          i1 = b_drift->size[1];
          for (k = 0; k <= i1 - 3; k++) {
            drift_data[k + 1] = ((double)k + 1.0) * delta1;
          }
        }
      }
    }
    for (i1 = 0; i1 < idx; i1++) {
      hist2_data[i1] = hist2_data[i + i1];
    }
    i = hist2->size[0];
    hist2->size[0] = idx;
    emxEnsureCapacity_real_T(hist2, i);
    interp1(b_drift, hist2, tmp);
    amin = minimum(tmp);
    for (i = 0; i < 100; i++) {
      tmp[i] -= amin;
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
      amax = tmp[0];
    } else {
      amax = tmp[idx - 1];
      i = idx + 1;
      for (k = i; k < 101; k++) {
        amin = tmp[k - 1];
        if (amax < amin) {
          amax = amin;
        }
      }
    }
    for (i = 0; i < 100; i++) {
      tmp[i] /= amax;
    }
    if (dtw(last_tmp, tmp) > 0.1) {
      for (i = 0; i < 100; i++) {
        amin = tmp[i];
        last_tmp[i] = amin;
        resp_waveform[i] =
            (amin + resp_waveform[i] * counter) / (counter + 1.0);
      }
      counter++;
      /*          resp_waveform = normalize_waveform(resp_waveform); */
      *new_waveform = true;
    }
  } else {
    memset(&tmp[0], 0, 100U * sizeof(double));
  }
  emxFree_real_T(&locs);
  emxFree_real_T(&b_drift);
  emxFree_real_T(&hist2);
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
