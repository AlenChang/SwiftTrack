//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// processFramePipe_internal_types.h
//
// Code generation for function 'processFramePipe'
//

#ifndef PROCESSFRAMEPIPE_INTERNAL_TYPES_H
#define PROCESSFRAMEPIPE_INTERNAL_TYPES_H

// Include files
#include "processFramePipe_types.h"
#include "rtwtypes.h"
#include "coder_array.h"

// Type Definitions
struct struct_T {
  coder::array<double, 1U> kaiser;
};

struct ATParameters {
  double Nzc_up;
  double BW;
  double fs;
  double fc;
  double c;
  double Nzc;
  double U;
  double T;
  coder::array<creal_T, 2U> zc_up;
  double down_sample_factor;
  double up_sample_factor;
  double read_channel;
  double start_cut_time_duration;
  double end_cut_time_duration;
  boolean_T PLOT;
  double idlow;
  double idhigh;
  boolean_T COMP_FREQ;
  coder::array<double, 1U> inv_freq;
  struct_T win;
  double doppler_search_step_size;
  double cali_time;
  double time_counter;
  double frame_counter;
  boolean_T caliSucceeded;
  boolean_T syncSucceeded;
  boolean_T backgroundInitSucceeded;
  boolean_T bufferInitSucceeded;
  double sync_id;
  double movingThreshold;
  double movingThresholdFacor;
  boolean_T current_moving_frame;
};

#endif
// End of code generation (processFramePipe_internal_types.h)
