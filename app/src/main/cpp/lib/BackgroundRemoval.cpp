//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: BackgroundRemoval.cpp
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 09-Nov-2021 20:58:03
//

// Include Files
#include "BackgroundRemoval.h"

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
// Arguments    : const double x_real[480]
//                const double x_imag[480]
//                double iter
//                double tapsel[2]
//                boolean_T *init1_flag
// Return Type  : void
//
void BackgroundRemoval::compute_thre(const double x_real[480],
                                     const double x_imag[480], double iter,
                                     double tapsel[2], boolean_T *init1_flag)
{
  double y[480];
  double d;
  double ex;
  int iindx;
  int k;
  for (k = 0; k < 480; k++) {
    d = x_real[k];
    ex = x_imag[k];
    y[k] = d * d + ex * ex;
  }
  ex = y[0];
  iindx = 0;
  for (k = 0; k < 479; k++) {
    d = y[k + 1];
    if (ex < d) {
      ex = d;
      iindx = k + 1;
    }
  }
  tapsel[0] = x_real[iindx];
  tapsel[1] = x_imag[iindx];
  if (iter == 100.0) {
    *init1_flag = true;
    //     thre = 4 * mean(abs(tapsel(1:100)));
  } else {
    *init1_flag = false;
  }
}

//
// File trailer for BackgroundRemoval.cpp
//
// [EOF]
//
