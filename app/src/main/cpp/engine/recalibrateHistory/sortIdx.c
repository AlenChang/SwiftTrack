/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sortIdx.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 18-Nov-2022 21:41:51
 */

/* Include Files */
#include "sortIdx.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void merge(int idx_data[], int x_data[], int offset, int np, int nq,
                  int iwork_data[], int xwork_data[]);

/* Function Definitions */
/*
 * Arguments    : int idx_data[]
 *                int x_data[]
 *                int offset
 *                int np
 *                int nq
 *                int iwork_data[]
 *                int xwork_data[]
 * Return Type  : void
 */
static void merge(int idx_data[], int x_data[], int offset, int np, int nq,
                  int iwork_data[], int xwork_data[])
{
  int exitg1;
  int iout;
  int j;
  int n_tmp;
  int p;
  int q;
  if (nq != 0) {
    n_tmp = np + nq;
    for (j = 0; j < n_tmp; j++) {
      iout = offset + j;
      iwork_data[j] = idx_data[iout];
      xwork_data[j] = x_data[iout];
    }
    p = 0;
    q = np;
    iout = offset - 1;
    do {
      exitg1 = 0;
      iout++;
      if (xwork_data[p] <= xwork_data[q]) {
        idx_data[iout] = iwork_data[p];
        x_data[iout] = xwork_data[p];
        if (p + 1 < np) {
          p++;
        } else {
          exitg1 = 1;
        }
      } else {
        idx_data[iout] = iwork_data[q];
        x_data[iout] = xwork_data[q];
        if (q + 1 < n_tmp) {
          q++;
        } else {
          q = iout - p;
          for (j = p + 1; j <= np; j++) {
            iout = q + j;
            idx_data[iout] = iwork_data[j - 1];
            x_data[iout] = xwork_data[j - 1];
          }
          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
  }
}

/*
 * Arguments    : int idx_data[]
 *                int x_data[]
 *                int offset
 *                int n
 *                int preSortLevel
 *                int iwork_data[]
 *                int xwork_data[]
 * Return Type  : void
 */
void merge_block(int idx_data[], int x_data[], int offset, int n,
                 int preSortLevel, int iwork_data[], int xwork_data[])
{
  int bLen;
  int k;
  int nPairs;
  int nTail;
  int tailOffset;
  nPairs = n >> preSortLevel;
  bLen = 1 << preSortLevel;
  while (nPairs > 1) {
    if ((nPairs & 1) != 0) {
      nPairs--;
      tailOffset = bLen * nPairs;
      nTail = n - tailOffset;
      if (nTail > bLen) {
        merge(idx_data, x_data, offset + tailOffset, bLen, nTail - bLen,
              iwork_data, xwork_data);
      }
    }
    tailOffset = bLen << 1;
    nPairs >>= 1;
    nTail = (unsigned short)nPairs;
    for (k = 0; k < nTail; k++) {
      merge(idx_data, x_data, offset + k * tailOffset, bLen, bLen, iwork_data,
            xwork_data);
    }
    bLen = tailOffset;
  }
  if (n > bLen) {
    merge(idx_data, x_data, offset, bLen, n - bLen, iwork_data, xwork_data);
  }
}

/*
 * File trailer for sortIdx.c
 *
 * [EOF]
 */
