/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: my_movmedian_terminate.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 05-Nov-2021 16:55:26
 */

/* Include Files */
#include "my_movmedian_terminate.h"
#include "my_movmedian.h"
#include "my_movmedian_data.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void my_movmedian_terminate(void)
{
  my_movmedian_free();
  isInitialized_my_movmedian = false;
}

/*
 * File trailer for my_movmedian_terminate.c
 *
 * [EOF]
 */
