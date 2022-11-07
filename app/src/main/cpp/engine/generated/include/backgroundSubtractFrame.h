//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// backgroundSubtractFrame.h
//
// Code generation for function 'backgroundSubtractFrame'
//

#ifndef BACKGROUNDSUBTRACTFRAME_H
#define BACKGROUNDSUBTRACTFRAME_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
void backgroundSubtractFrame_init();

void binary_expand_op(creal_T in1[480], double in2,
                      const coder::array<creal_T, 1U> &in3,
                      const creal_T in4[480]);

void minus(creal_T in1[480], const coder::array<creal_T, 1U> &in2);

#endif
// End of code generation (backgroundSubtractFrame.h)
