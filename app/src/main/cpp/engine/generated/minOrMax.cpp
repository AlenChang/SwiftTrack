//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// minOrMax.cpp
//
// Code generation for function 'minOrMax'
//

// Include files
#include "minOrMax.h"

// Function Definitions
namespace coder {
namespace internal {
void maximum(const double x_data[], int x_size, double *ex, int *idx)
{
  if (x_size <= 2) {
    if (x_size == 1) {
      *ex = x_data[0];
      *idx = 1;
    } else {
      *ex = x_data[x_size - 1];
      if (x_data[0] < *ex) {
        *idx = x_size;
      } else {
        *ex = x_data[0];
        *idx = 1;
      }
    }
  } else {
    *ex = x_data[0];
    *idx = 1;
    for (int k{2}; k <= x_size; k++) {
      double d;
      d = x_data[k - 1];
      if (*ex < d) {
        *ex = d;
        *idx = k;
      }
    }
  }
}

} // namespace internal
} // namespace coder

// End of code generation (minOrMax.cpp)
