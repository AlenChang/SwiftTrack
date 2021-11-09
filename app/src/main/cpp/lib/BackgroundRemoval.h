//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: BackgroundRemoval.h
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 09-Nov-2021 20:58:03
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
  static void compute_thre(const double x_real[480], const double x_imag[480],
                    double iter, double tapsel[2], boolean_T *init1_flag);
};

#endif
//
// File trailer for BackgroundRemoval.h
//
// [EOF]
//
