//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: codeGen.cpp
//
// MATLAB Coder version            : 5.2
// C/C++ source code generated on  : 06-Nov-2021 14:27:28
//

// Include Files
#include "codeGen.h"
#include "quickselect.h"
#include <algorithm>
#include <cmath>

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
codeGen::codeGen()
{
}

//
// Arguments    : void
// Return Type  : void
//
codeGen::~codeGen()
{
  // (no terminate code required)
}

//
// Arguments    : double win[480]
// Return Type  : void
//
void codeGen::genWindow(double win[480])
{
  static const double b_win[480]{0.0,
                                 0.0029310214228202014,
                                 0.011689722144956671,
                                 0.026173414408598772,
                                 0.0462122901645215,
                                 0.071571411916205363,
                                 0.10195346714717812,
                                 0.13700225403843458,
                                 0.17630685760908615,
                                 0.21940646731880886,
                                 0.26579577965010492,
                                 0.31493092233004272,
                                 0.36623583073538951,
                                 0.41910900172361759,
                                 0.47293054570729121,
                                 0.52706945429270879,
                                 0.58089099827638235,
                                 0.63376416926461032,
                                 0.68506907766995728,
                                 0.73420422034989508,
                                 0.78059353268119114,
                                 0.82369314239091385,
                                 0.86299774596156531,
                                 0.89804653285282188,
                                 0.92842858808379458,
                                 0.95378770983547845,
                                 0.97382658559140123,
                                 0.98831027785504333,
                                 0.9970689785771798,
                                 1.0,
                                 0.9970689785771798,
                                 0.98831027785504333,
                                 0.97382658559140123,
                                 0.95378770983547845,
                                 0.92842858808379458,
                                 0.89804653285282188,
                                 0.86299774596156531,
                                 0.82369314239091385,
                                 0.78059353268119114,
                                 0.73420422034989508,
                                 0.68506907766995728,
                                 0.63376416926461032,
                                 0.58089099827638235,
                                 0.52706945429270879,
                                 0.47293054570729121,
                                 0.41910900172361759,
                                 0.36623583073538951,
                                 0.31493092233004272,
                                 0.26579577965010492,
                                 0.21940646731880886,
                                 0.17630685760908615,
                                 0.13700225403843458,
                                 0.10195346714717812,
                                 0.071571411916205363,
                                 0.0462122901645215,
                                 0.026173414408598772,
                                 0.011689722144956671,
                                 0.0029310214228202014,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0};
  double a__1[29];
  std::copy(&b_win[0], &b_win[480], &win[0]);
  std::copy(&b_win[0], &b_win[29], &a__1[0]);
  for (int k{0}; k < 451; k++) {
    win[k] = win[k + 29];
  }
  std::copy(&a__1[0], &a__1[29], &win[451]);
}

//
// Arguments    : double x
//                double buffer[5]
//                double *id
// Return Type  : double
//
double codeGen::mvMedian(double x, double buffer[5], double *id)
{
  double a__4[5];
  double vref;
  int ia;
  int ib;
  int ilast;
  int ipiv;
  int oldnv;
  boolean_T checkspeed;
  boolean_T exitg1;
  boolean_T isslow;
  if (*id == 0.0) {
    vref = 0.0;
  } else {
    vref = std::fmod(*id, 5.0);
    if (vref == 0.0) {
      vref = 0.0;
    } else if (*id < 0.0) {
      vref += 5.0;
    }
  }
  buffer[static_cast<int>(vref + 1.0) - 1] = x;
  (*id)++;
  for (ipiv = 0; ipiv < 5; ipiv++) {
    a__4[ipiv] = buffer[ipiv];
  }
  ipiv = 2;
  ia = 0;
  ib = 4;
  ilast = 4;
  oldnv = 5;
  checkspeed = false;
  isslow = false;
  exitg1 = false;
  while ((!exitg1) && (ia + 1 < ib + 1)) {
    int k;
    boolean_T guard1{false};
    vref = a__4[ipiv];
    a__4[ipiv] = a__4[ib];
    a__4[ib] = vref;
    ilast = ia;
    ipiv = -1;
    for (k = ia + 1; k <= ib; k++) {
      double vk_tmp;
      vk_tmp = a__4[k - 1];
      if (vk_tmp == vref) {
        a__4[k - 1] = a__4[ilast];
        a__4[ilast] = vk_tmp;
        ipiv++;
        ilast++;
      } else if (vk_tmp < vref) {
        a__4[k - 1] = a__4[ilast];
        a__4[ilast] = vk_tmp;
        ilast++;
      }
    }
    a__4[ib] = a__4[ilast];
    a__4[ilast] = vref;
    guard1 = false;
    if (3 <= ilast + 1) {
      if (3 >= ilast - ipiv) {
        exitg1 = true;
      } else {
        ib = ilast - 1;
        guard1 = true;
      }
    } else {
      ia = ilast + 1;
      guard1 = true;
    }
    if (guard1) {
      int c;
      c = (ib - ia) + 1;
      if (checkspeed) {
        isslow = (c > oldnv / 2);
        oldnv = c;
      }
      checkspeed = !checkspeed;
      if (isslow) {
        while (c > 1) {
          int ngroupsof5;
          int nlast;
          ngroupsof5 = c / 5;
          nlast = c - ngroupsof5 * 5;
          c = ngroupsof5;
          for (k = 0; k < ngroupsof5; k++) {
            ipiv = (ia + k * 5) + 1;
            ipiv = coder::internal::thirdOfFive(a__4, ipiv, ipiv + 4) - 1;
            ilast = ia + k;
            vref = a__4[ilast];
            a__4[ilast] = a__4[ipiv];
            a__4[ipiv] = vref;
          }
          if (nlast > 0) {
            ipiv = (ia + ngroupsof5 * 5) + 1;
            ipiv =
                coder::internal::thirdOfFive(a__4, ipiv, (ipiv + nlast) - 1) -
                1;
            ilast = ia + ngroupsof5;
            vref = a__4[ilast];
            a__4[ilast] = a__4[ipiv];
            a__4[ipiv] = vref;
            c = ngroupsof5 + 1;
          }
        }
      } else if (c >= 3) {
        ipiv = ia + (c - 1) / 2;
        if (a__4[ia] < a__4[ipiv]) {
          if (a__4[ipiv] >= a__4[ib]) {
            if (a__4[ia] < a__4[ib]) {
              ipiv = ib;
            } else {
              ipiv = ia;
            }
          }
        } else if (a__4[ia] < a__4[ib]) {
          ipiv = ia;
        } else if (a__4[ipiv] < a__4[ib]) {
          ipiv = ib;
        }
        if (ipiv + 1 > ia + 1) {
          vref = a__4[ia];
          a__4[ia] = a__4[ipiv];
          a__4[ipiv] = vref;
        }
      }
      ipiv = ia;
      ilast = ib;
    }
  }
  return a__4[ilast];
}

//
// File trailer for codeGen.cpp
//
// [EOF]
//
