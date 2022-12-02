/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: eml_setop.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 02-Dec-2022 11:02:26
 */

#ifndef EML_SETOP_H
#define EML_SETOP_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_do_vectors(const int a_data[], int a_size, const int b_data[],
                  int b_size, int c_data[], int *c_size, int ia_data[],
                  int *ia_size, int ib_data[], int *ib_size);

void do_vectors(const int a_data[], int a_size, const int b_data[], int b_size,
                int c_data[], int *c_size, int ia_data[], int *ia_size,
                int ib_data[], int *ib_size);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for eml_setop.h
 *
 * [EOF]
 */
