//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// normalize.cpp
//
// Code generation for function 'normalize'
//

// Include files
#include "normalize.h"
#include "coder_array.h"
#include <cmath>
#include <math.h>

// Function Definitions
namespace coder {
void normalize(const ::coder::array<double, 2U> &a,
               ::coder::array<double, 2U> &n)
{
  array<int, 2U> r;
  double ex;
  double iMin;
  double r1;
  double r3;
  double sigma;
  int eint;
  int k;
  int last;
  last = a.size(1);
  if (a.size(1) <= 2) {
    if (a.size(1) == 1) {
      r1 = a[0];
    } else if (a[0] > a[a.size(1) - 1]) {
      r1 = a[a.size(1) - 1];
    } else {
      r1 = a[0];
    }
  } else {
    r1 = a[0];
    for (k = 2; k <= last; k++) {
      sigma = a[k - 1];
      if (r1 > sigma) {
        r1 = sigma;
      }
    }
  }
  last = a.size(1);
  if (a.size(1) <= 2) {
    if (a.size(1) == 1) {
      ex = a[0];
    } else if (a[0] < a[a.size(1) - 1]) {
      ex = a[a.size(1) - 1];
    } else {
      ex = a[0];
    }
  } else {
    ex = a[0];
    for (k = 2; k <= last; k++) {
      sigma = a[k - 1];
      if (ex < sigma) {
        ex = sigma;
      }
    }
  }
  sigma = std::fmax(0.0, r1);
  n.set_size(1, a.size(1));
  last = a.size(1);
  for (k = 0; k < last; k++) {
    n[k] = a[k] - sigma;
  }
  iMin = r1 - sigma;
  sigma = ex - sigma;
  frexp(std::fmax(std::abs(sigma), std::abs(iMin)), &eint);
  r1 = std::pow(2.0, static_cast<double>(eint) - 1.0);
  r3 = std::pow(2.0, std::trunc((static_cast<double>(eint) + 1.0) / 2.0) - 1.0);
  if (iMin == sigma) {
    last = n.size(1) - 1;
    k = 0;
    for (eint = 0; eint <= last; eint++) {
      k++;
    }
    r.set_size(1, k);
    for (eint = 0; eint <= last; eint++) {
      r[eint] = eint + 1;
    }
    last = r.size(1) - 1;
    for (k = 0; k <= last; k++) {
      n[r[k] - 1] = 0.0;
    }
  } else {
    double c1;
    c1 = sigma / r1;
    r1 = iMin / r1;
    ex = 1.0 / (sigma / r3 - iMin / r3) / r3;
    sigma = r3 * ((c1 * (0.0 / r3) - r1 * (1.0 / r3)) / (c1 - r1));
    n.set_size(1, n.size(1));
    last = n.size(1) - 1;
    for (k = 0; k <= last; k++) {
      n[k] = ex * n[k] + sigma;
    }
    last = n.size(1);
    for (eint = 0; eint < last; eint++) {
      if (n[eint] < 0.0) {
        n[eint] = 0.0;
      }
    }
    last = n.size(1);
    for (eint = 0; eint < last; eint++) {
      if (n[eint] > 1.0) {
        n[eint] = 1.0;
      }
    }
  }
}

} // namespace coder

// End of code generation (normalize.cpp)
