//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// calibrateFrame.h
//
// Code generation for function 'calibrateFrame'
//

#ifndef CALIBRATEFRAME_H
#define CALIBRATEFRAME_H

// Include files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct ATParameters;

// Function Declarations
void calibrateFrame(const creal_T rxbb[480], ATParameters *params);

void calibrateFrame_init();

void synchronizeFrame_init();

#endif
// End of code generation (calibrateFrame.h)
