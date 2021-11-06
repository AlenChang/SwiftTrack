//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: codeGen.h
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 06-Nov-2021 14:27:28
//

#ifndef CODEGEN_H
#define CODEGEN_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
class codeGen {
public:
  codeGen();
  ~codeGen();
  void genWindow(double win[480]);
  double mvMedian(double x, double buffer[5], double *id);
};

#endif
//
// File trailer for codeGen.h
//
// [EOF]
//
