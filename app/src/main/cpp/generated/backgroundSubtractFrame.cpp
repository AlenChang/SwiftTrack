//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// backgroundSubtractFrame.cpp
//
// Code generation for function 'backgroundSubtractFrame'
//

// Include files
#include "backgroundSubtractFrame.h"
#include "processFramePipe_data.h"
#include "coder_array.h"

// Function Definitions
void backgroundSubtractFrame_init()
{
  static_vec_not_empty = false;
}

void binary_expand_op(creal_T in1[480], double in2,
                      const coder::array<creal_T, 1U> &in3,
                      const creal_T in4[480])
{
  int stride_0_0;
  stride_0_0 = (in3.size(0) != 1);
  for (int i{0}; i < 480; i++) {
    in1[i].re = in2 * in3[i * stride_0_0].re + in4[i].re;
    in1[i].im = in2 * in3[i * stride_0_0].im + in4[i].im;
  }
}

void minus(creal_T in1[480], const coder::array<creal_T, 1U> &in2)
{
  int stride_0_0;
  stride_0_0 = (in2.size(0) != 1);
  for (int i{0}; i < 480; i++) {
    in1[i].re -= in2[i * stride_0_0].re;
    in1[i].im -= in2[i * stride_0_0].im;
  }
}

// End of code generation (backgroundSubtractFrame.cpp)
