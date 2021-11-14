//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: BackgroundRemoval.h
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 14-Nov-2021 10:46:44
//

#ifndef BACKGROUNDREMOVAL_H
#define BACKGROUNDREMOVAL_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
class BackgroundRemoval {
public:
  BackgroundRemoval();
  ~BackgroundRemoval();
  static boolean_T check_moving(const double x[960], double thre);
  static void compute_thre(const double x[960], double taps[50], double iter,
                    double factor, boolean_T *init1_flag, double *thre);
};

#endif
//
// File trailer for BackgroundRemoval.h
//
// [EOF]
//
