//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// Doppler_Compensation_01.h
//
// Code generation for function 'Doppler_Compensation_01'
//

#ifndef DOPPLER_COMPENSATION_01_H
#define DOPPLER_COMPENSATION_01_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct ATParameters;

// Function Declarations
void Doppler_Compensation_01_init();

void binary_expand_op(coder::array<creal_T, 1U> &in1,
                      const coder::array<creal_T, 2U> &in2, int in3);

void binary_expand_op(coder::array<creal_T, 1U> &in1,
                      const coder::array<creal_T, 1U> &in2,
                      const ATParameters *in3);

#endif
// End of code generation (Doppler_Compensation_01.h)
