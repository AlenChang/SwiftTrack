/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: recalibrateHistory_types.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 25-Nov-2022 15:54:38
 */

#ifndef RECALIBRATEHISTORY_TYPES_H
#define RECALIBRATEHISTORY_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T
struct emxArray_real_T {
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};
#endif /* struct_emxArray_real_T */
#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T
typedef struct emxArray_real_T emxArray_real_T;
#endif /* typedef_emxArray_real_T */

#endif
/*
 * File trailer for recalibrateHistory_types.h
 *
 * [EOF]
 */
