//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// circshift.cpp
//
// Code generation for function 'circshift'
//

// Include files
#include "circshift.h"
#include "coder_array.h"
#include <cmath>

// Function Definitions
namespace coder {
void circshift(::coder::array<creal_T, 2U> &a, double p)
{
  array<creal_T, 2U> buffer;
  if ((a.size(1) != 0) && (a.size(1) != 1)) {
    int i;
    int loop_ub;
    int ns;
    boolean_T shiftright;
    if (p < 0.0) {
      ns = static_cast<int>(-p);
      shiftright = false;
    } else {
      ns = static_cast<int>(p);
      shiftright = true;
    }
    if (ns > a.size(1)) {
      ns -= ns / a.size(1) * a.size(1);
    }
    if (ns > (a.size(1) >> 1)) {
      ns = a.size(1) - ns;
      shiftright = !shiftright;
    }
    loop_ub =
        static_cast<int>(std::floor(static_cast<double>(a.size(1)) / 2.0));
    buffer.set_size(
        1, static_cast<int>(std::floor(static_cast<double>(a.size(1)) / 2.0)));
    for (i = 0; i < loop_ub; i++) {
      buffer[i].re = 0.0;
      buffer[i].im = 0.0;
    }
    loop_ub = a.size(1);
    if ((a.size(1) > 1) && (ns > 0)) {
      if (shiftright) {
        for (int k{0}; k < ns; k++) {
          buffer[k] = a[(k + loop_ub) - ns];
        }
        i = ns + 1;
        for (int k{loop_ub}; k >= i; k--) {
          a[k - 1] = a[(k - ns) - 1];
        }
        for (int k{0}; k < ns; k++) {
          a[k] = buffer[k];
        }
      } else {
        for (int k{0}; k < ns; k++) {
          buffer[k] = a[k];
        }
        i = a.size(1) - ns;
        for (int k{0}; k < i; k++) {
          a[k] = a[k + ns];
        }
        for (int k{0}; k < ns; k++) {
          a[(k + loop_ub) - ns] = buffer[k];
        }
      }
    }
  }
}

} // namespace coder

// End of code generation (circshift.cpp)
