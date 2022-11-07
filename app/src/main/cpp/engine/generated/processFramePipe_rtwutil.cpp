//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// processFramePipe_rtwutil.cpp
//
// Code generation for function 'processFramePipe_rtwutil'
//

// Include files
#include "processFramePipe_rtwutil.h"
#include <cmath>

// Function Definitions
double rt_hypotd(double u0, double u1)
{
  double a;
  double b;
  double y;
  a = std::abs(u0);
  b = std::abs(u1);
  if (a < b) {
    a /= b;
    y = b * std::sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * std::sqrt(b * b + 1.0);
  } else {
    y = a * 1.4142135623730951;
  }
  return y;
}

// End of code generation (processFramePipe_rtwutil.cpp)
