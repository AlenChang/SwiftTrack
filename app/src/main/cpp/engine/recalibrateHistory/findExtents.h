/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: findExtents.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 07-Feb-2023 10:03:28
 */

#ifndef FINDEXTENTS_H
#define FINDEXTENTS_H

/* Include Files */
#include "recalibrateHistory_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void findExtents(const emxArray_real_T *y, const emxArray_real_T *x,
                 emxArray_int32_T *iPk, const emxArray_int32_T *iFin,
                 const emxArray_int32_T *iInf, const emxArray_int32_T *iInflect,
                 emxArray_real_T *bPk, emxArray_real_T *bxPk,
                 emxArray_real_T *byPk, emxArray_real_T *wxPk);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for findExtents.h
 *
 * [EOF]
 */
