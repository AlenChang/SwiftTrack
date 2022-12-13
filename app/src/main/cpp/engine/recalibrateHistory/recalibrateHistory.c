/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: recalibrateHistory.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Dec-2022 11:27:48
 */

/* Include Files */
#include "recalibrateHistory.h"
#include "findpeaks.h"
#include "interp1.h"
#include "minOrMax.h"
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
  emxArray_real_T *b_hist;
  emxArray_real_T *drift;
  emxArray_real_T *locs;
  double amin;
  double delta1;
  double pdist;
  double time_duration;
  double *b_drift_data;
  double *drift_data;
  double *hist_data;
  double *locs_data;
  int i;
  int i1;
  int idx;
  int ix;
  int iy;
  int k;
  int ti;
  boolean_T *is_body_moving_data;
  if (!isInitialized_recalibrateHistory) {
    recalibrateHistory_initialize();
  }
  hist_data = hist->data;
  amin = minimum(hist);
  pdist = maximum(hist) - amin;
  emxInit_real_T(&b_hist, 1);
  i = b_hist->size[0];
  b_hist->size[0] = hist->size[0];
  emxEnsureCapacity_real_T(b_hist, i);
  drift_data = b_hist->data;
  idx = hist->size[0];
  for (i = 0; i < idx; i++) {
    drift_data[i] = (hist_data[i] - amin) / pdist;
  }
  emxInit_real_T(&locs, 1);
  emxInit_real_T(&drift, 1);
  findpeaks(b_hist, drift, locs);
  locs_data = locs->data;
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
      double d1_tmp;
      double n_tmp;
      d1_tmp = hist_data[(int)locs_data[ti] - 1];
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
          pdist = hist_data[(int)n_tmp - 1];
          drift_data[(int)amin - 1] = pdist;
          if (b_drift->size[1] >= 2) {
            drift_data[0] = d1_tmp;
            if (b_drift->size[1] >= 3) {
              if (d1_tmp == -pdist) {
                amin = pdist / ((double)b_drift->size[1] - 1.0);
                i1 = b_drift->size[1] - 1;
                for (k = 2; k <= i1; k++) {
                  drift_data[k - 1] =
                      (double)(((k << 1) - b_drift->size[1]) - 1) * amin;
                }
                if ((b_drift->size[1] & 1) == 1) {
                  drift_data[b_drift->size[1] >> 1] = 0.0;
                }
              } else if (((d1_tmp < 0.0) != (pdist < 0.0)) &&
                         ((fabs(d1_tmp) > 8.9884656743115785E+307) ||
                          (fabs(pdist) > 8.9884656743115785E+307))) {
                delta1 = d1_tmp / ((double)b_drift->size[1] - 1.0);
                amin = pdist / ((double)b_drift->size[1] - 1.0);
                i1 = b_drift->size[1];
                for (k = 0; k <= i1 - 3; k++) {
                  drift_data[k + 1] = (d1_tmp + amin * ((double)k + 1.0)) -
                                      delta1 * ((double)k + 1.0);
                }
              } else {
                delta1 = (pdist - d1_tmp) / ((double)b_drift->size[1] - 1.0);
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
        ix = 0;
        iy = 0;
        idx = 0;
      } else {
        i1 = (int)(locs_data[ti] + 1.0) - 1;
        ix = (int)locs_data[ti + 1];
        iy = i1;
        idx = (int)locs_data[ti + 1];
      }
      if (ix - i1 == drift->size[0]) {
        idx -= iy;
        ix = b_drift->size[0] * b_drift->size[1];
        b_drift->size[0] = 1;
        b_drift->size[1] = idx;
        emxEnsureCapacity_real_T(b_drift, ix);
        drift_data = b_drift->data;
        for (ix = 0; ix < idx; ix++) {
          drift_data[ix] = hist_data[i1 + ix] - b_drift_data[ix];
        }
        idx = b_drift->size[1];
        for (i1 = 0; i1 < idx; i1++) {
          hist_data[iy + i1] = drift_data[i1];
        }
      } else {
        b_binary_expand_op(hist, iy, idx, i1, ix - 1, drift);
        hist_data = hist->data;
      }
      if (n_tmp + 1.0 > hist->size[0]) {
        i1 = 1;
      } else {
        i1 = (int)(n_tmp + 1.0);
      }
      if (n_tmp + 1.0 > hist->size[0]) {
        ix = 0;
        iy = 0;
      } else {
        ix = (int)(locs_data[ti + 1] + 1.0) - 1;
        iy = hist->size[0];
      }
      idx = iy - ix;
      iy = b_drift->size[0] * b_drift->size[1];
      b_drift->size[0] = 1;
      b_drift->size[1] = idx;
      emxEnsureCapacity_real_T(b_drift, iy);
      drift_data = b_drift->data;
      for (iy = 0; iy < idx; iy++) {
        drift_data[iy] =
            hist_data[(i1 + iy) - 1] - b_drift_data[drift->size[0] - 1];
      }
      idx = b_drift->size[1];
      for (i1 = 0; i1 < idx; i1++) {
        hist_data[ix + i1] = drift_data[i1];
      }
    }
  }
  /*  hist_out = wdenoise(hist); */
  i = hist_out->size[0];
  hist_out->size[0] = hist->size[0];
  emxEnsureCapacity_real_T(hist_out, i);
  drift_data = hist_out->data;
  idx = hist->size[0];
  for (i = 0; i < idx; i++) {
    drift_data[i] = hist_data[i];
  }
  i = is_body_moving->size[0];
  is_body_moving->size[0] = hist->size[0];
  emxEnsureCapacity_boolean_T(is_body_moving, i);
  is_body_moving_data = is_body_moving->data;
  idx = hist->size[0];
  for (i = 0; i < idx; i++) {
    is_body_moving_data[i] = false;
  }
  if (locs->size[0] >= 2) {
    i = locs->size[0];
    for (ti = 0; ti <= i - 2; ti++) {
      amin = locs_data[ti + 1];
      if (locs_data[ti] + 1.0 > amin) {
        i1 = 0;
        ix = 0;
      } else {
        i1 = (int)(locs_data[ti] + 1.0) - 1;
        ix = (int)amin;
      }
      idx = ix - i1;
      ix = drift->size[0];
      drift->size[0] = idx;
      emxEnsureCapacity_real_T(drift, ix);
      b_drift_data = drift->data;
      for (ix = 0; ix < idx; ix++) {
        b_drift_data[ix] = hist_data[i1 + ix];
      }
      if (maximum(drift) - minimum(drift) > moving_thre) {
        if (locs_data[ti] + 1.0 > amin) {
          i1 = 0;
          ix = 0;
        } else {
          i1 = (int)(locs_data[ti] + 1.0) - 1;
          ix = (int)locs_data[ti + 1];
        }
        idx = ix - i1;
        for (ix = 0; ix < idx; ix++) {
          is_body_moving_data[i1 + ix] = true;
        }
      } else {
        time_duration = (time_duration + amin) - locs_data[ti];
        (*resp_freq)++;
      }
    }
  }
  emxFree_real_T(&drift);
  time_duration = time_duration * 960.0 / 48000.0;
  *resp_freq = *resp_freq / time_duration * 60.0;
  *new_waveform = false;
  if (locs->size[0] > 2) {
    double C[10000];
    boolean_T exitg1;
    if (locs_data[locs->size[0] - 2] > locs_data[locs->size[0] - 1]) {
      i = 0;
      i1 = 0;
    } else {
      i = (int)locs_data[locs->size[0] - 2] - 1;
      i1 = (int)locs_data[locs->size[0] - 1];
    }
    ix = b_drift->size[0] * b_drift->size[1];
    b_drift->size[0] = 1;
    idx = i1 - i;
    b_drift->size[1] = idx;
    emxEnsureCapacity_real_T(b_drift, ix);
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
    i1 = b_hist->size[0];
    b_hist->size[0] = idx;
    emxEnsureCapacity_real_T(b_hist, i1);
    drift_data = b_hist->data;
    for (i1 = 0; i1 < idx; i1++) {
      drift_data[i1] = hist_data[i + i1];
    }
    interp1(b_drift, b_hist, tmp);
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
      pdist = tmp[0];
    } else {
      pdist = tmp[idx - 1];
      i = idx + 1;
      for (k = i; k < 101; k++) {
        amin = tmp[k - 1];
        if (pdist > amin) {
          pdist = amin;
        }
      }
    }
    for (i = 0; i < 100; i++) {
      tmp[i] -= pdist;
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
      pdist = tmp[0];
    } else {
      pdist = tmp[idx - 1];
      i = idx + 1;
      for (k = i; k < 101; k++) {
        amin = tmp[k - 1];
        if (pdist < amin) {
          pdist = amin;
        }
      }
    }
    amin = 0.0;
    for (ix = 0; ix < 100; ix++) {
      tmp[ix] /= pdist;
      amin += fabs(last_tmp[ix] - tmp[0]);
      C[ix] = amin;
    }
    idx = 0;
    for (iy = 0; iy < 99; iy++) {
      amin = C[idx];
      pdist = C[idx];
      for (ix = 0; ix < 100; ix++) {
        k = idx + ix;
        delta1 = C[k];
        if (delta1 < amin) {
          amin = C[k];
        }
        if (pdist < amin) {
          amin = pdist;
        }
        pdist = amin + fabs(last_tmp[ix] - tmp[iy + 1]);
        C[k + 100] = pdist;
        amin = delta1;
      }
      idx += 100;
    }
    if (C[9999] > 0.1) {
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
  emxFree_real_T(&b_hist);
  emxFree_real_T(&locs);
  emxFree_real_T(&b_drift);
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
