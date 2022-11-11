/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * findpeaks.h
 *
 * Code generation for function 'findpeaks'
 *
 */

#ifndef FINDPEAKS_H
#define FINDPEAKS_H

/* Include files */
#include "recalibrateHistory_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void c_findPeaksSeparatedByMoreThanM(const double y[2048], const int iPk_data[],
                                     int iPk_size, int idx_data[],
                                     int *idx_size);

void findExtents(const double y[2048], int iPk_data[], int *iPk_size,
                 const int iFin_data[], int iFin_size, const int iInf_data[],
                 int iInf_size, const int iInflect_data[], int iInflect_size,
                 double bPk_data[], int *bPk_size, emxArray_real_T *bxPk,
                 emxArray_real_T *byPk, emxArray_real_T *wxPk);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (findpeaks.h) */
