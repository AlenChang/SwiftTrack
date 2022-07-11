//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// mod.cpp
//
// Code generation for function 'mod'
//

// Include files
#include "mod.h"
#include <cmath>

// Function Definitions
namespace coder {
double b_mod(double x, double y)
{
  double r;
  r = x;
  if (y == 0.0) {
    if (x == 0.0) {
      r = y;
    }
  } else if (x == 0.0) {
    r = 0.0 / y;
  } else {
    boolean_T rEQ0;
    r = std::fmod(x, y);
    rEQ0 = (r == 0.0);
    if ((!rEQ0) && (y > std::floor(y))) {
      double q;
      q = std::abs(x / y);
      rEQ0 = (std::abs(q - std::floor(q + 0.5)) <= 2.2204460492503131E-16 * q);
    }
    if (rEQ0) {
      r = 0.0;
    } else if ((x < 0.0) != (y < 0.0)) {
      r += y;
    }
  }
  return r;
}

} // namespace coder

// End of code generation (mod.cpp)
