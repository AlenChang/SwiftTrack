//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// StandaloneFFTW3Interface.cpp
//
// Code generation for function 'StandaloneFFTW3Interface'
//

// Include files
#include "StandaloneFFTW3Interface.h"
#include "fftw3.h"

// Function Definitions
namespace coder {
namespace fftw {
void StandaloneFFTW3Interface::fftwCleanUp()
{
  fftw_cleanup();
}

} // namespace fftw
} // namespace coder

// End of code generation (StandaloneFFTW3Interface.cpp)
