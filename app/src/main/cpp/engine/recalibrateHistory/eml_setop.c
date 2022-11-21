/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: eml_setop.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 18-Nov-2022 21:41:51
 */

/* Include Files */
#include "eml_setop.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const int a_data[]
 *                int a_size
 *                const int b_data[]
 *                int b_size
 *                int c_data[]
 *                int *c_size
 *                int ia_data[]
 *                int *ia_size
 *                int ib_data[]
 *                int *ib_size
 * Return Type  : void
 */
void b_do_vectors(const int a_data[], int a_size, const int b_data[],
                  int b_size, int c_data[], int *c_size, int ia_data[],
                  int *ia_size, int ib_data[], int *ib_size)
{
  int ak;
  int b_ialast;
  int b_iblast;
  int bk;
  int iafirst;
  int ialast;
  int ibfirst;
  int iblast;
  int nc;
  if (a_size <= b_size) {
    *c_size = a_size;
  } else {
    *c_size = b_size;
  }
  *ia_size = *c_size;
  *ib_size = *c_size;
  nc = 0;
  iafirst = 0;
  ialast = 1;
  ibfirst = 0;
  iblast = 1;
  while ((ialast <= a_size) && (iblast <= b_size)) {
    b_ialast = ialast;
    ak = a_data[ialast - 1];
    while ((b_ialast < a_size) && (a_data[b_ialast] == ak)) {
      b_ialast++;
    }
    ialast = b_ialast;
    b_iblast = iblast;
    bk = b_data[iblast - 1];
    while ((b_iblast < b_size) && (b_data[b_iblast] == bk)) {
      b_iblast++;
    }
    iblast = b_iblast;
    if (ak == bk) {
      nc++;
      c_data[nc - 1] = ak;
      ia_data[nc - 1] = iafirst + 1;
      ib_data[nc - 1] = ibfirst + 1;
      ialast = b_ialast + 1;
      iafirst = b_ialast;
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    } else if (ak < bk) {
      ialast = b_ialast + 1;
      iafirst = b_ialast;
    } else {
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    }
  }
  if (*c_size > 0) {
    if (nc < 1) {
      *ia_size = 0;
      *ib_size = 0;
      *c_size = 0;
    } else {
      *ia_size = nc;
      *ib_size = nc;
      *c_size = nc;
    }
  }
}

/*
 * Arguments    : const int a_data[]
 *                int a_size
 *                const int b_data[]
 *                int b_size
 *                int c_data[]
 *                int *c_size
 *                int ia_data[]
 *                int *ia_size
 *                int ib_data[]
 *                int *ib_size
 * Return Type  : void
 */
void do_vectors(const int a_data[], int a_size, const int b_data[], int b_size,
                int c_data[], int *c_size, int ia_data[], int *ia_size,
                int ib_data[], int *ib_size)
{
  int ak;
  int b_ialast;
  int b_iblast;
  int bk;
  int iafirst;
  int ialast;
  int ibfirst;
  int iblast;
  int nc;
  int nia;
  int nib;
  *c_size = a_size + b_size;
  *ia_size = a_size;
  *ib_size = b_size;
  nc = -1;
  nia = -1;
  nib = 0;
  iafirst = 1;
  ialast = 0;
  ibfirst = 0;
  iblast = 0;
  while ((ialast + 1 <= a_size) && (iblast + 1 <= b_size)) {
    b_ialast = ialast + 1;
    ak = a_data[ialast];
    while ((b_ialast < a_size) && (a_data[b_ialast] == ak)) {
      b_ialast++;
    }
    ialast = b_ialast - 1;
    b_iblast = iblast + 1;
    bk = b_data[iblast];
    while ((b_iblast < b_size) && (b_data[b_iblast] == bk)) {
      b_iblast++;
    }
    iblast = b_iblast - 1;
    if (ak == bk) {
      nc++;
      c_data[nc] = ak;
      nia++;
      ia_data[nia] = iafirst;
      ialast = b_ialast;
      iafirst = b_ialast + 1;
      iblast = b_iblast;
      ibfirst = b_iblast;
    } else if (ak < bk) {
      nc++;
      nia++;
      c_data[nc] = ak;
      ia_data[nia] = iafirst;
      ialast = b_ialast;
      iafirst = b_ialast + 1;
    } else {
      nc++;
      nib++;
      c_data[nc] = bk;
      ib_data[nib - 1] = ibfirst + 1;
      iblast = b_iblast;
      ibfirst = b_iblast;
    }
  }
  while (ialast + 1 <= a_size) {
    b_ialast = ialast + 1;
    while ((b_ialast < a_size) && (a_data[b_ialast] == a_data[ialast])) {
      b_ialast++;
    }
    nc++;
    nia++;
    c_data[nc] = a_data[ialast];
    ia_data[nia] = iafirst;
    ialast = b_ialast;
    iafirst = b_ialast + 1;
  }
  while (iblast + 1 <= b_size) {
    b_iblast = iblast + 1;
    while ((b_iblast < b_size) && (b_data[b_iblast] == b_data[iblast])) {
      b_iblast++;
    }
    nc++;
    nib++;
    c_data[nc] = b_data[iblast];
    ib_data[nib - 1] = ibfirst + 1;
    iblast = b_iblast;
    ibfirst = b_iblast;
  }
  if (a_size > 0) {
    if (nia + 1 < 1) {
      *ia_size = 0;
    } else {
      *ia_size = nia + 1;
    }
  }
  if (b_size > 0) {
    if (nib < 1) {
      *ib_size = 0;
    } else {
      *ib_size = nib;
    }
  }
  if (*c_size > 0) {
    if (nc + 1 < 1) {
      *c_size = 0;
    } else {
      *c_size = nc + 1;
    }
  }
}

/*
 * File trailer for eml_setop.c
 *
 * [EOF]
 */
