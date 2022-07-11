//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// cir2dist.cpp
//
// Code generation for function 'cir2dist'
//

// Include files
#include "cir2dist.h"
#include "processFramePipe_rtwutil.h"
#include "coder_array.h"
#include <cmath>

// Variable Definitions
static double last_phase;

static double last_unwrap_phase;

// Function Definitions
void cir2dist(coder::array<creal_T, 2U> &cir, coder::array<double, 2U> &dist)
{
  coder::array<creal_T, 2U> taps;
  coder::array<double, 2U> ex;
  coder::array<double, 2U> p;
  coder::array<double, 2U> varargin_1;
  coder::array<unsigned char, 2U> idx;
  double d;
  double work_data;
  int i;
  int n;
  int nx;
  n = cir.size(1) - 1;
  for (i = 0; i <= n; i++) {
    for (nx = 0; nx < 200; nx++) {
      cir[nx + 200 * i] = cir[nx + cir.size(0) * i];
    }
  }
  cir.set_size(200, n + 1);
  nx = 200 * (n + 1);
  varargin_1.set_size(200, n + 1);
  for (n = 0; n < nx; n++) {
    varargin_1[n] = rt_hypotd(cir[n].re, cir[n].im);
  }
  n = varargin_1.size(1);
  idx.set_size(1, varargin_1.size(1));
  nx = varargin_1.size(1);
  for (i = 0; i < nx; i++) {
    idx[i] = 1U;
  }
  if (varargin_1.size(1) >= 1) {
    for (nx = 0; nx < n; nx++) {
      d = varargin_1[200 * nx];
      for (i = 0; i < 199; i++) {
        work_data = varargin_1[(i + 200 * nx) + 1];
        if (d < work_data) {
          d = work_data;
          idx[nx] = static_cast<unsigned char>(i + 2);
        }
      }
    }
  }
  ex.set_size(1, idx.size(1));
  nx = idx.size(1);
  for (i = 0; i < nx; i++) {
    ex[i] = idx[i];
  }
  taps.set_size(1, ex.size(1));
  nx = ex.size(1);
  for (i = 0; i < nx; i++) {
    taps[i].re = 0.0;
    taps[i].im = 0.0;
  }
  i = ex.size(1);
  for (nx = 0; nx < i; nx++) {
    taps[nx] = cir[(static_cast<int>(ex[nx]) + cir.size(0) * nx) - 1];
  }
  nx = taps.size(1);
  ex.set_size(1, taps.size(1));
  for (n = 0; n < nx; n++) {
    ex[n] = std::atan2(taps[n].im, taps[n].re);
  }
  p.set_size(1, ex.size(1) + 1);
  p[0] = last_phase;
  nx = ex.size(1);
  for (i = 0; i < nx; i++) {
    p[i + 1] = ex[i];
  }
  n = p.size(1);
  nx = p.size(1) - 1;
  if (nx > 1) {
    nx = 1;
  }
  if (nx < 1) {
    dist.set_size(1, 0);
  } else {
    dist.set_size(1, p.size(1) - 1);
    if (p.size(1) - 1 != 0) {
      work_data = p[0];
      for (nx = 2; nx <= n; nx++) {
        double tmp1;
        tmp1 = p[nx - 1];
        d = tmp1;
        tmp1 -= work_data;
        work_data = d;
        dist[nx - 2] = tmp1;
      }
    }
  }
  i = dist.size(1);
  for (nx = 0; nx < i; nx++) {
    d = dist[nx];
    if (d > 3.1415926535897931) {
      d -= 6.2831853071795862;
      dist[nx] = d;
    }
    if (d < -3.1415926535897931) {
      d += 6.2831853071795862;
      dist[nx] = d;
    }
  }
  if ((dist.size(1) != 0) && (dist.size(1) != 1)) {
    i = dist.size(1);
    for (n = 0; n <= i - 2; n++) {
      dist[n + 1] = dist[n] + dist[n + 1];
    }
  }
  dist.set_size(1, dist.size(1));
  nx = dist.size(1) - 1;
  for (i = 0; i <= nx; i++) {
    dist[i] = dist[i] + last_unwrap_phase;
  }
  last_phase = ex[ex.size(1) - 1];
  last_unwrap_phase = dist[dist.size(1) - 1];
  dist.set_size(1, dist.size(1));
  nx = dist.size(1) - 1;
  for (i = 0; i <= nx; i++) {
    dist[i] = -dist[i] * 34300.0 / 2.0 / 3.1415926535897931 / 19000.0 / 2.0;
  }
}

void cir2dist_init()
{
  last_phase = 0.0;
  last_unwrap_phase = 0.0;
}

// End of code generation (cir2dist.cpp)
