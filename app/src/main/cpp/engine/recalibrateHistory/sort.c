/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sort.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 18-Nov-2022 21:41:51
 */

/* Include Files */
#include "sort.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : int x_data[]
 *                const int *x_size
 * Return Type  : void
 */
void sort(int x_data[], const int *x_size)
{
  int b_iwork_data[4096];
  int iidx_data[4096];
  int iwork_data[4096];
  int vwork_data[4096];
  int xwork_data[4096];
  int xwork[256];
  int x4[4];
  int b;
  int bLen;
  int bLen2;
  int b_b;
  int b_i;
  int b_j;
  int dim;
  int exitg1;
  int i;
  int i1;
  int i2;
  int i4;
  int j;
  int k;
  int nLeft;
  int nQuartets;
  int vlen;
  int vstride;
  int vwork_size;
  short iwork[256];
  short idx4[4];
  signed char perm[4];
  signed char b_i1;
  signed char b_i2;
  signed char b_i4;
  signed char i3;
  dim = 0;
  if (*x_size != 1) {
    dim = -1;
  }
  if (dim + 2 <= 1) {
    vwork_size = *x_size;
  } else {
    vwork_size = 1;
  }
  vlen = vwork_size - 1;
  vstride = 1;
  for (k = 0; k <= dim; k++) {
    vstride *= *x_size;
  }
  for (j = 0; j < vstride; j++) {
    for (k = 0; k <= vlen; k++) {
      vwork_data[k] = x_data[j + k * vstride];
    }
    if (vwork_size - 1 >= 0) {
      memset(&iidx_data[0], 0, (unsigned int)vwork_size * sizeof(int));
    }
    if (vwork_size != 0) {
      x4[0] = 0;
      idx4[0] = 0;
      x4[1] = 0;
      idx4[1] = 0;
      x4[2] = 0;
      idx4[2] = 0;
      x4[3] = 0;
      idx4[3] = 0;
      memset(&iwork_data[0], 0, (unsigned int)vwork_size * sizeof(int));
      memset(&xwork_data[0], 0, (unsigned int)vwork_size * sizeof(int));
      nQuartets = vwork_size >> 2;
      for (b_j = 0; b_j < nQuartets; b_j++) {
        i = b_j << 2;
        idx4[0] = (short)(i + 1);
        idx4[1] = (short)(i + 2);
        idx4[2] = (short)(i + 3);
        idx4[3] = (short)(i + 4);
        b_i = vwork_data[i];
        x4[0] = b_i;
        dim = vwork_data[i + 1];
        x4[1] = dim;
        i4 = vwork_data[i + 2];
        x4[2] = i4;
        nLeft = vwork_data[i + 3];
        x4[3] = nLeft;
        if (b_i <= dim) {
          i1 = 1;
          i2 = 2;
        } else {
          i1 = 2;
          i2 = 1;
        }
        if (i4 <= nLeft) {
          dim = 3;
          i4 = 4;
        } else {
          dim = 4;
          i4 = 3;
        }
        b_i = x4[i1 - 1];
        nLeft = x4[dim - 1];
        if (b_i <= nLeft) {
          b_i = x4[i2 - 1];
          if (b_i <= nLeft) {
            b_i1 = (signed char)i1;
            b_i2 = (signed char)i2;
            i3 = (signed char)dim;
            b_i4 = (signed char)i4;
          } else if (b_i <= x4[i4 - 1]) {
            b_i1 = (signed char)i1;
            b_i2 = (signed char)dim;
            i3 = (signed char)i2;
            b_i4 = (signed char)i4;
          } else {
            b_i1 = (signed char)i1;
            b_i2 = (signed char)dim;
            i3 = (signed char)i4;
            b_i4 = (signed char)i2;
          }
        } else {
          nLeft = x4[i4 - 1];
          if (b_i <= nLeft) {
            if (x4[i2 - 1] <= nLeft) {
              b_i1 = (signed char)dim;
              b_i2 = (signed char)i1;
              i3 = (signed char)i2;
              b_i4 = (signed char)i4;
            } else {
              b_i1 = (signed char)dim;
              b_i2 = (signed char)i1;
              i3 = (signed char)i4;
              b_i4 = (signed char)i2;
            }
          } else {
            b_i1 = (signed char)dim;
            b_i2 = (signed char)i4;
            i3 = (signed char)i1;
            b_i4 = (signed char)i2;
          }
        }
        iidx_data[i] = idx4[b_i1 - 1];
        iidx_data[i + 1] = idx4[b_i2 - 1];
        iidx_data[i + 2] = idx4[i3 - 1];
        iidx_data[i + 3] = idx4[b_i4 - 1];
        vwork_data[i] = x4[b_i1 - 1];
        vwork_data[i + 1] = x4[b_i2 - 1];
        vwork_data[i + 2] = x4[i3 - 1];
        vwork_data[i + 3] = x4[b_i4 - 1];
      }
      i4 = nQuartets << 2;
      nLeft = vwork_size - i4;
      if (nLeft > 0) {
        for (k = 0; k < nLeft; k++) {
          dim = i4 + k;
          idx4[k] = (short)(dim + 1);
          x4[k] = vwork_data[dim];
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
          dim = i4 + k;
          iidx_data[dim] = idx4[i1];
          vwork_data[dim] = x4[i1];
        }
      }
      dim = 2;
      if (vwork_size > 1) {
        if (vwork_size >= 256) {
          nQuartets = vwork_size >> 8;
          for (b = 0; b < nQuartets; b++) {
            i = (b << 8) - 1;
            for (b_b = 0; b_b < 6; b_b++) {
              bLen = 1 << (b_b + 2);
              bLen2 = bLen << 1;
              b_i = 256 >> (b_b + 3);
              for (k = 0; k < b_i; k++) {
                i4 = (i + k * bLen2) + 1;
                for (b_j = 0; b_j < bLen2; b_j++) {
                  dim = i4 + b_j;
                  iwork[b_j] = (short)iidx_data[dim];
                  xwork[b_j] = vwork_data[dim];
                }
                i2 = 0;
                nLeft = bLen;
                dim = i4 - 1;
                do {
                  exitg1 = 0;
                  dim++;
                  if (xwork[i2] <= xwork[nLeft]) {
                    iidx_data[dim] = iwork[i2];
                    vwork_data[dim] = xwork[i2];
                    if (i2 + 1 < bLen) {
                      i2++;
                    } else {
                      exitg1 = 1;
                    }
                  } else {
                    iidx_data[dim] = iwork[nLeft];
                    vwork_data[dim] = xwork[nLeft];
                    if (nLeft + 1 < bLen2) {
                      nLeft++;
                    } else {
                      dim -= i2;
                      for (b_j = i2 + 1; b_j <= bLen; b_j++) {
                        i1 = dim + b_j;
                        iidx_data[i1] = iwork[b_j - 1];
                        vwork_data[i1] = xwork[b_j - 1];
                      }
                      exitg1 = 1;
                    }
                  }
                } while (exitg1 == 0);
              }
            }
          }
          dim = nQuartets << 8;
          i4 = vwork_size - dim;
          if (i4 > 0) {
            merge_block(iidx_data, vwork_data, dim, i4, 2, iwork_data,
                        xwork_data);
          }
          dim = 8;
        }
        if (vwork_size - 1 >= 0) {
          memcpy(&b_iwork_data[0], &iwork_data[0],
                 (unsigned int)vwork_size * sizeof(int));
          memcpy(&iwork_data[0], &xwork_data[0],
                 (unsigned int)vwork_size * sizeof(int));
        }
        merge_block(iidx_data, vwork_data, 0, vwork_size, dim, b_iwork_data,
                    iwork_data);
      }
    }
    for (k = 0; k <= vlen; k++) {
      x_data[j + k * vstride] = vwork_data[k];
    }
  }
}

/*
 * File trailer for sort.c
 *
 * [EOF]
 */
