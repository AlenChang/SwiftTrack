//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// processFramePipe.h
//
// Code generation for function 'processFramePipe'
//

#ifndef PROCESSFRAMEPIPE_H
#define PROCESSFRAMEPIPE_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void processFramePipe(const double frame[480], double Nzc_up,
                             double down_sample_factor,
                             coder::array<creal_T, 2U> &dist,
                             boolean_T *isValid);

void processFramePipe_init();

#endif
// End of code generation (processFramePipe.h)
