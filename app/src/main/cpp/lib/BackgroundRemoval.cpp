//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: BackgroundRemoval.cpp
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 14-Nov-2021 10:46:44
//

// Include Files
#include "BackgroundRemoval.h"
#include <cmath>

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
BackgroundRemoval::BackgroundRemoval()
{
}

//
// Arguments    : void
// Return Type  : void
//
BackgroundRemoval::~BackgroundRemoval()
{
  // (no terminate code required)
}

//
// Arguments    : const double x[960]
//                double thre
// Return Type  : boolean_T
//
boolean_T BackgroundRemoval::check_moving(const double x[960], double thre)
{
  double x_abs[480];
  double d;
  double ex;
  int k;
  for (k = 0; k < 480; k++) {
    int i;
    i = k << 1;
    d = x[i];
    ex = x[i + 1];
    x_abs[k] = std::sqrt(d * d + ex * ex);
  }
  ex = x_abs[0];
  for (k = 0; k < 479; k++) {
    d = x_abs[k + 1];
    if (ex < d) {
      ex = d;
    }
  }
  return ex > thre;
}

//
// Arguments    : const double x[960]
//                double taps[50]
//                double iter
//                double factor
//                boolean_T *init1_flag
//                double *thre
// Return Type  : void
//
void BackgroundRemoval::compute_thre(const double x[960], double taps[50],
                                     double iter, double factor,
                                     boolean_T *init1_flag, double *thre)
{
  double x_abs[480];
  double thre_tmp[50];
  double d;
  double ex;
  int k;
  for (k = 0; k < 480; k++) {
    int i;
    i = k << 1;
    d = x[i];
    ex = x[i + 1];
    x_abs[k] = std::sqrt(d * d + ex * ex);
  }
  ex = x_abs[0];
  for (k = 0; k < 479; k++) {
    d = x_abs[k + 1];
    if (ex < d) {
      ex = d;
    }
  }
  taps[static_cast<int>(iter) - 1] = ex;
  if (iter == 50.0) {
    double scale;
    double xbar;
    double y;
    *init1_flag = true;
    for (k = 0; k < 50; k++) {
      thre_tmp[k] = std::abs(taps[k]);
    }
    y = thre_tmp[0];
    xbar = thre_tmp[0];
    for (k = 0; k < 49; k++) {
      ex = thre_tmp[k + 1];
      y += ex;
      xbar += ex;
    }
    xbar /= 50.0;
    ex = 0.0;
    scale = 3.3121686421112381E-170;
    for (k = 0; k < 50; k++) {
      d = std::abs(thre_tmp[k] - xbar);
      if (d > scale) {
        double t;
        t = scale / d;
        ex = ex * t * t + 1.0;
        scale = d;
      } else {
        double t;
        t = d / scale;
        ex += t * t;
      }
    }
    ex = scale * std::sqrt(ex);
    ex /= 7.0;
    *thre = y / 50.0 + factor * ex;
  } else {
    *init1_flag = false;
    *thre = 0.0;
  }
}

//
// File trailer for BackgroundRemoval.cpp
//
// [EOF]
//
