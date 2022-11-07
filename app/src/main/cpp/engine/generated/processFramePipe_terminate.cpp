//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// processFramePipe_terminate.cpp
//
// Code generation for function 'processFramePipe_terminate'
//

// Include files
#include "processFramePipe_terminate.h"
#include "StandaloneFFTW3Interface.h"
#include "processFramePipe_data.h"

// Function Declarations
static void customAtExit();

// Function Definitions
static void customAtExit()
{
  coder::fftw::StandaloneFFTW3Interface::fftwCleanUp();
}

void processFramePipe_terminate()
{
  customAtExit();
  isInitialized_processFramePipe = false;
}

// End of code generation (processFramePipe_terminate.cpp)
