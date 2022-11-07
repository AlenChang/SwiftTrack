//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: NoiseSupression.h
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 12-Nov-2021 14:22:45
//

#ifndef NOISESUPRESSION_H
#define NOISESUPRESSION_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
class NoiseSupression {
public:
  NoiseSupression();
  ~NoiseSupression();
  static void genWindow(double win[480]);
  static double lowPass6_15_60_100(double x, double taps[22]);
  static double mvMedian(double x, double buffer[5], double *id);
};

#endif
//
// File trailer for NoiseSupression.h
//
// [EOF]
//
