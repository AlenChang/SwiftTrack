/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: findpeaks.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 02-Dec-2022 11:02:26
 */

#ifndef FINDPEAKS_H
#define FINDPEAKS_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void findpeaks(const double Yin[4096], double Ypk_data[], int *Ypk_size,
               double Xpk_data[], int *Xpk_size);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for findpeaks.h
 *
 * [EOF]
 */
