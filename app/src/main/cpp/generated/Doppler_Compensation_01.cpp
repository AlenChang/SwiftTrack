//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// Doppler_Compensation_01.cpp
//
// Code generation for function 'Doppler_Compensation_01'
//

// Include files
#include "Doppler_Compensation_01.h"
#include "fft.h"
#include "ifft.h"
#include "processFramePipe_data.h"
#include "processFramePipe_internal_types.h"
#include "coder_array.h"

// Function Definitions
void Doppler_Compensation_01_init()
{
  b_frame_counter = 1.0;
  last_idmax = 0.0;
  vest = 0.0;
}

void binary_expand_op(coder::array<creal_T, 1U> &in1,
                      const coder::array<creal_T, 2U> &in2, int in3)
{
  coder::array<creal_T, 1U> b_in2;
  int i;
  int i1;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  i = in2.size(0);
  if (in1.size(0) == 1) {
    i1 = i;
  } else {
    i1 = in1.size(0);
  }
  b_in2.set_size(i1);
  stride_0_0 = (i != 1);
  stride_1_0 = (in1.size(0) != 1);
  if (in1.size(0) == 1) {
    loop_ub = i;
  } else {
    loop_ub = in1.size(0);
  }
  for (i = 0; i < loop_ub; i++) {
    double d;
    double d1;
    double d2;
    double d3;
    int i2;
    i1 = i * stride_0_0;
    i2 = i * stride_1_0;
    d = in2[i1 + in2.size(0) * in3].re;
    d1 = in1[i2].im;
    d2 = in2[i1 + in2.size(0) * in3].im;
    d3 = in1[i2].re;
    b_in2[i].re = d * d3 - d2 * d1;
    b_in2[i].im = d * d1 + d2 * d3;
  }
  coder::fft(b_in2, in1);
}

void binary_expand_op(coder::array<creal_T, 1U> &in1,
                      const coder::array<creal_T, 1U> &in2,
                      const ATParameters *in3)
{
  coder::array<creal_T, 1U> b_in1;
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  int stride_2_0;
  if (in3->inv_freq.size(0) == 1) {
    if (in2.size(0) == 1) {
      i = in1.size(0);
    } else {
      i = in2.size(0);
    }
  } else {
    i = in3->inv_freq.size(0);
  }
  b_in1.set_size(i);
  stride_0_0 = (in1.size(0) != 1);
  stride_1_0 = (in2.size(0) != 1);
  stride_2_0 = (in3->inv_freq.size(0) != 1);
  if (in3->inv_freq.size(0) == 1) {
    if (in2.size(0) == 1) {
      loop_ub = in1.size(0);
    } else {
      loop_ub = in2.size(0);
    }
  } else {
    loop_ub = in3->inv_freq.size(0);
  }
  for (i = 0; i < loop_ub; i++) {
    double b_in1_re_tmp;
    double c_in1_re_tmp;
    double d;
    int in1_re_tmp;
    in1_re_tmp = i * stride_1_0;
    b_in1_re_tmp = in2[in1_re_tmp].im;
    c_in1_re_tmp = in2[in1_re_tmp].re;
    d = in3->inv_freq[i * stride_2_0];
    b_in1[i].re = d * (in1[i * stride_0_0].re * c_in1_re_tmp -
                       in1[i * stride_0_0].im * b_in1_re_tmp);
    b_in1[i].im = d * (in1[i * stride_0_0].re * b_in1_re_tmp +
                       in1[i * stride_0_0].im * c_in1_re_tmp);
  }
  coder::ifft(b_in1, in1);
}

// End of code generation (Doppler_Compensation_01.cpp)
