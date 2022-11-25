/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: recalibrateHistory_initialize.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 25-Nov-2022 15:54:38
 */

/* Include Files */
#include "recalibrateHistory_initialize.h"
#include "recalibrateHistory.h"
#include "recalibrateHistory_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void recalibrateHistory_initialize(void)
{
  recalibrateHistory_init();
  isInitialized_recalibrateHistory = true;
}

/*
 * File trailer for recalibrateHistory_initialize.c
 *
 * [EOF]
 */
