//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: quickselect.cpp
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 06-Nov-2021 14:27:28
//

// Include Files
#include "quickselect.h"

// Function Definitions
//
// Arguments    : const double v[5]
//                int ia
//                int ib
// Return Type  : int
//
namespace coder {
namespace internal {
int thirdOfFive(const double v[5], int ia, int ib)
{
  int im;
  if ((ia == ib) || (ia + 1 == ib)) {
    im = ia;
  } else if ((ia + 2 == ib) || (ia + 3 == ib)) {
    double v4;
    v4 = v[ia - 1];
    if (v4 < v[ia]) {
      double v5;
      v5 = v[ia + 1];
      if (v[ia] < v5) {
        im = ia + 1;
      } else if (v4 < v5) {
        im = ia + 2;
      } else {
        im = ia;
      }
    } else {
      double v5;
      v5 = v[ia + 1];
      if (v4 < v5) {
        im = ia;
      } else if (v[ia] < v5) {
        im = ia + 2;
      } else {
        im = ia + 1;
      }
    }
  } else {
    double v4;
    double v5;
    int b_j1;
    int j2;
    int j4;
    int j5;
    v4 = v[ia - 1];
    if (v4 < v[ia]) {
      v5 = v[ia + 1];
      if (v[ia] < v5) {
        b_j1 = ia;
        j2 = ia;
        im = ia + 2;
      } else if (v4 < v5) {
        b_j1 = ia;
        j2 = ia + 1;
        im = ia + 1;
      } else {
        b_j1 = ia + 2;
        j2 = ia - 1;
        im = ia + 1;
      }
    } else {
      v5 = v[ia + 1];
      if (v4 < v5) {
        b_j1 = ia + 1;
        j2 = ia - 1;
        im = ia + 2;
      } else if (v[ia] < v5) {
        b_j1 = ia + 1;
        j2 = ia + 1;
        im = ia;
      } else {
        b_j1 = ia + 2;
        j2 = ia;
        im = ia;
      }
    }
    j4 = ia;
    j5 = ia + 1;
    v4 = v[ia + 2];
    v5 = v[ia + 3];
    if (v5 < v4) {
      j4 = ia + 1;
      j5 = ia;
      v5 = v4;
      v4 = v[ia + 3];
    }
    if (v5 < v[b_j1 - 1]) {
      im = b_j1;
    } else if (v5 < v[j2]) {
      im = j5 + 3;
    } else if (v4 < v[j2]) {
      im = j2 + 1;
    } else if (v4 < v[im - 1]) {
      im = j4 + 3;
    }
  }
  return im;
}

} // namespace internal
} // namespace coder

//
// File trailer for quickselect.cpp
//
// [EOF]
//
