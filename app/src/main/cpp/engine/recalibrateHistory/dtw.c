/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: dtw.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 29-Jan-2023 18:06:51
 */

/* Include Files */
#include "dtw.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const double x[100]
 *                const double y[100]
 * Return Type  : double
 */
double dtw(const double x[100], const double y[100])
{
  double C[10000];
  double sumz;
  int ix;
  int iy;
  int iz;
  sumz = 0.0;
  for (ix = 0; ix < 100; ix++) {
    sumz += fabs(x[ix] - y[0]);
    C[ix] = sumz;
  }
  iz = 0;
  for (iy = 0; iy < 99; iy++) {
    double pdist;
    sumz = C[iz];
    pdist = C[iz];
    for (ix = 0; ix < 100; ix++) {
      double b_nmin_tmp;
      int nmin_tmp;
      nmin_tmp = iz + ix;
      b_nmin_tmp = C[nmin_tmp];
      if (b_nmin_tmp < sumz) {
        sumz = C[nmin_tmp];
      }
      if (pdist < sumz) {
        sumz = pdist;
      }
      pdist = sumz + fabs(x[ix] - y[iy + 1]);
      C[nmin_tmp + 100] = pdist;
      sumz = b_nmin_tmp;
    }
    iz += 100;
  }
  return C[9999];
}

/*
 * File trailer for dtw.c
 *
 * [EOF]
 */
