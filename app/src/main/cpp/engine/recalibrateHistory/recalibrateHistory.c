/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: recalibrateHistory.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 07-Feb-2023 10:03:28
 */

/* Include Files */
#include "recalibrateHistory.h"
#include "findpeaks.h"
#include "interp1.h"
#include "prctile.h"
#include "recalibrateHistory_emxutil.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void b_binary_expand_op(emxArray_real_T *in1, int in2, int in3, int in4,
                               int in5, const emxArray_real_T *in6);

static void recalibrateCore(emxArray_real_T *hist2, double moving_thre,
                            emxArray_real_T *hist_out,
                            emxArray_boolean_T *is_body_moving, double tmp[100],
                            double *resp_freq);

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
 * Arguments    : emxArray_real_T *hist2
 *                double moving_thre
 *                emxArray_real_T *hist_out
 *                emxArray_boolean_T *is_body_moving
 *                double tmp[100]
 *                double *resp_freq
 * Return Type  : void
 */
static void recalibrateCore(emxArray_real_T *hist2, double moving_thre,
                            emxArray_real_T *hist_out,
                            emxArray_boolean_T *is_body_moving, double tmp[100],
                            double *resp_freq)
{
  emxArray_real_T *b_drift;
  emxArray_real_T *b_hist2;
  emxArray_real_T *drift;
  emxArray_real_T *locs;
  double amax;
  double amin;
  double d1_tmp;
  double delta1;
  double *b_drift_data;
  double *drift_data;
  double *hist2_data;
  double *locs_data;
  int i;
  int i1;
  int i2;
  int idx;
  int k;
  int last_tmp;
  int ti;
  boolean_T exitg1;
  boolean_T *is_body_moving_data;
  hist2_data = hist2->data;
  amin = prctile(hist2);
  amax = b_prctile(hist2) - amin;
  emxInit_real_T(&b_hist2, 1);
  i = b_hist2->size[0];
  b_hist2->size[0] = hist2->size[0];
  emxEnsureCapacity_real_T(b_hist2, i);
  drift_data = b_hist2->data;
  idx = hist2->size[0];
  for (i = 0; i < idx; i++) {
    drift_data[i] = (hist2_data[i] - amin) / amax;
  }
  emxInit_real_T(&locs, 1);
  emxInit_real_T(&drift, 1);
  findpeaks(b_hist2, drift, locs);
  locs_data = locs->data;
  /*  [~, locs2] = findpeaks(-hist, ... */
  /*      'MinPeakDistance',100, 'MinPeakWidth', 50, 'MinPeakProminence', 0.05);
   */
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
        last_tmp = 0;
        idx = 0;
      } else {
        i1 = (int)(locs_data[ti] + 1.0) - 1;
        i2 = (int)locs_data[ti + 1];
        last_tmp = i1;
        idx = (int)locs_data[ti + 1];
      }
      if (i2 - i1 == drift->size[0]) {
        idx -= last_tmp;
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
          hist2_data[last_tmp + i1] = drift_data[i1];
        }
      } else {
        b_binary_expand_op(hist2, last_tmp, idx, i1, i2 - 1, drift);
        hist2_data = hist2->data;
      }
      if (n_tmp + 1.0 > hist2->size[0]) {
        i1 = 1;
      } else {
        i1 = (int)(n_tmp + 1.0);
      }
      if (n_tmp + 1.0 > hist2->size[0]) {
        i2 = 0;
        last_tmp = 0;
      } else {
        i2 = (int)(locs_data[ti + 1] + 1.0) - 1;
        last_tmp = hist2->size[0];
      }
      idx = last_tmp - i2;
      last_tmp = b_drift->size[0] * b_drift->size[1];
      b_drift->size[0] = 1;
      b_drift->size[1] = idx;
      emxEnsureCapacity_real_T(b_drift, last_tmp);
      drift_data = b_drift->data;
      for (last_tmp = 0; last_tmp < idx; last_tmp++) {
        drift_data[last_tmp] =
            hist2_data[(i1 + last_tmp) - 1] - b_drift_data[drift->size[0] - 1];
      }
      idx = b_drift->size[1];
      for (i1 = 0; i1 < idx; i1++) {
        hist2_data[i2 + i1] = drift_data[i1];
      }
    }
  }
  emxFree_real_T(&drift);
  i = hist_out->size[0];
  hist_out->size[0] = hist2->size[0];
  emxEnsureCapacity_real_T(hist_out, i);
  drift_data = hist_out->data;
  idx = hist2->size[0];
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
      d1_tmp = locs_data[ti + 1];
      if (locs_data[ti] + 1.0 > d1_tmp) {
        i1 = 0;
        i2 = 0;
      } else {
        i1 = (int)(locs_data[ti] + 1.0) - 1;
        i2 = (int)d1_tmp;
      }
      last_tmp = i2 - i1;
      if (last_tmp <= 2) {
        if (last_tmp == 1) {
          delta1 = hist2_data[i1];
          amax = hist2_data[i1];
        } else {
          delta1 = hist2_data[i2 - 1];
          if ((!(hist2_data[i1] < delta1)) &&
              ((!rtIsNaN(hist2_data[i1])) || rtIsNaN(delta1))) {
            delta1 = hist2_data[i1];
          }
          amax = hist2_data[i2 - 1];
          if ((!(hist2_data[i1] > amax)) &&
              ((!rtIsNaN(hist2_data[i1])) || rtIsNaN(hist2_data[i2 - 1]))) {
            amax = hist2_data[i1];
          }
        }
      } else {
        if (!rtIsNaN(hist2_data[i1])) {
          idx = 1;
        } else {
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= last_tmp)) {
            if (!rtIsNaN(hist2_data[(i1 + k) - 1])) {
              idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }
        if (idx == 0) {
          delta1 = hist2_data[i1];
        } else {
          delta1 = hist2_data[(i1 + idx) - 1];
          i2 = idx + 1;
          for (k = i2; k <= last_tmp; k++) {
            amin = hist2_data[(i1 + k) - 1];
            if (delta1 < amin) {
              delta1 = amin;
            }
          }
        }
        if (!rtIsNaN(hist2_data[i1])) {
          idx = 1;
        } else {
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= last_tmp)) {
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
          for (k = i2; k <= last_tmp; k++) {
            amin = hist2_data[(i1 + k) - 1];
            if (amax > amin) {
              amax = amin;
            }
          }
        }
      }
      if (delta1 - amax > moving_thre) {
        if (locs_data[ti] + 1.0 > d1_tmp) {
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
    }
  }
  /*  time_duration = time_duration * 960 / 48e3; */
  /*  resp_freq = resp_freq / time_duration * 60; */
  *resp_freq = ((double)locs->size[0] - 1.0) * 60.0 / 24.0;
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
    i1 = b_hist2->size[0];
    b_hist2->size[0] = idx;
    emxEnsureCapacity_real_T(b_hist2, i1);
    drift_data = b_hist2->data;
    for (i1 = 0; i1 < idx; i1++) {
      drift_data[i1] = hist2_data[i + i1];
    }
    interp1(b_drift, b_hist2, tmp);
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
      delta1 = tmp[0];
    } else {
      delta1 = tmp[idx - 1];
      i = idx + 1;
      for (k = i; k < 101; k++) {
        d1_tmp = tmp[k - 1];
        if (delta1 > d1_tmp) {
          delta1 = d1_tmp;
        }
      }
    }
    for (i = 0; i < 100; i++) {
      tmp[i] -= delta1;
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
      delta1 = tmp[0];
    } else {
      delta1 = tmp[idx - 1];
      i = idx + 1;
      for (k = i; k < 101; k++) {
        d1_tmp = tmp[k - 1];
        if (delta1 < d1_tmp) {
          delta1 = d1_tmp;
        }
      }
    }
    for (i = 0; i < 100; i++) {
      tmp[i] /= delta1;
    }
    /*      if(dtw(last_tmp, tmp) > 0.1) */
    /*          last_tmp = tmp; */
    /*          resp_waveform =  (tmp + resp_waveform * counter) / (counter +
     * 1); */
    /*          counter = counter + 1; */
    /*  %         resp_waveform = normalize_waveform(resp_waveform); */
    /*   */
    /*          new_waveform = true; */
    /*      end */
  } else {
    memset(&tmp[0], 0, 100U * sizeof(double));
  }
  emxFree_real_T(&b_hist2);
  emxFree_real_T(&locs);
  emxFree_real_T(&b_drift);
}

