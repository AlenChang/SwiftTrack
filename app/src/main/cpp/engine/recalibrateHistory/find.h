/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: find.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 25-Nov-2022 15:54:38
 */

#ifndef FIND_H
#define FIND_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void c_binary_expand_op(int in1_data[], int *in1_size, const double in2[2048],
                        const int in3_data[], const int *in3_size,
                        const double in4_data[], const int *in4_size);

void eml_find(const boolean_T x_data[], int x_size, int i_data[], int *i_size);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for find.h
 *
 * [EOF]
 */
