//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// pass2baseFrame.cpp
//
// Code generation for function 'pass2baseFrame'
//

// Include files
#include "pass2baseFrame.h"
#include "processFramePipe_data.h"
#include "coder_array.h"

// Function Definitions
void pass2baseFrame_init()
{
  delta_phase_not_empty = false;
}

void times(creal_T in1[480], const coder::array<creal_T, 1U> &in2)
{
  int stride_0_0;
  stride_0_0 = (in2.size(0) != 1);
  for (int i{0}; i < 480; i++) {
    double b_re_tmp;
    double re_tmp;
    re_tmp = in1[i].re;
    b_re_tmp = in1[i].im;
    in1[i].re =
        re_tmp * in2[i * stride_0_0].re - b_re_tmp * in2[i * stride_0_0].im;
    in1[i].im =
        re_tmp * in2[i * stride_0_0].im + b_re_tmp * in2[i * stride_0_0].re;
  }
}

// End of code generation (pass2baseFrame.cpp)
