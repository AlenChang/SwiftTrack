/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: eml_setop.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 02-Dec-2022 12:50:48
 */

#ifndef EML_SETOP_H
#define EML_SETOP_H

/* Include Files */
#include "recalibrateHistory_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_do_vectors(const emxArray_int32_T *a, const emxArray_int32_T *b,
                  emxArray_int32_T *c, emxArray_int32_T *ia,
                  emxArray_int32_T *ib);

void do_vectors(const emxArray_int32_T *a, const emxArray_int32_T *b,
                emxArray_int32_T *c, emxArray_int32_T *ia,
                emxArray_int32_T *ib);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for eml_setop.h
 *
 * [EOF]
 */
