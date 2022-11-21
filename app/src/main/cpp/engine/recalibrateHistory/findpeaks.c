/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: findpeaks.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 18-Nov-2022 21:41:51
 */

/* Include Files */
#include "findpeaks.h"
#include "findExtents.h"
#include "recalibrateHistory_emxutil.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Declarations */
static void binary_expand_op(boolean_T in1_data[], int *in1_size,
                             const short in2_data[], const int *in2_size,
                             const int in3_data[], const int in4_data[],
                             int in5);

/* Function Definitions */
/*
 * Arguments    : boolean_T in1_data[]
 *                int *in1_size
 *                const short in2_data[]
 *                const int *in2_size
 *                const int in3_data[]
 *                const int in4_data[]
 *                int in5
 * Return Type  : void
 */
static void binary_expand_op(boolean_T in1_data[], int *in1_size,
                             const short in2_data[], const int *in2_size,
                             const int in3_data[], const int in4_data[],
                             int in5)
{
  int b_in1_size;
  int i;
  int i1;
  int in1_tmp;
  int stride_0_0;
  int stride_1_0;
  boolean_T b_in1_data[4096];
  i = (short)((short)(in3_data[in4_data[in5] - 1] - 1) + 1);
  if (*in2_size == 1) {
    b_in1_size = *in1_size;
  } else {
    b_in1_size = *in2_size;
  }
  stride_0_0 = (*in1_size != 1);
  stride_1_0 = (*in2_size != 1);
  if (*in2_size != 1) {
    *in1_size = *in2_size;
  }
  for (i1 = 0; i1 < *in1_size; i1++) {
    in1_tmp = in2_data[i1 * stride_1_0];
    b_in1_data[i1] = (in1_data[i1 * stride_0_0] ||
                      ((in1_tmp >= i - 100) && (in1_tmp <= i + 100)));
  }
  *in1_size = b_in1_size;
  if (b_in1_size - 1 >= 0) {
    memcpy(&in1_data[0], &b_in1_data[0],
           (unsigned int)b_in1_size * sizeof(boolean_T));
  }
}

/*
 * Arguments    : const double Yin[2048]
 *                double Ypk_data[]
 *                int *Ypk_size
 *                double Xpk_data[]
 *                int *Xpk_size
 * Return Type  : void
 */
