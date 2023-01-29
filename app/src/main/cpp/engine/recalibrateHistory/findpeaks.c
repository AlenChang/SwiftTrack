/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: findpeaks.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 29-Jan-2023 18:06:51
 */

/* Include Files */
#include "findpeaks.h"
#include "findExtents.h"
#include "recalibrateHistory_emxutil.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static void binary_expand_op(emxArray_boolean_T *in1,
                             const emxArray_uint32_T *in2,
                             const emxArray_uint32_T *in3,
                             const emxArray_int32_T *in4,
                             const emxArray_int32_T *in5, int in6);

/* Function Definitions */
/*
 * Arguments    : emxArray_boolean_T *in1
 *                const emxArray_uint32_T *in2
 *                const emxArray_uint32_T *in3
 *                const emxArray_int32_T *in4
 *                const emxArray_int32_T *in5
 *                int in6
 * Return Type  : void
 */
static void binary_expand_op(emxArray_boolean_T *in1,
                             const emxArray_uint32_T *in2,
                             const emxArray_uint32_T *in3,
                             const emxArray_int32_T *in4,
                             const emxArray_int32_T *in5, int in6)
{
  emxArray_boolean_T *b_in1;
  const unsigned int *in2_data;
  const unsigned int *in3_data;
  const int *in4_data;
  const int *in5_data;
  int i;
  unsigned int in3_tmp;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  boolean_T *b_in1_data;
  boolean_T *in1_data;
  in5_data = in5->data;
  in4_data = in4->data;
  in3_data = in3->data;
  in2_data = in2->data;
  in1_data = in1->data;
  in3_tmp = in3_data[in4_data[in5_data[in6] - 1] - 1];
  emxInit_boolean_T(&b_in1, 1);
  i = b_in1->size[0];
  if (in2->size[0] == 1) {
    b_in1->size[0] = in1->size[0];
  } else {
    b_in1->size[0] = in2->size[0];
  }
  emxEnsureCapacity_boolean_T(b_in1, i);
  b_in1_data = b_in1->data;
  stride_0_0 = (in1->size[0] != 1);
  stride_1_0 = (in2->size[0] != 1);
  if (in2->size[0] == 1) {
    loop_ub = in1->size[0];
  } else {
    loop_ub = in2->size[0];
  }
  for (i = 0; i < loop_ub; i++) {
    unsigned int in1_tmp;
    in1_tmp = in2_data[i * stride_1_0];
    b_in1_data[i] =
        (in1_data[i * stride_0_0] ||
         (((int)in1_tmp >= (int)in3_tmp - 30) && (in1_tmp <= in3_tmp + 30U)));
  }
  i = in1->size[0];
  in1->size[0] = b_in1->size[0];
  emxEnsureCapacity_boolean_T(in1, i);
  in1_data = in1->data;
  loop_ub = b_in1->size[0];
  for (i = 0; i < loop_ub; i++) {
    in1_data[i] = b_in1_data[i];
  }
  emxFree_boolean_T(&b_in1);
}

/*
 * Arguments    : const emxArray_real_T *Yin
 *                emxArray_real_T *Ypk
 *                emxArray_real_T *Xpk
 * Return Type  : void
 */
