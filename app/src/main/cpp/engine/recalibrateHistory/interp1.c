/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: interp1.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 18-Nov-2022 21:41:51
 */

/* Include Files */
#include "interp1.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double varargin_1_data[]
 *                const int varargin_1_size[2]
 *                const double varargin_2_data[]
 *                int varargin_2_size
 *                double Vq[100]
 * Return Type  : void
 */
void interp1(const double varargin_1_data[], const int varargin_1_size[2],
             const double varargin_2_data[], int varargin_2_size,
             double Vq[100])
{
  static const double dv[100] = {0.0,
                                 0.010101010101010102,
                                 0.020202020202020204,
                                 0.030303030303030304,
                                 0.040404040404040407,
                                 0.050505050505050511,
                                 0.060606060606060608,
                                 0.070707070707070718,
                                 0.080808080808080815,
                                 0.090909090909090912,
                                 0.10101010101010102,
                                 0.11111111111111112,
                                 0.12121212121212122,
                                 0.13131313131313133,
                                 0.14141414141414144,
                                 0.15151515151515152,
                                 0.16161616161616163,
                                 0.17171717171717174,
                                 0.18181818181818182,
                                 0.19191919191919193,
                                 0.20202020202020204,
                                 0.21212121212121213,
                                 0.22222222222222224,
                                 0.23232323232323235,
                                 0.24242424242424243,
                                 0.25252525252525254,
                                 0.26262626262626265,
                                 0.27272727272727276,
                                 0.28282828282828287,
                                 0.29292929292929293,
                                 0.30303030303030304,
                                 0.31313131313131315,
                                 0.32323232323232326,
                                 0.33333333333333337,
                                 0.34343434343434348,
                                 0.35353535353535359,
                                 0.36363636363636365,
                                 0.37373737373737376,
                                 0.38383838383838387,
                                 0.393939393939394,
                                 0.40404040404040409,
                                 0.4141414141414142,
                                 0.42424242424242425,
                                 0.43434343434343436,
                                 0.44444444444444448,
                                 0.45454545454545459,
                                 0.4646464646464647,
                                 0.47474747474747481,
                                 0.48484848484848486,
                                 0.494949494949495,
                                 0.50505050505050508,
                                 0.51515151515151525,
                                 0.5252525252525253,
                                 0.53535353535353536,
                                 0.54545454545454553,
                                 0.55555555555555558,
                                 0.56565656565656575,
                                 0.5757575757575758,
                                 0.58585858585858586,
                                 0.595959595959596,
                                 0.60606060606060608,
                                 0.61616161616161624,
                                 0.6262626262626263,
                                 0.63636363636363646,
                                 0.64646464646464652,
                                 0.65656565656565657,
                                 0.66666666666666674,
                                 0.6767676767676768,
                                 0.686868686868687,
                                 0.696969696969697,
                                 0.70707070707070718,
                                 0.71717171717171724,
                                 0.72727272727272729,
                                 0.73737373737373746,
                                 0.74747474747474751,
                                 0.75757575757575768,
                                 0.76767676767676774,
                                 0.77777777777777779,
                                 0.787878787878788,
                                 0.797979797979798,
                                 0.80808080808080818,
                                 0.81818181818181823,
                                 0.8282828282828284,
                                 0.83838383838383845,
                                 0.84848484848484851,
                                 0.85858585858585867,
                                 0.86868686868686873,
                                 0.8787878787878789,
                                 0.888888888888889,
                                 0.89898989898989912,
                                 0.90909090909090917,
                                 0.91919191919191923,
                                 0.92929292929292939,
                                 0.93939393939393945,
                                 0.94949494949494961,
                                 0.95959595959595967,
                                 0.96969696969696972,
                                 0.97979797979797989,
                                 0.98989898989899,
                                 1.0};
  double x_data[2048];
  double y_data[2048];
  double xtmp;
  int exitg1;
  int high_i;
  int k;
  int low_ip1;
  int mid_i;
  int nx;
  int x_size_idx_1;
  if (varargin_2_size - 1 >= 0) {
    memcpy(&y_data[0], &varargin_2_data[0],
           (unsigned int)varargin_2_size * sizeof(double));
  }
  x_size_idx_1 = varargin_1_size[1];
  high_i = varargin_1_size[1];
  if (high_i - 1 >= 0) {
    memcpy(&x_data[0], &varargin_1_data[0],
           (unsigned int)high_i * sizeof(double));
  }
  nx = varargin_1_size[1] - 1;
  k = 0;
  do {
    exitg1 = 0;
    if (k <= nx) {
      if (rtIsNaN(varargin_1_data[k])) {
        exitg1 = 1;
      } else {
        k++;
      }
    } else {
      if (varargin_1_data[1] < varargin_1_data[0]) {
        low_ip1 = (nx + 1) >> 1;
        for (mid_i = 0; mid_i < low_ip1; mid_i++) {
          xtmp = x_data[mid_i];
          high_i = nx - mid_i;
          x_data[mid_i] = x_data[high_i];
          x_data[high_i] = xtmp;
        }
        if (varargin_2_size > 1) {
          low_ip1 = varargin_2_size >> 1;
          for (k = 0; k < low_ip1; k++) {
            xtmp = y_data[k];
            high_i = (varargin_2_size - k) - 1;
            y_data[k] = y_data[high_i];
            y_data[high_i] = xtmp;
          }
        }
      }
      for (k = 0; k < 100; k++) {
        Vq[k] = rtNaN;
        xtmp = dv[k];
        if ((!(xtmp > x_data[x_size_idx_1 - 1])) && (!(xtmp < x_data[0]))) {
          high_i = x_size_idx_1;
          nx = 1;
          low_ip1 = 2;
          while (high_i > low_ip1) {
            mid_i = (nx >> 1) + (high_i >> 1);
            if (((nx & 1) == 1) && ((high_i & 1) == 1)) {
              mid_i++;
            }
            if (dv[k] >= x_data[mid_i - 1]) {
              nx = mid_i;
              low_ip1 = mid_i + 1;
            } else {
              high_i = mid_i;
            }
          }
          xtmp = x_data[nx - 1];
          xtmp = (dv[k] - xtmp) / (x_data[nx] - xtmp);
          if (xtmp == 0.0) {
            Vq[k] = y_data[nx - 1];
          } else if (xtmp == 1.0) {
            Vq[k] = y_data[nx];
          } else if (y_data[nx - 1] == y_data[nx]) {
            Vq[k] = y_data[nx - 1];
          } else {
            Vq[k] = (1.0 - xtmp) * y_data[nx - 1] + xtmp * y_data[nx];
          }
        }
      }
      exitg1 = 1;
    }
  } while (exitg1 == 0);
}

/*
 * File trailer for interp1.c
 *
 * [EOF]
 */
