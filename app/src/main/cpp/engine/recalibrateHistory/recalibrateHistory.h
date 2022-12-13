/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: recalibrateHistory.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Dec-2022 11:27:48
 */

#ifndef RECALIBRATEHISTORY_H
#define RECALIBRATEHISTORY_H

/* Include Files */
#include "recalibrateHistory_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void recalibrateHistory(emxArray_real_T *hist, double moving_thre,
                               double last_tmp[100], double resp_waveform[100],
                               emxArray_real_T *hist_out,
                               emxArray_boolean_T *is_body_moving,
                               double tmp[100], boolean_T *new_waveform,
                               double *resp_freq);

void recalibrateHistory_init(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for recalibrateHistory.h
 *
 * [EOF]
 */
