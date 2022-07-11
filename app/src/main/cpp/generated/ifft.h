//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// ifft.h
//
// Code generation for function 'ifft'
//

#ifndef IFFT_H
#define IFFT_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
void ifft(const creal_T x[480], creal_T y[480]);

void ifft(const ::coder::array<creal_T, 1U> &x, ::coder::array<creal_T, 1U> &y);

} // namespace coder

#endif
// End of code generation (ifft.h)