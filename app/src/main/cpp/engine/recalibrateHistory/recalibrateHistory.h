/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * recalibrateHistory.h
 *
 * Code generation for function 'recalibrateHistory'
 *
 */

#ifndef RECALIBRATEHISTORY_H
#define RECALIBRATEHISTORY_H

/* Include files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void recalibrateHistory(double hist[2048], double moving_thre,
                               double hist_out[2048],
                               boolean_T is_body_moving[2048],
                               double pks_data[], int pks_size[1],
                               double locs_data[], int locs_size[1]);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (recalibrateHistory.h) */
