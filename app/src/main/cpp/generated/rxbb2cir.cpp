//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// rxbb2cir.cpp
//
// Code generation for function 'rxbb2cir'
//

// Include files
#include "rxbb2cir.h"
#include "ifft.h"
#include "coder_array.h"

// Function Definitions
void binary_expand_op(creal_T in1[480], const coder::array<creal_T, 1U> &in2)
{
  creal_T b_in1[480];
  int stride_0_0;
  stride_0_0 = (in2.size(0) != 1);
  for (int i{0}; i < 480; i++) {
    double d;
    double d1;
    d = in1[i].re;
    d1 = in1[i].im;
    b_in1[i].re = d * in2[i * stride_0_0].re - d1 * in2[i * stride_0_0].im;
    b_in1[i].im = d * in2[i * stride_0_0].im + d1 * in2[i * stride_0_0].re;
  }
  coder::ifft(b_in1, in1);
}

// End of code generation (rxbb2cir.cpp)