void findpeaks(const emxArray_real_T *Yin, emxArray_real_T *Ypk,
               emxArray_real_T *Xpk)
{
  emxArray_boolean_T *idelete;
  emxArray_int32_T *b_sortIdx;
  emxArray_int32_T *iInflect;
  emxArray_int32_T *iPk;
  emxArray_int32_T *idx;
  emxArray_int32_T *iwork;
  emxArray_int32_T *r1;
  emxArray_real_T *bPk;
  emxArray_real_T *bxPk;
  emxArray_real_T *byPk;
  emxArray_real_T *r;
  emxArray_real_T *wxPk;
  emxArray_uint32_T *locs_temp;
  emxArray_uint32_T *x;
  const double *Yin_data;
  double ykfirst;
  double *Ypk_data;
  int b_i;
  int i;
  int k;
  int kfirst;
  int n;
  int nInf;
  int nInflect;
  int nPk;
  int ny;
  int qEnd;
  int *iPk_data;
  int *idx_data;
  int *iwork_data;
  unsigned int *locs_temp_data;
  int *sortIdx_data;
  unsigned int *x_data;
  char dir;
  boolean_T isinfykfirst;
  boolean_T *idelete_data;
  Yin_data = Yin->data;
  emxInit_uint32_T(&x);
  i = x->size[0];
  x->size[0] = Yin->size[0];
  emxEnsureCapacity_uint32_T(x, i);
  x_data = x->data;
  kfirst = Yin->size[0] - 1;
  for (i = 0; i <= kfirst; i++) {
    x_data[i] = (unsigned int)i + 1U;
  }
  emxInit_int32_T(&b_sortIdx);
  i = b_sortIdx->size[0];
  b_sortIdx->size[0] = Yin->size[0];
  emxEnsureCapacity_int32_T(b_sortIdx, i);
  sortIdx_data = b_sortIdx->data;
  emxInit_int32_T(&idx);
  i = idx->size[0];
  idx->size[0] = Yin->size[0];
  emxEnsureCapacity_int32_T(idx, i);
  idx_data = idx->data;
  emxInit_int32_T(&iInflect);
  i = iInflect->size[0];
  iInflect->size[0] = Yin->size[0];
  emxEnsureCapacity_int32_T(iInflect, i);
  iwork_data = iInflect->data;
  ny = Yin->size[0];
  nPk = 0;
  nInf = 0;
  nInflect = -1;
  dir = 'n';
  kfirst = 0;
  ykfirst = rtInf;
  isinfykfirst = true;
  for (k = 1; k <= ny; k++) {
    double yk;
    boolean_T isinfyk;
    yk = Yin_data[k - 1];
    if (rtIsNaN(yk)) {
      yk = rtInf;
      isinfyk = true;
    } else if (rtIsInf(yk) && (yk > 0.0)) {
      isinfyk = true;
      nInf++;
      idx_data[nInf - 1] = k;
    } else {
      isinfyk = false;
    }
    if (yk != ykfirst) {
      char previousdir;
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
        if (kfirst >= 1) {
          nInflect++;
          iwork_data[nInflect] = kfirst;
        }
      } else if (yk < ykfirst) {
        dir = 'd';
        if (previousdir != 'd') {
          nInflect++;
          iwork_data[nInflect] = kfirst;
          if (previousdir == 'i') {
            nPk++;
            sortIdx_data[nPk - 1] = kfirst;
          }
        }
      } else {
        dir = 'i';
        if (previousdir != 'i') {
          nInflect++;
          iwork_data[nInflect] = kfirst;
        }
      }
      ykfirst = yk;
      kfirst = k;
      isinfykfirst = isinfyk;
    }
  }
  if ((!isinfykfirst) &&
      ((nInflect + 1 == 0) || (iwork_data[nInflect] < Yin->size[0]))) {
    nInflect++;
    iwork_data[nInflect] = Yin->size[0];
  }
  i = b_sortIdx->size[0];
  if (nPk < 1) {
    b_sortIdx->size[0] = 0;
  } else {
    b_sortIdx->size[0] = nPk;
  }
  emxEnsureCapacity_int32_T(b_sortIdx, i);
  sortIdx_data = b_sortIdx->data;
  i = idx->size[0];
  if (nInf < 1) {
    idx->size[0] = 0;
  } else {
    idx->size[0] = nInf;
  }
  emxEnsureCapacity_int32_T(idx, i);
  idx_data = idx->data;
  i = iInflect->size[0];
  if (nInflect + 1 < 1) {
    iInflect->size[0] = 0;
  } else {
    iInflect->size[0] = nInflect + 1;
  }
  emxEnsureCapacity_int32_T(iInflect, i);
  emxInit_int32_T(&iPk);
  i = iPk->size[0];
  iPk->size[0] = b_sortIdx->size[0];
  emxEnsureCapacity_int32_T(iPk, i);
  iPk_data = iPk->data;
  nPk = 0;
  n = b_sortIdx->size[0];
  for (k = 0; k < n; k++) {
    ykfirst = Yin_data[sortIdx_data[k] - 1];
    if ((ykfirst > rtMinusInf) && (ykfirst - fmax(Yin_data[sortIdx_data[k] - 2],
                                                  Yin_data[sortIdx_data[k]]) >=
                                   0.0)) {
      nPk++;
      iPk_data[nPk - 1] = sortIdx_data[k];
    }
  }
  i = iPk->size[0];
  if (nPk < 1) {
    iPk->size[0] = 0;
  } else {
    iPk->size[0] = nPk;
  }
  emxEnsureCapacity_int32_T(iPk, i);
  emxInit_real_T(&r, 1);
  i = r->size[0];
  r->size[0] = Yin->size[0];
  emxEnsureCapacity_real_T(r, i);
  Ypk_data = r->data;
  kfirst = Yin->size[0] - 1;
  for (i = 0; i <= kfirst; i++) {
    Ypk_data[i] = (double)i + 1.0;
  }
  emxInit_real_T(&bPk, 1);
  emxInit_real_T(&bxPk, 2);
  emxInit_real_T(&byPk, 2);
  emxInit_real_T(&wxPk, 2);
  findExtents(Yin, r, iPk, b_sortIdx, idx, iInflect, bPk, bxPk, byPk, wxPk);
  iPk_data = iPk->data;
  emxFree_real_T(&r);
  emxFree_real_T(&wxPk);
  emxFree_real_T(&byPk);
  emxFree_real_T(&bxPk);
  emxFree_real_T(&bPk);
  emxFree_int32_T(&iInflect);
  if (iPk->size[0] == 0) {
    idx->size[0] = 0;
  } else {
    n = iPk->size[0] + 1;
    i = b_sortIdx->size[0];
    b_sortIdx->size[0] = iPk->size[0];
    emxEnsureCapacity_int32_T(b_sortIdx, i);
    sortIdx_data = b_sortIdx->data;
    kfirst = iPk->size[0];
    for (i = 0; i < kfirst; i++) {
      sortIdx_data[i] = 0;
    }
    emxInit_int32_T(&iwork);
    i = iwork->size[0];
    iwork->size[0] = iPk->size[0];
    emxEnsureCapacity_int32_T(iwork, i);
    iwork_data = iwork->data;
    i = iPk->size[0] - 1;
    for (k = 1; k <= i; k += 2) {
      ykfirst = Yin_data[iPk_data[k - 1] - 1];
      if ((ykfirst >= Yin_data[iPk_data[k] - 1]) || rtIsNaN(ykfirst)) {
        sortIdx_data[k - 1] = k;
        sortIdx_data[k] = k + 1;
      } else {
        sortIdx_data[k - 1] = k + 1;
        sortIdx_data[k] = k;
      }
    }
    if ((iPk->size[0] & 1) != 0) {
      sortIdx_data[iPk->size[0] - 1] = iPk->size[0];
    }
    b_i = 2;
    while (b_i < n - 1) {
      ny = b_i << 1;
      kfirst = 1;
      for (nInf = b_i + 1; nInf < n; nInf = qEnd + b_i) {
        int kEnd;
        nInflect = kfirst - 1;
        nPk = nInf;
        qEnd = kfirst + ny;
        if (qEnd > n) {
          qEnd = n;
        }
        k = 0;
        kEnd = qEnd - kfirst;
        while (k + 1 <= kEnd) {
          ykfirst = Yin_data[iPk_data[sortIdx_data[nInflect] - 1] - 1];
          i = sortIdx_data[nPk - 1];
          if ((ykfirst >= Yin_data[iPk_data[i - 1] - 1]) || rtIsNaN(ykfirst)) {
            iwork_data[k] = sortIdx_data[nInflect];
            nInflect++;
            if (nInflect + 1 == nInf) {
              while (nPk < qEnd) {
                k++;
                iwork_data[k] = sortIdx_data[nPk - 1];
                nPk++;
              }
            }
          } else {
            iwork_data[k] = i;
            nPk++;
            if (nPk == qEnd) {
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
          sortIdx_data[(kfirst + k) - 1] = iwork_data[k];
        }
        kfirst = qEnd;
      }
      b_i = ny;
    }
    emxFree_int32_T(&iwork);
    emxInit_uint32_T(&locs_temp);
    i = locs_temp->size[0];
    locs_temp->size[0] = b_sortIdx->size[0];
    emxEnsureCapacity_uint32_T(locs_temp, i);
    locs_temp_data = locs_temp->data;
    kfirst = b_sortIdx->size[0];
    for (i = 0; i < kfirst; i++) {
      locs_temp_data[i] = (unsigned int)iPk_data[sortIdx_data[i] - 1];
    }
    emxInit_boolean_T(&idelete, 1);
    i = idelete->size[0];
    idelete->size[0] = b_sortIdx->size[0];
    emxEnsureCapacity_boolean_T(idelete, i);
    idelete_data = idelete->data;
    kfirst = b_sortIdx->size[0];
    for (i = 0; i < kfirst; i++) {
      idelete_data[i] = false;
    }
    i = b_sortIdx->size[0];
    for (b_i = 0; b_i < i; b_i++) {
      if (!idelete_data[b_i]) {
        if (idelete->size[0] == locs_temp->size[0]) {
          ny = sortIdx_data[b_i];
          kfirst = idelete->size[0];
          for (nInf = 0; nInf < kfirst; nInf++) {
            unsigned int idelete_tmp;
            idelete_tmp = x_data[iPk_data[ny - 1] - 1];
            idelete_data[nInf] =
                (idelete_data[nInf] ||
                 (((int)locs_temp_data[nInf] >= (int)idelete_tmp - 30) &&
                  (locs_temp_data[nInf] <= idelete_tmp + 30U)));
          }
        } else {
          binary_expand_op(idelete, locs_temp, x, iPk, b_sortIdx, b_i);
          idelete_data = idelete->data;
        }
        idelete_data[b_i] = false;
      }
    }
    emxFree_uint32_T(&locs_temp);
    kfirst = idelete->size[0] - 1;
    ny = 0;
    for (b_i = 0; b_i <= kfirst; b_i++) {
      if (!idelete_data[b_i]) {
        ny++;
      }
    }
    emxInit_int32_T(&r1);
    i = r1->size[0];
    r1->size[0] = ny;
    emxEnsureCapacity_int32_T(r1, i);
    iwork_data = r1->data;
    ny = 0;
    for (b_i = 0; b_i <= kfirst; b_i++) {
      if (!idelete_data[b_i]) {
        iwork_data[ny] = b_i + 1;
        ny++;
      }
    }
    emxFree_boolean_T(&idelete);
    i = idx->size[0];
    idx->size[0] = r1->size[0];
    emxEnsureCapacity_int32_T(idx, i);
    idx_data = idx->data;
    kfirst = r1->size[0];
    for (i = 0; i < kfirst; i++) {
      idx_data[i] = sortIdx_data[iwork_data[i] - 1];
    }
    emxFree_int32_T(&r1);
    sort(idx);
    idx_data = idx->data;
  }
  emxFree_uint32_T(&x);
  if (idx->size[0] > Yin->size[0]) {
    i = idx->size[0];
    idx->size[0] = Yin->size[0];
    emxEnsureCapacity_int32_T(idx, i);
    idx_data = idx->data;
  }
  i = b_sortIdx->size[0];
  b_sortIdx->size[0] = idx->size[0];
  emxEnsureCapacity_int32_T(b_sortIdx, i);
  sortIdx_data = b_sortIdx->data;
  kfirst = idx->size[0];
  for (i = 0; i < kfirst; i++) {
    sortIdx_data[i] = iPk_data[idx_data[i] - 1];
  }
  emxFree_int32_T(&idx);
  i = iPk->size[0];
  iPk->size[0] = b_sortIdx->size[0];
  emxEnsureCapacity_int32_T(iPk, i);
  iPk_data = iPk->data;
  kfirst = b_sortIdx->size[0];
  for (i = 0; i < kfirst; i++) {
    iPk_data[i] = sortIdx_data[i];
  }
  emxFree_int32_T(&b_sortIdx);
  i = Ypk->size[0];
  Ypk->size[0] = iPk->size[0];
  emxEnsureCapacity_real_T(Ypk, i);
  Ypk_data = Ypk->data;
  kfirst = iPk->size[0];
  for (i = 0; i < kfirst; i++) {
    Ypk_data[i] = Yin_data[iPk_data[i] - 1];
  }
  i = Xpk->size[0];
  Xpk->size[0] = iPk->size[0];
  emxEnsureCapacity_real_T(Xpk, i);
  Ypk_data = Xpk->data;
  kfirst = iPk->size[0];
  for (i = 0; i < kfirst; i++) {
    Ypk_data[i] = (unsigned int)iPk_data[i];
  }
  emxFree_int32_T(&iPk);
}

/*
 * File trailer for findpeaks.c
 *
 * [EOF]
 */
