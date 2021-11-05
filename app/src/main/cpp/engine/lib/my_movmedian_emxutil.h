/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: my_movmedian_emxutil.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 05-Nov-2021 16:55:26
 */

#ifndef MY_MOVMEDIAN_EMXUTIL_H
#define MY_MOVMEDIAN_EMXUTIL_H

/* Include Files */
#include "my_movmedian_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);

extern void emxFree_real_T(emxArray_real_T **pEmxArray);

extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for my_movmedian_emxutil.h
 *
 * [EOF]
 */