/*
 * Arguments    : const emxArray_real_T *hist
 *                double moving_thre
 *                const double last_tmp[100]
 *                const double resp_waveform[100]
 *                boolean_T need_bandpass
 *                emxArray_real_T *hist_out
 *                emxArray_boolean_T *is_body_moving
 *                double tmp[100]
 *                boolean_T *new_waveform
 *                double *resp_freq
 * Return Type  : void
 */
void recalibrateHistory(const emxArray_real_T *hist, double moving_thre,
                        const double last_tmp[100],
                        const double resp_waveform[100],
                        boolean_T need_bandpass, emxArray_real_T *hist_out,
                        emxArray_boolean_T *is_body_moving, double tmp[100],
                        boolean_T *new_waveform, double *resp_freq)
{
  static const double b[286] = {
      0.0179,  0.002,   0.0019,  0.0017,  0.0014,  0.001,   0.0004,  -0.0002,
      -0.0009, -0.0016, -0.0023, -0.0029, -0.0035, -0.004,  -0.0044, -0.0046,
      -0.0047, -0.0046, -0.0044, -0.004,  -0.0034, -0.0027, -0.002,  -0.0012,
      -0.0005, 0.0003,  0.0009,  0.0014,  0.0018,  0.002,   0.002,   0.0017,
      0.0013,  0.0007,  -0.0001, -0.001,  -0.002,  -0.003,  -0.004,  -0.005,
      -0.0058, -0.0065, -0.007,  -0.0072, -0.0072, -0.0069, -0.0064, -0.0057,
      -0.0048, -0.0037, -0.0026, -0.0014, -0.0003, 0.0007,  0.0016,  0.0022,
      0.0026,  0.0027,  0.0025,  0.002,   0.0012,  0.0001,  -0.0011, -0.0026,
      -0.004,  -0.0055, -0.007,  -0.0083, -0.0093, -0.0101, -0.0106, -0.0107,
      -0.0104, -0.0098, -0.0088, -0.0076, -0.0061, -0.0045, -0.0029, -0.0012,
      0.0003,  0.0016,  0.0026,  0.0033,  0.0036,  0.0034,  0.0028,  0.0017,
      0.0003,  -0.0014, -0.0033, -0.0054, -0.0075, -0.0095, -0.0113, -0.0128,
      -0.014,  -0.0146, -0.0147, -0.0143, -0.0134, -0.012,  -0.0101, -0.0079,
      -0.0055, -0.003,  -0.0005, 0.0018,  0.0038,  0.0053,  0.0063,  0.0066,
      0.0062,  0.0051,  0.0034,  0.001,   -0.002,  -0.0054, -0.0089, -0.0126,
      -0.0161, -0.0192, -0.0218, -0.0236, -0.0245, -0.0244, -0.023,  -0.0204,
      -0.0166, -0.0116, -0.0055, 0.0015,  0.0093,  0.0177,  0.0263,  0.035,
      0.0433,  0.0511,  0.0581,  0.064,   0.0686,  0.0718,  0.0734,  0.0734,
      0.0718,  0.0686,  0.064,   0.0581,  0.0511,  0.0433,  0.035,   0.0263,
      0.0177,  0.0093,  0.0015,  -0.0055, -0.0116, -0.0166, -0.0204, -0.023,
      -0.0244, -0.0245, -0.0236, -0.0218, -0.0192, -0.0161, -0.0126, -0.0089,
      -0.0054, -0.002,  0.001,   0.0034,  0.0051,  0.0062,  0.0066,  0.0063,
      0.0053,  0.0038,  0.0018,  -0.0005, -0.003,  -0.0055, -0.0079, -0.0101,
      -0.012,  -0.0134, -0.0143, -0.0147, -0.0146, -0.014,  -0.0128, -0.0113,
      -0.0095, -0.0075, -0.0054, -0.0033, -0.0014, 0.0003,  0.0017,  0.0028,
      0.0034,  0.0036,  0.0033,  0.0026,  0.0016,  0.0003,  -0.0012, -0.0029,
      -0.0045, -0.0061, -0.0076, -0.0088, -0.0098, -0.0104, -0.0107, -0.0106,
      -0.0101, -0.0093, -0.0083, -0.007,  -0.0055, -0.004,  -0.0026, -0.0011,
      0.0001,  0.0012,  0.002,   0.0025,  0.0027,  0.0026,  0.0022,  0.0016,
      0.0007,  -0.0003, -0.0014, -0.0026, -0.0037, -0.0048, -0.0057, -0.0064,
      -0.0069, -0.0072, -0.0072, -0.007,  -0.0065, -0.0058, -0.005,  -0.004,
      -0.003,  -0.002,  -0.001,  -0.0001, 0.0007,  0.0013,  0.0017,  0.002,
      0.002,   0.0018,  0.0014,  0.0009,  0.0003,  -0.0005, -0.0012, -0.002,
      -0.0027, -0.0034, -0.004,  -0.0044, -0.0046, -0.0047, -0.0046, -0.0044,
      -0.004,  -0.0035, -0.0029, -0.0023, -0.0016, -0.0009, -0.0002, 0.0004,
      0.001,   0.0014,  0.0017,  0.0019,  0.002,   0.0179};
  emxArray_boolean_T *a__3;
  emxArray_real_T *b_hist;
  emxArray_real_T *hist2;
  double a__4[100];
  const double *hist_data;
  double a__1;
  double *hist2_data;
  int hist2_tmp;
  int i;
  int j;
  int k;
  int nx;
  int nx_m_nb;
  (void)last_tmp;
  (void)resp_waveform;
  (void)need_bandpass;
  hist_data = hist->data;
  /*  hist = bandpass(hist, [0.2, 1.5], 50, 'StopbandAttenuation', 30,
   * 'Steepness', 0.6); */
  /*  if(need_bandpass) */
  /*      filter_b = getfiltercoeffs; */
  /*      hist = filter(filter_b, 1, hist); */
  /*  end */
  /*  hist_out = wdenoise(hist); */
  emxInit_real_T(&hist2, 1);
  nx = hist->size[0];
  i = hist2->size[0];
  hist2->size[0] = hist->size[0];
  emxEnsureCapacity_real_T(hist2, i);
  hist2_data = hist2->data;
  nx_m_nb = hist->size[0];
  for (i = 0; i < nx_m_nb; i++) {
    hist2_data[i] = 0.0;
  }
  if (hist->size[0] >= 572) {
    for (k = 0; k < 286; k++) {
      nx_m_nb = k + 1;
      for (j = nx_m_nb; j <= nx; j++) {
        hist2_data[j - 1] += b[k] * hist_data[(j - k) - 1];
      }
    }
  } else {
    int naxpy;
    if (hist->size[0] > 286) {
      nx_m_nb = hist->size[0] - 287;
    } else {
      nx_m_nb = -1;
    }
    for (k = 0; k <= nx_m_nb; k++) {
      for (j = 0; j < 286; j++) {
        hist2_tmp = k + j;
        hist2_data[hist2_tmp] += hist_data[k] * b[j];
      }
    }
    naxpy = hist->size[0] - nx_m_nb;
    i = nx_m_nb + 2;
    for (k = i; k <= nx; k++) {
      for (j = 0; j <= naxpy - 2; j++) {
        hist2_tmp = (k + j) - 1;
        hist2_data[hist2_tmp] += hist_data[k - 1] * b[j];
      }
      naxpy--;
    }
  }
  if (hist2->size[0] < 286) {
    i = 0;
    hist2_tmp = 0;
  } else {
    i = 285;
    hist2_tmp = hist2->size[0];
  }
  nx_m_nb = hist2_tmp - i;
  for (hist2_tmp = 0; hist2_tmp < nx_m_nb; hist2_tmp++) {
    hist2_data[hist2_tmp] = hist2_data[i + hist2_tmp];
  }
  i = hist2->size[0];
  hist2->size[0] = nx_m_nb;
  emxEnsureCapacity_real_T(hist2, i);
  emxInit_real_T(&b_hist, 1);
  i = b_hist->size[0];
  b_hist->size[0] = hist->size[0];
  emxEnsureCapacity_real_T(b_hist, i);
  hist2_data = b_hist->data;
  nx_m_nb = hist->size[0] - 1;
  for (i = 0; i <= nx_m_nb; i++) {
    hist2_data[i] = hist_data[i];
  }
  recalibrateCore(b_hist, moving_thre, hist_out, is_body_moving, tmp, &a__1);
  emxInit_boolean_T(&a__3, 1);
  recalibrateCore(hist2, moving_thre, b_hist, a__3, a__4, resp_freq);
  emxFree_real_T(&b_hist);
  emxFree_boolean_T(&a__3);
  emxFree_real_T(&hist2);
  *new_waveform = false;
}

/*
 * File trailer for recalibrateHistory.c
 *
 * [EOF]
 */
