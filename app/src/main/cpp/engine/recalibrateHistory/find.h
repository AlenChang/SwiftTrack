/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: find.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 13-Dec-2022 11:27:48
 */

#ifndef FIND_H
#define FIND_H

/* Include Files */
#include "recalibrateHistory_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void e_binary_expand_op(emxArray_int32_T *in1, const emxArray_real_T *in2,
                        const emxArray_int32_T *in3,
                        const emxArray_real_T *in4);

void eml_find(const emxArray_boolean_T *x, emxArray_int32_T *i);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for find.h
 *
 * [EOF]
 */
