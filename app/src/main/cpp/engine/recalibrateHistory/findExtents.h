/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: findExtents.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 02-Dec-2022 11:02:26
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
void findExtents(const double y[4096], int iPk_data[], int *iPk_size,
                 const int iFin_data[], int iFin_size, const int iInf_data[],
                 int iInf_size, const int iInflect_data[], int iInflect_size,
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
