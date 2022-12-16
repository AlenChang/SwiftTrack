/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: recalibrateHistory.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 16-Dec-2022 15:26:49
 */

/* Include Files */
#include "recalibrateHistory.h"
#include "dtw.h"
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
 *                boolean_T need_bandpass
 *                emxArray_real_T *hist_out
 *                emxArray_boolean_T *is_body_moving
 *                double tmp[100]
 *                boolean_T *new_waveform
 *                double *resp_freq
 * Return Type  : void
 */
void recalibrateHistory(emxArray_real_T *hist, double moving_thre,
                        double last_tmp[100], double resp_waveform[100],
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
  emxArray_real_T *b_drift;
  emxArray_real_T *b_hist;
  emxArray_real_T *drift;
  emxArray_real_T *locs;
  double amax;
  double amin;
  double delta1;
  double time_duration;
  double *b_drift_data;
  double *drift_data;
  double *hist_data;
  double *locs_data;
  int hist_tmp;
  int i;
  int j;
  int k;
  int naxpy;
  int nx;
  int nx_m_nb;
  boolean_T *is_body_moving_data;
  if (!isInitialized_recalibrateHistory) {
    recalibrateHistory_initialize();
  }
  hist_data = hist->data;
  /*  hist = bandpass(hist, [0.2, 1.5], 50, 'StopbandAttenuation', 30,
   * 'Steepness', 0.6); */
  emxInit_real_T(&drift, 1);
  if (need_bandpass) {
    i = drift->size[0];
    drift->size[0] = hist->size[0];
    emxEnsureCapacity_real_T(drift, i);
    drift_data = drift->data;
    nx_m_nb = hist->size[0];
    for (i = 0; i < nx_m_nb; i++) {
      drift_data[i] = hist_data[i];
    }
    nx = drift->size[0];
    i = hist->size[0];
    hist->size[0] = drift->size[0];
    emxEnsureCapacity_real_T(hist, i);
    hist_data = hist->data;
    nx_m_nb = drift->size[0];
    for (i = 0; i < nx_m_nb; i++) {
      hist_data[i] = 0.0;
    }
    if (drift->size[0] >= 572) {
      for (k = 0; k < 286; k++) {
        nx_m_nb = k + 1;
        for (j = nx_m_nb; j <= nx; j++) {
          hist_data[j - 1] += b[k] * drift_data[(j - k) - 1];
        }
      }
    } else {
      if (drift->size[0] > 286) {
        nx_m_nb = drift->size[0] - 287;
      } else {
        nx_m_nb = -1;
      }
      for (k = 0; k <= nx_m_nb; k++) {
        for (j = 0; j < 286; j++) {
          hist_tmp = k + j;
          hist_data[hist_tmp] += drift_data[k] * b[j];
        }
      }
      naxpy = drift->size[0] - nx_m_nb;
      i = nx_m_nb + 2;
      for (k = i; k <= nx; k++) {
        for (j = 0; j <= naxpy - 2; j++) {
          hist_tmp = (k + j) - 1;
          hist_data[hist_tmp] += drift_data[k - 1] * b[j];
        }
        naxpy--;
      }
    }
  }
  amin = minimum(hist);
  amax = maximum(hist) - amin;
  emxInit_real_T(&b_hist, 1);
  i = b_hist->size[0];
  b_hist->size[0] = hist->size[0];
  emxEnsureCapacity_real_T(b_hist, i);
  drift_data = b_hist->data;
  nx_m_nb = hist->size[0];
  for (i = 0; i < nx_m_nb; i++) {
    drift_data[i] = (hist_data[i] - amin) / amax;
  }
  emxInit_real_T(&locs, 1);
  findpeaks(b_hist, drift, locs);
  locs_data = locs->data;
  /*  [~, locs2] = findpeaks(-hist, ... */
  /*      'MinPeakDistance',100, 'MinPeakWidth', 50, 'MinPeakProminence', 0.05);
   */
  time_duration = 0.0;
  *resp_freq = 0.0;
  emxInit_real_T(&b_drift, 2);
  b_drift_data = b_drift->data;
  if (locs->size[0] >= 2) {
    i = locs->size[0];
    for (nx = 0; nx <= i - 2; nx++) {
      double d1_tmp;
      double n_tmp;
      d1_tmp = hist_data[(int)locs_data[nx] - 1];
      n_tmp = locs_data[nx + 1];
      amin = n_tmp - locs_data[nx];
      if (!(amin >= 0.0)) {
        b_drift->size[0] = 1;
        b_drift->size[1] = 0;
      } else {
        j = b_drift->size[0] * b_drift->size[1];
        b_drift->size[0] = 1;
        b_drift->size[1] = (int)amin;
        emxEnsureCapacity_real_T(b_drift, j);
        b_drift_data = b_drift->data;
        if ((int)amin >= 1) {
          amax = hist_data[(int)n_tmp - 1];
          b_drift_data[(int)amin - 1] = amax;
          if (b_drift->size[1] >= 2) {
            b_drift_data[0] = d1_tmp;
            if (b_drift->size[1] >= 3) {
              if (d1_tmp == -amax) {
                amin = amax / ((double)b_drift->size[1] - 1.0);
                j = b_drift->size[1] - 1;
                for (k = 2; k <= j; k++) {
                  b_drift_data[k - 1] =
                      (double)(((k << 1) - b_drift->size[1]) - 1) * amin;
                }
                if ((b_drift->size[1] & 1) == 1) {
                  b_drift_data[b_drift->size[1] >> 1] = 0.0;
                }
              } else if (((d1_tmp < 0.0) != (amax < 0.0)) &&
                         ((fabs(d1_tmp) > 8.9884656743115785E+307) ||
                          (fabs(amax) > 8.9884656743115785E+307))) {
                delta1 = d1_tmp / ((double)b_drift->size[1] - 1.0);
                amin = amax / ((double)b_drift->size[1] - 1.0);
                j = b_drift->size[1];
                for (k = 0; k <= j - 3; k++) {
                  b_drift_data[k + 1] = (d1_tmp + amin * ((double)k + 1.0)) -
                                        delta1 * ((double)k + 1.0);
                }
              } else {
                delta1 = (amax - d1_tmp) / ((double)b_drift->size[1] - 1.0);
                j = b_drift->size[1];
                for (k = 0; k <= j - 3; k++) {
                  b_drift_data[k + 1] = d1_tmp + ((double)k + 1.0) * delta1;
                }
              }
            }
          }
        }
      }
      amin = b_drift_data[0];
      j = drift->size[0];
      drift->size[0] = b_drift->size[1];
      emxEnsureCapacity_real_T(drift, j);
      drift_data = drift->data;
      nx_m_nb = b_drift->size[1];
      for (j = 0; j < nx_m_nb; j++) {
        drift_data[j] = b_drift_data[j] - amin;
      }
      if (locs_data[nx] + 1.0 > n_tmp) {
        j = 0;
        hist_tmp = 0;
        naxpy = 0;
        nx_m_nb = 0;
      } else {
        j = (int)(locs_data[nx] + 1.0) - 1;
        hist_tmp = (int)locs_data[nx + 1];
        naxpy = j;
        nx_m_nb = (int)locs_data[nx + 1];
      }
      if (hist_tmp - j == drift->size[0]) {
        nx_m_nb -= naxpy;
        hist_tmp = b_drift->size[0] * b_drift->size[1];
        b_drift->size[0] = 1;
        b_drift->size[1] = nx_m_nb;
        emxEnsureCapacity_real_T(b_drift, hist_tmp);
        b_drift_data = b_drift->data;
        for (hist_tmp = 0; hist_tmp < nx_m_nb; hist_tmp++) {
          b_drift_data[hist_tmp] =
              hist_data[j + hist_tmp] - drift_data[hist_tmp];
        }
        nx_m_nb = b_drift->size[1];
        for (j = 0; j < nx_m_nb; j++) {
          hist_data[naxpy + j] = b_drift_data[j];
        }
      } else {
        b_binary_expand_op(hist, naxpy, nx_m_nb, j, hist_tmp - 1, drift);
        hist_data = hist->data;
      }
      if (n_tmp + 1.0 > hist->size[0]) {
        j = 1;
      } else {
        j = (int)(n_tmp + 1.0);
      }
      if (n_tmp + 1.0 > hist->size[0]) {
        hist_tmp = 0;
        naxpy = 0;
      } else {
        hist_tmp = (int)(locs_data[nx + 1] + 1.0) - 1;
        naxpy = hist->size[0];
      }
      nx_m_nb = naxpy - hist_tmp;
      naxpy = b_drift->size[0] * b_drift->size[1];
      b_drift->size[0] = 1;
      b_drift->size[1] = nx_m_nb;
      emxEnsureCapacity_real_T(b_drift, naxpy);
      b_drift_data = b_drift->data;
      for (naxpy = 0; naxpy < nx_m_nb; naxpy++) {
        b_drift_data[naxpy] =
            hist_data[(j + naxpy) - 1] - drift_data[drift->size[0] - 1];
      }
      nx_m_nb = b_drift->size[1];
      for (j = 0; j < nx_m_nb; j++) {
        hist_data[hist_tmp + j] = b_drift_data[j];
      }
    }
  }
  /*  hist_out = wdenoise(hist); */
  i = hist_out->size[0];
  hist_out->size[0] = hist->size[0];
  emxEnsureCapacity_real_T(hist_out, i);
  drift_data = hist_out->data;
  nx_m_nb = hist->size[0];
  for (i = 0; i < nx_m_nb; i++) {
    drift_data[i] = hist_data[i];
  }
  i = is_body_moving->size[0];
  is_body_moving->size[0] = hist->size[0];
  emxEnsureCapacity_boolean_T(is_body_moving, i);
  is_body_moving_data = is_body_moving->data;
  nx_m_nb = hist->size[0];
  for (i = 0; i < nx_m_nb; i++) {
    is_body_moving_data[i] = false;
  }
  if (locs->size[0] >= 2) {
    i = locs->size[0];
    for (nx = 0; nx <= i - 2; nx++) {
      amax = locs_data[nx + 1];
      if (locs_data[nx] + 1.0 > amax) {
        j = 0;
        hist_tmp = 0;
      } else {
        j = (int)(locs_data[nx] + 1.0) - 1;
        hist_tmp = (int)amax;
      }
      nx_m_nb = hist_tmp - j;
      hist_tmp = drift->size[0];
      drift->size[0] = nx_m_nb;
      emxEnsureCapacity_real_T(drift, hist_tmp);
      drift_data = drift->data;
      for (hist_tmp = 0; hist_tmp < nx_m_nb; hist_tmp++) {
        drift_data[hist_tmp] = hist_data[j + hist_tmp];
      }
      if (maximum(drift) - minimum(drift) > moving_thre) {
        if (locs_data[nx] + 1.0 > amax) {
          j = 0;
          hist_tmp = 0;
        } else {
          j = (int)(locs_data[nx] + 1.0) - 1;
          hist_tmp = (int)locs_data[nx + 1];
        }
        nx_m_nb = hist_tmp - j;
        for (hist_tmp = 0; hist_tmp < nx_m_nb; hist_tmp++) {
          is_body_moving_data[j + hist_tmp] = true;
        }
      } else {
        time_duration = (time_duration + amax) - locs_data[nx];
        (*resp_freq)++;
      }
    }
  }
  emxFree_real_T(&drift);
  time_duration = time_duration * 960.0 / 48000.0;
  *resp_freq = *resp_freq / time_duration * 60.0;
  *new_waveform = false;
  if (locs->size[0] > 2) {
    boolean_T exitg1;
    if (locs_data[locs->size[0] - 2] > locs_data[locs->size[0] - 1]) {
      i = 0;
      j = 0;
    } else {
      i = (int)locs_data[locs->size[0] - 2] - 1;
      j = (int)locs_data[locs->size[0] - 1];
    }
    hist_tmp = b_drift->size[0] * b_drift->size[1];
    b_drift->size[0] = 1;
    nx_m_nb = j - i;
    b_drift->size[1] = nx_m_nb;
    emxEnsureCapacity_real_T(b_drift, hist_tmp);
    b_drift_data = b_drift->data;
    if (nx_m_nb >= 1) {
      b_drift_data[nx_m_nb - 1] = 1.0;
      if (b_drift->size[1] >= 2) {
        b_drift_data[0] = 0.0;
        if (b_drift->size[1] >= 3) {
          delta1 = 1.0 / ((double)b_drift->size[1] - 1.0);
          j = b_drift->size[1];
          for (k = 0; k <= j - 3; k++) {
            b_drift_data[k + 1] = ((double)k + 1.0) * delta1;
          }
        }
      }
    }
    j = b_hist->size[0];
    b_hist->size[0] = nx_m_nb;
    emxEnsureCapacity_real_T(b_hist, j);
    drift_data = b_hist->data;
    for (j = 0; j < nx_m_nb; j++) {
      drift_data[j] = hist_data[i + j];
    }
    interp1(b_drift, b_hist, tmp);
    if (!rtIsNaN(tmp[0])) {
      nx_m_nb = 1;
    } else {
      nx_m_nb = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k < 101)) {
        if (!rtIsNaN(tmp[k - 1])) {
          nx_m_nb = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (nx_m_nb == 0) {
      amin = tmp[0];
    } else {
      amin = tmp[nx_m_nb - 1];
      i = nx_m_nb + 1;
      for (k = i; k < 101; k++) {
        amax = tmp[k - 1];
        if (amin > amax) {
          amin = amax;
        }
      }
    }
    for (i = 0; i < 100; i++) {
      tmp[i] -= amin;
    }
    if (!rtIsNaN(tmp[0])) {
      nx_m_nb = 1;
    } else {
      nx_m_nb = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k < 101)) {
        if (!rtIsNaN(tmp[k - 1])) {
          nx_m_nb = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (nx_m_nb == 0) {
      amin = tmp[0];
    } else {
      amin = tmp[nx_m_nb - 1];
      i = nx_m_nb + 1;
      for (k = i; k < 101; k++) {
        amax = tmp[k - 1];
        if (amin < amax) {
          amin = amax;
        }
      }
    }
    for (i = 0; i < 100; i++) {
      tmp[i] /= amin;
    }
    if (dtw(last_tmp, tmp) > 0.1) {
      for (i = 0; i < 100; i++) {
        amax = tmp[i];
        last_tmp[i] = amax;
        resp_waveform[i] =
            (amax + resp_waveform[i] * counter) / (counter + 1.0);
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
