/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sort.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Feb-2023 12:44:44
 */

/* Include Files */
#include "sort.h"
#include "recalibrateHistory_emxutil.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"

/* Function Definitions */
/*
 * Arguments    : emxArray_int32_T *x
 * Return Type  : void
 */
void sort(emxArray_int32_T *x)
{
  emxArray_int32_T *b_iwork;
  emxArray_int32_T *iidx;
  emxArray_int32_T *iwork;
  emxArray_int32_T *vwork;
  emxArray_int32_T *xwork;
  int b;
  int b_b;
  int b_j;
  int dim;
  int i;
  int j;
  int k;
  int nLeft;
  int vlen;
  int vstride;
  int *iidx_data;
  int *iwork_data;
  int *vwork_data;
  int *x_data;
  int *xwork_data;
  x_data = x->data;
  dim = 0;
  if (x->size[0] != 1) {
    dim = -1;
  }
  if (dim + 2 <= 1) {
    i = x->size[0];
  } else {
    i = 1;
  }
  vlen = i - 1;
  emxInit_int32_T(&vwork);
  nLeft = vwork->size[0];
  vwork->size[0] = i;
  emxEnsureCapacity_int32_T(vwork, nLeft);
  vwork_data = vwork->data;
  vstride = 1;
  for (k = 0; k <= dim; k++) {
    vstride *= x->size[0];
  }
  emxInit_int32_T(&iidx);
  emxInit_int32_T(&iwork);
  emxInit_int32_T(&xwork);
  emxInit_int32_T(&b_iwork);
  for (j = 0; j < vstride; j++) {
    int i3;
    for (k = 0; k <= vlen; k++) {
      vwork_data[k] = x_data[j + k * vstride];
    }
    i = iidx->size[0];
    iidx->size[0] = vwork->size[0];
    emxEnsureCapacity_int32_T(iidx, i);
    iidx_data = iidx->data;
    i3 = vwork->size[0];
    for (i = 0; i < i3; i++) {
      iidx_data[i] = 0;
    }
    if (vwork->size[0] != 0) {
      int idx4[4];
      int x4[4];
      int b_i;
      int i1;
      int i2;
      int nQuartets;
      x4[0] = 0;
      idx4[0] = 0;
      x4[1] = 0;
      idx4[1] = 0;
      x4[2] = 0;
      idx4[2] = 0;
      x4[3] = 0;
      idx4[3] = 0;
      i = iwork->size[0];
      iwork->size[0] = vwork->size[0];
      emxEnsureCapacity_int32_T(iwork, i);
      iwork_data = iwork->data;
      i3 = vwork->size[0];
      for (i = 0; i < i3; i++) {
        iwork_data[i] = 0;
      }
      i = xwork->size[0];
      xwork->size[0] = vwork->size[0];
      emxEnsureCapacity_int32_T(xwork, i);
      xwork_data = xwork->data;
      i3 = vwork->size[0];
      for (i = 0; i < i3; i++) {
        xwork_data[i] = 0;
      }
      nQuartets = vwork->size[0] >> 2;
      for (b_j = 0; b_j < nQuartets; b_j++) {
        signed char b_i1;
        signed char b_i2;
        signed char b_i3;
        signed char i4;
        b_i = b_j << 2;
        idx4[0] = b_i + 1;
        idx4[1] = b_i + 2;
        idx4[2] = b_i + 3;
        idx4[3] = b_i + 4;
        x4[0] = vwork_data[b_i];
        i3 = vwork_data[b_i + 1];
        x4[1] = i3;
        dim = vwork_data[b_i + 2];
        x4[2] = dim;
        nLeft = vwork_data[b_i + 3];
        x4[3] = nLeft;
        if (vwork_data[b_i] <= i3) {
          i1 = 1;
          i2 = 2;
        } else {
          i1 = 2;
          i2 = 1;
        }
        if (dim <= nLeft) {
          i3 = 3;
          dim = 4;
        } else {
          i3 = 4;
          dim = 3;
        }
        i = x4[i1 - 1];
        nLeft = x4[i3 - 1];
        if (i <= nLeft) {
          i = x4[i2 - 1];
          if (i <= nLeft) {
            b_i1 = (signed char)i1;
            b_i2 = (signed char)i2;
            b_i3 = (signed char)i3;
            i4 = (signed char)dim;
          } else if (i <= x4[dim - 1]) {
            b_i1 = (signed char)i1;
            b_i2 = (signed char)i3;
            b_i3 = (signed char)i2;
            i4 = (signed char)dim;
          } else {
            b_i1 = (signed char)i1;
            b_i2 = (signed char)i3;
            b_i3 = (signed char)dim;
            i4 = (signed char)i2;
          }
        } else {
          nLeft = x4[dim - 1];
          if (i <= nLeft) {
            if (x4[i2 - 1] <= nLeft) {
              b_i1 = (signed char)i3;
              b_i2 = (signed char)i1;
              b_i3 = (signed char)i2;
              i4 = (signed char)dim;
            } else {
              b_i1 = (signed char)i3;
              b_i2 = (signed char)i1;
              b_i3 = (signed char)dim;
              i4 = (signed char)i2;
            }
          } else {
            b_i1 = (signed char)i3;
            b_i2 = (signed char)dim;
            b_i3 = (signed char)i1;
            i4 = (signed char)i2;
          }
        }
        iidx_data[b_i] = idx4[b_i1 - 1];
        iidx_data[b_i + 1] = idx4[b_i2 - 1];
        iidx_data[b_i + 2] = idx4[b_i3 - 1];
        iidx_data[b_i + 3] = idx4[i4 - 1];
        vwork_data[b_i] = x4[b_i1 - 1];
        vwork_data[b_i + 1] = x4[b_i2 - 1];
        vwork_data[b_i + 2] = x4[b_i3 - 1];
        vwork_data[b_i + 3] = x4[i4 - 1];
      }
      dim = nQuartets << 2;
      nLeft = vwork->size[0] - dim;
      if (nLeft > 0) {
        signed char perm[4];
        for (k = 0; k < nLeft; k++) {
          i3 = dim + k;
          idx4[k] = i3 + 1;
          x4[k] = vwork_data[i3];
        }
        perm[1] = 0;
        perm[2] = 0;
        perm[3] = 0;
        if (nLeft == 1) {
          perm[0] = 1;
        } else if (nLeft == 2) {
          if (x4[0] <= x4[1]) {
            perm[0] = 1;
            perm[1] = 2;
          } else {
            perm[0] = 2;
            perm[1] = 1;
          }
        } else if (x4[0] <= x4[1]) {
          if (x4[1] <= x4[2]) {
            perm[0] = 1;
            perm[1] = 2;
            perm[2] = 3;
          } else if (x4[0] <= x4[2]) {
            perm[0] = 1;
            perm[1] = 3;
            perm[2] = 2;
          } else {
            perm[0] = 3;
            perm[1] = 1;
            perm[2] = 2;
          }
        } else if (x4[0] <= x4[2]) {
          perm[0] = 2;
          perm[1] = 1;
          perm[2] = 3;
        } else if (x4[1] <= x4[2]) {
          perm[0] = 2;
          perm[1] = 3;
          perm[2] = 1;
        } else {
          perm[0] = 3;
          perm[1] = 2;
          perm[2] = 1;
        }
        for (k = 0; k < nLeft; k++) {
          i1 = perm[k] - 1;
          i3 = dim + k;
          iidx_data[i3] = idx4[i1];
          vwork_data[i3] = x4[i1];
        }
      }
      dim = 2;
      if (vwork->size[0] > 1) {
        if (vwork->size[0] >= 256) {
          nQuartets = vwork->size[0] >> 8;
          for (b = 0; b < nQuartets; b++) {
            int b_xwork[256];
            int c_iwork[256];
            b_i = (b << 8) - 1;
            for (b_b = 0; b_b < 6; b_b++) {
              int bLen;
              int bLen2;
              bLen = 1 << (b_b + 2);
              bLen2 = bLen << 1;
              i = 256 >> (b_b + 3);
              for (k = 0; k < i; k++) {
                i3 = (b_i + k * bLen2) + 1;
                for (b_j = 0; b_j < bLen2; b_j++) {
                  dim = i3 + b_j;
                  c_iwork[b_j] = iidx_data[dim];
                  b_xwork[b_j] = vwork_data[dim];
                }
                i2 = 0;
                nLeft = bLen;
                dim = i3 - 1;
                int exitg1;
                do {
                  exitg1 = 0;
                  dim++;
                  if (b_xwork[i2] <= b_xwork[nLeft]) {
                    iidx_data[dim] = c_iwork[i2];
                    vwork_data[dim] = b_xwork[i2];
                    if (i2 + 1 < bLen) {
                      i2++;
                    } else {
                      exitg1 = 1;
                    }
                  } else {
                    iidx_data[dim] = c_iwork[nLeft];
                    vwork_data[dim] = b_xwork[nLeft];
                    if (nLeft + 1 < bLen2) {
                      nLeft++;
                    } else {
                      dim -= i2;
                      for (b_j = i2 + 1; b_j <= bLen; b_j++) {
                        i1 = dim + b_j;
                        iidx_data[i1] = c_iwork[b_j - 1];
                        vwork_data[i1] = b_xwork[b_j - 1];
                      }
                      exitg1 = 1;
                    }
                  }
                } while (exitg1 == 0);
              }
            }
          }
          dim = nQuartets << 8;
          i3 = vwork->size[0] - dim;
          if (i3 > 0) {
            merge_block(iidx, vwork, dim, i3, 2, iwork, xwork);
            xwork_data = xwork->data;
            iwork_data = iwork->data;
          }
          dim = 8;
        }
        i3 = iwork->size[0];
        i = b_iwork->size[0];
        b_iwork->size[0] = iwork->size[0];
        emxEnsureCapacity_int32_T(b_iwork, i);
        vwork_data = b_iwork->data;
        for (i = 0; i < i3; i++) {
          vwork_data[i] = iwork_data[i];
        }
        i3 = xwork->size[0];
        i = iwork->size[0];
        iwork->size[0] = xwork->size[0];
        emxEnsureCapacity_int32_T(iwork, i);
        iwork_data = iwork->data;
        for (i = 0; i < i3; i++) {
          iwork_data[i] = xwork_data[i];
        }
        merge_block(iidx, vwork, 0, vwork->size[0], dim, b_iwork, iwork);
        vwork_data = vwork->data;
      }
    }
    for (k = 0; k <= vlen; k++) {
      x_data[j + k * vstride] = vwork_data[k];
    }
  }
  emxFree_int32_T(&b_iwork);
  emxFree_int32_T(&xwork);
  emxFree_int32_T(&iwork);
  emxFree_int32_T(&iidx);
  emxFree_int32_T(&vwork);
}

/*
 * File trailer for sort.c
 *
 * [EOF]
 */
