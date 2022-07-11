//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// processFramePipe_initialize.cpp
//
// Code generation for function 'processFramePipe_initialize'
//

// Include files
#include "processFramePipe_initialize.h"
#include "Doppler_Compensation_01.h"
#include "backgroundSubtractFrame.h"
#include "calibrateFrame.h"
#include "cir2dist.h"
#include "offerRxUpsample.h"
#include "pass2baseFrame.h"
#include "processFramePipe.h"
#include "processFramePipe_data.h"

// Function Definitions
void processFramePipe_initialize()
{
  frame_counter_not_empty_init();
  processFramePipe_init();
  pass2baseFrame_init();
  calibrateFrame_init();
  synchronizeFrame_init();
  backgroundSubtractFrame_init();
  offerRxUpsample_init();
  Doppler_Compensation_01_init();
  cir2dist_init();
  isInitialized_processFramePipe = true;
}

// End of code generation (processFramePipe_initialize.cpp)