void findpeaks(const double Yin[2048], double Ypk_data[], int *Ypk_size,
               double Xpk_data[], int *Xpk_size)
{
  static double bPk_data[4096];
  static int b_iPk_data[4096];
  static int idx_data[4096];
  static int iwork_data[4096];
  static int sortIdx_data[4096];
  emxArray_real_T *bxPk;
  emxArray_real_T *byPk;
  emxArray_real_T *wxPk;
  double yk;
  double ykfirst;
  int iFinite_data[2048];
  int iInfinite_data[2048];
  int iInflect_data[2048];
  int b_i;
  int i;
  int k;
  int kEnd;
  int kfirst;
  int loop_ub;
  int nInf;
  int nInflect;
  int nPk;
  int q;
  int qEnd;
  short locs_temp_data[4096];
  short tmp_data[4096];
  short iPk_data[2048];
  short i1;
  char dir;
  char previousdir;
  boolean_T idelete_data[4096];
  boolean_T isinfyk;
  boolean_T isinfykfirst;
  nPk = -1;
  nInf = -1;
  nInflect = -1;
  dir = 'n';
  kfirst = 0;
  ykfirst = rtInf;
  isinfykfirst = true;
  for (k = 0; k < 2048; k++) {
    yk = Yin[k];
    if (rtIsNaN(yk)) {
      yk = rtInf;
      isinfyk = true;
    } else if (rtIsInf(yk) && (yk > 0.0)) {
      isinfyk = true;
      nInf++;
      iInfinite_data[nInf] = k + 1;
    } else {
      isinfyk = false;
    }
    if (yk != ykfirst) {
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
        if (kfirst >= 1) {
          nInflect++;
          iInflect_data[nInflect] = kfirst;
        }
      } else if (yk < ykfirst) {
        dir = 'd';
        if (previousdir != 'd') {
          nInflect++;
          iInflect_data[nInflect] = kfirst;
          if (previousdir == 'i') {
            nPk++;
            iFinite_data[nPk] = kfirst;
          }
        }
      } else {
        dir = 'i';
        if (previousdir != 'i') {
          nInflect++;
          iInflect_data[nInflect] = kfirst;
        }
      }
      ykfirst = yk;
      kfirst = k + 1;
      isinfykfirst = isinfyk;
    }
  }
  if ((!isinfykfirst) &&
      ((nInflect + 1 == 0) || (iInflect_data[nInflect] < 2048))) {
    nInflect++;
    iInflect_data[nInflect] = 2048;
  }
  if (nPk + 1 < 1) {
    kfirst = -1;
  } else {
    kfirst = nPk;
  }
  nPk = 0;
  i = kfirst + 1;
  for (k = 0; k < i; k++) {
    q = iFinite_data[k];
    ykfirst = Yin[q - 1];
    if (ykfirst > rtMinusInf) {
      if ((Yin[q - 2] >= Yin[q]) || rtIsNaN(Yin[q])) {
        yk = Yin[q - 2];
      } else {
        yk = Yin[q];
      }
      if (ykfirst - yk >= 0.0) {
        nPk++;
        iPk_data[nPk - 1] = (short)q;
      }
    }
  }
  if (nPk < 1) {
    loop_ub = 0;
  } else {
    loop_ub = nPk;
  }
  for (i = 0; i < loop_ub; i++) {
    b_iPk_data[i] = iPk_data[i];
  }
  emxInit_real_T(&bxPk);
  emxInit_real_T(&byPk);
  emxInit_real_T(&wxPk);
  if (nInf + 1 < 1) {
    nInf = -1;
  }
  if (nInflect + 1 < 1) {
    nInflect = -1;
  }
  findExtents(Yin, b_iPk_data, &loop_ub, iFinite_data, kfirst + 1,
              iInfinite_data, nInf + 1, iInflect_data, nInflect + 1, bPk_data,
              &nPk, bxPk, byPk, wxPk);
  emxFree_real_T(&wxPk);
  emxFree_real_T(&byPk);
  emxFree_real_T(&bxPk);
  if (loop_ub == 0) {
    *Ypk_size = 0;
  } else {
    memset(&sortIdx_data[0], 0, (unsigned int)loop_ub * sizeof(int));
    i = loop_ub - 1;
    for (k = 1; k <= i; k += 2) {
      ykfirst = Yin[b_iPk_data[k - 1] - 1];
      if ((ykfirst >= Yin[b_iPk_data[k] - 1]) || rtIsNaN(ykfirst)) {
        sortIdx_data[k - 1] = k;
        sortIdx_data[k] = k + 1;
      } else {
        sortIdx_data[k - 1] = k + 1;
        sortIdx_data[k] = k;
      }
    }
    if ((loop_ub & 1) != 0) {
      sortIdx_data[loop_ub - 1] = loop_ub;
    }
    b_i = 2;
    while (b_i < loop_ub) {
      kfirst = b_i << 1;
      nPk = 1;
      for (nInf = b_i + 1; nInf < loop_ub + 1; nInf = qEnd + b_i) {
        nInflect = nPk - 1;
        q = nInf;
        qEnd = nPk + kfirst;
        if (qEnd > loop_ub + 1) {
          qEnd = loop_ub + 1;
        }
        k = 0;
        kEnd = qEnd - nPk;
        while (k + 1 <= kEnd) {
          ykfirst = Yin[b_iPk_data[sortIdx_data[nInflect] - 1] - 1];
          i = sortIdx_data[q - 1];
          if ((ykfirst >= Yin[b_iPk_data[i - 1] - 1]) || rtIsNaN(ykfirst)) {
            iwork_data[k] = sortIdx_data[nInflect];
            nInflect++;
            if (nInflect + 1 == nInf) {
              while (q < qEnd) {
                k++;
                iwork_data[k] = sortIdx_data[q - 1];
                q++;
              }
            }
          } else {
            iwork_data[k] = i;
            q++;
            if (q == qEnd) {
              while (nInflect + 1 < nInf) {
                k++;
                iwork_data[k] = sortIdx_data[nInflect];
                nInflect++;
              }
            }
          }
          k++;
        }
        for (k = 0; k < kEnd; k++) {
          sortIdx_data[(nPk + k) - 1] = iwork_data[k];
        }
        nPk = qEnd;
      }
      b_i = kfirst;
    }
    for (i = 0; i < loop_ub; i++) {
      locs_temp_data[i] =
          (short)((short)(b_iPk_data[sortIdx_data[i] - 1] - 1) + 1);
    }
    kfirst = loop_ub;
    memset(&idelete_data[0], 0, (unsigned int)loop_ub * sizeof(boolean_T));
    for (b_i = 0; b_i < loop_ub; b_i++) {
      if (!idelete_data[b_i]) {
        if (kfirst == loop_ub) {
          i = sortIdx_data[b_i];
          for (q = 0; q < kfirst; q++) {
            nPk = (short)((short)(b_iPk_data[i - 1] - 1) + 1);
            i1 = locs_temp_data[q];
            idelete_data[q] =
                (idelete_data[q] || ((i1 >= nPk - 100) && (i1 <= nPk + 100)));
          }
        } else {
          binary_expand_op(idelete_data, &kfirst, locs_temp_data, &loop_ub,
                           b_iPk_data, sortIdx_data, b_i);
        }
        idelete_data[b_i] = false;
      }
    }
    kfirst--;
    *Ypk_size = 0;
    nPk = 0;
    for (b_i = 0; b_i <= kfirst; b_i++) {
      if (!idelete_data[b_i]) {
        (*Ypk_size)++;
        tmp_data[nPk] = (short)(b_i + 1);
        nPk++;
      }
    }
    for (i = 0; i < *Ypk_size; i++) {
      idx_data[i] = sortIdx_data[tmp_data[i] - 1];
    }
    sort(idx_data, Ypk_size);
  }
  if (*Ypk_size > 2048) {
    *Ypk_size = 2048;
  }
  for (i = 0; i < *Ypk_size; i++) {
    iwork_data[i] = b_iPk_data[idx_data[i] - 1];
  }
  for (i = 0; i < *Ypk_size; i++) {
    Ypk_data[i] = Yin[iwork_data[i] - 1];
  }
  *Xpk_size = *Ypk_size;
  for (i = 0; i < *Ypk_size; i++) {
    Xpk_data[i] = (short)((short)(iwork_data[i] - 1) + 1);
  }
}

/*
 * File trailer for findpeaks.c
 *
 * [EOF]
 */
