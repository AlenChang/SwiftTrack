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
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void findpeaks(const double Yin[2048], double Ypk_data[], int *Ypk_size,
               double Xpk_data[], int *Xpk_size);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (findpeaks.h) */
