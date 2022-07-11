//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// calibrateFrame.cpp
//
// Code generation for function 'calibrateFrame'
//

// Include files
#include "calibrateFrame.h"
#include "circshift.h"
#include "fft.h"
#include "ifft.h"
#include "processFramePipe_internal_types.h"
#include "processFramePipe_rtwutil.h"
#include "rxbb2cir.h"
#include "coder_array.h"

// Variable Definitions
static boolean_T rxbb_calib_data_not_empty;

static double last_pks;

static double last_locs;

// Function Declarations
static void minus(creal_T in1[480], const creal_T in2[480],
                  const coder::array<creal_T, 1U> &in3);

// Function Definitions
static void minus(creal_T in1[480], const creal_T in2[480],
                  const coder::array<creal_T, 1U> &in3)
{
  int stride_0_0;
  stride_0_0 = (in3.size(0) != 1);
  for (int i{0}; i < 480; i++) {
    in1[i].re = in2[i].re - in3[i * stride_0_0].re;
    in1[i].im = in2[i].im - in3[i * stride_0_0].im;
  }
}

void calibrateFrame(const creal_T rxbb[480], ATParameters *params)
{
  static coder::array<creal_T, 2U> rxbb_calib_data;
  static coder::array<creal_T, 1U> last_cir;
  coder::array<creal_T, 2U> b_x;
  coder::array<creal_T, 2U> b_y1;
  coder::array<creal_T, 2U> taps;
  coder::array<creal_T, 2U> x;
  coder::array<creal_T, 1U> r;
  coder::array<double, 2U> b_varargin_1;
  coder::array<double, 2U> ex;
  coder::array<unsigned char, 2U> idx;
  creal_T cirdiff[480];
  int i;
  int ixLead;
  int loop_ub;
  if (!rxbb_calib_data_not_empty) {
    ixLead = static_cast<int>(params->Nzc_up);
    i = static_cast<int>(48000.0 / params->Nzc_up);
    rxbb_calib_data.set_size(ixLead, i);
    loop_ub = ixLead * i;
    for (i = 0; i < loop_ub; i++) {
      rxbb_calib_data[i].re = 0.0;
      rxbb_calib_data[i].im = 0.0;
    }
    rxbb_calib_data_not_empty =
        ((rxbb_calib_data.size(0) != 0) && (rxbb_calib_data.size(1) != 0));
    last_cir.set_size(ixLead);
    for (i = 0; i < ixLead; i++) {
      last_cir[i].re = 0.0;
      last_cir[i].im = 0.0;
    }
  }
  // * the first five frams are used to determing the start point
  if (!params->syncSucceeded) {
    creal_T cir[480];
    double varargin_1[480];
    double tmp2_im;
    double work_re;
    int nx;
    coder::fft(params->zc_up, taps);
    r.set_size(taps.size(1));
    ixLead = taps.size(1);
    for (i = 0; i < ixLead; i++) {
      r[i].re = taps[i].re;
      r[i].im = -taps[i].im;
    }
    coder::fft(rxbb, cir);
    if (r.size(0) == 480) {
      creal_T b_cir[480];
      for (i = 0; i < 480; i++) {
        double tmp2_re;
        double work_im;
        tmp2_im = r[i].im;
        work_re = r[i].re;
        work_im = cir[i].re;
        tmp2_re = cir[i].im;
        b_cir[i].re = work_im * work_re - tmp2_re * tmp2_im;
        b_cir[i].im = work_im * tmp2_im + tmp2_re * work_re;
      }
      coder::ifft(b_cir, cir);
    } else {
      binary_expand_op(cir, r);
    }
    for (int m{0}; m < 480; m++) {
      varargin_1[m] = rt_hypotd(cir[m].re, cir[m].im);
    }
    work_re = varargin_1[0];
    nx = -1;
    for (int m{0}; m < 479; m++) {
      tmp2_im = varargin_1[m + 1];
      if (work_re < tmp2_im) {
        work_re = tmp2_im;
        nx = m;
      }
    }
    if (work_re > last_pks) {
      last_locs = static_cast<double>(nx) + 2.0;
      last_pks = work_re;
    }
    if (params->frame_counter >= 5.0) {
      params->sync_id = last_locs;
      coder::circshift(params->zc_up, last_locs);
      params->syncSucceeded = true;
      //  fprintf('Scynchronization succeed in the %ith frame\n',
      //  params.frame_counter);
    }
  } else if (params->time_counter <= 1.0) {
    creal_T cir[480];
    int nx;
    coder::fft(params->zc_up, taps);
    r.set_size(taps.size(1));
    ixLead = taps.size(1);
    for (i = 0; i < ixLead; i++) {
      r[i].re = taps[i].re;
      r[i].im = -taps[i].im;
    }
    coder::fft(rxbb, cir);
    if (r.size(0) == 480) {
      creal_T b_cir[480];
      for (i = 0; i < 480; i++) {
        double tmp2_im;
        double tmp2_re;
        double work_im;
        double work_re;
        tmp2_im = r[i].im;
        work_re = r[i].re;
        work_im = cir[i].re;
        tmp2_re = cir[i].im;
        b_cir[i].re = work_im * work_re - tmp2_re * tmp2_im;
        b_cir[i].im = work_im * tmp2_im + tmp2_re * work_re;
      }
      coder::ifft(b_cir, cir);
    } else {
      binary_expand_op(cir, r);
    }
    nx = static_cast<int>(params->frame_counter);
    ixLead = rxbb_calib_data.size(0);
    for (i = 0; i < ixLead; i++) {
      rxbb_calib_data[i + rxbb_calib_data.size(0) * (nx - 1)] = cir[i];
    }
    ixLead = rxbb_calib_data.size(0);
    last_cir.set_size(rxbb_calib_data.size(0));
    for (i = 0; i < ixLead; i++) {
      last_cir[i] = rxbb_calib_data[i + rxbb_calib_data.size(0) * (nx - 1)];
    }
  } else {
    creal_T cir[480];
    double varargin_1_data[200];
    double tmp2_im;
    double tmp2_re;
    double work_im;
    double work_re;
    if (!params->caliSucceeded) {
      int i1;
      int iy;
      int lastBlockLength;
      int nx;
      if (rxbb_calib_data.size(1) - 1 < 6) {
        i = -1;
        i1 = -1;
      } else {
        i = 4;
        i1 = rxbb_calib_data.size(1) - 2;
      }
      ixLead = rxbb_calib_data.size(0);
      loop_ub = i1 - i;
      x.set_size(rxbb_calib_data.size(0), loop_ub);
      for (i1 = 0; i1 < loop_ub; i1++) {
        for (nx = 0; nx < ixLead; nx++) {
          x[nx + x.size(0) * i1] =
              rxbb_calib_data[nx + rxbb_calib_data.size(0) * ((i + i1) + 1)];
        }
      }
      if (loop_ub <= 1) {
        b_y1.set_size(rxbb_calib_data.size(0), 0);
      } else {
        b_y1.set_size(rxbb_calib_data.size(0), loop_ub - 1);
        if (rxbb_calib_data.size(0) != 0) {
          i = rxbb_calib_data.size(0);
          iy = 0;
          for (int s{0}; s < i; s++) {
            ixLead = s + i;
            work_re = x[s].re;
            work_im = x[s].im;
            for (int m{2}; m <= loop_ub; m++) {
              tmp2_re = work_re;
              tmp2_im = work_im;
              nx = (m - 2) * i;
              lastBlockLength = ixLead + nx;
              work_re = x[lastBlockLength].re;
              work_im = x[lastBlockLength].im;
              i1 = iy + nx;
              b_y1[i1].re = x[lastBlockLength].re - tmp2_re;
              b_y1[i1].im = x[lastBlockLength].im - tmp2_im;
            }
            iy++;
          }
        }
      }
      ixLead = b_y1.size(1);
      b_x.set_size(200, b_y1.size(1));
      for (i = 0; i < ixLead; i++) {
        for (i1 = 0; i1 < 200; i1++) {
          b_x[i1 + 200 * i] = b_y1[i1 + b_y1.size(0) * i];
        }
      }
      nx = 200 * b_y1.size(1);
      b_varargin_1.set_size(200, b_y1.size(1));
      for (int m{0}; m < nx; m++) {
        b_varargin_1[m] = rt_hypotd(b_x[m].re, b_x[m].im);
      }
      iy = b_varargin_1.size(1);
      idx.set_size(1, b_varargin_1.size(1));
      ixLead = b_varargin_1.size(1);
      for (i = 0; i < ixLead; i++) {
        idx[i] = 1U;
      }
      if (b_varargin_1.size(1) >= 1) {
        for (nx = 0; nx < iy; nx++) {
          tmp2_im = b_varargin_1[200 * nx];
          for (ixLead = 0; ixLead < 199; ixLead++) {
            work_re = b_varargin_1[(ixLead + 200 * nx) + 1];
            if (tmp2_im < work_re) {
              tmp2_im = work_re;
              idx[nx] = static_cast<unsigned char>(ixLead + 2);
            }
          }
        }
      }
      ex.set_size(1, idx.size(1));
      ixLead = idx.size(1);
      for (i = 0; i < ixLead; i++) {
        ex[i] = idx[i];
      }
      nx = ex.size(1);
      taps.set_size(1, ex.size(1));
      for (i = 0; i < nx; i++) {
        taps[i].re = 0.0;
        taps[i].im = 0.0;
      }
      i = taps.size(1);
      for (nx = 0; nx < i; nx++) {
        taps[nx] = b_y1[(static_cast<int>(ex[nx]) + b_y1.size(0) * nx) - 1];
      }
      nx = taps.size(1);
      ex.set_size(1, taps.size(1));
      for (int m{0}; m < nx; m++) {
        ex[m] = rt_hypotd(taps[m].re, taps[m].im);
      }
      if (ex.size(1) == 0) {
        work_re = 0.0;
      } else {
        if (ex.size(1) <= 1024) {
          nx = ex.size(1);
          lastBlockLength = 0;
          iy = 1;
        } else {
          nx = 1024;
          iy = ex.size(1) / 1024;
          lastBlockLength = ex.size(1) - (iy << 10);
          if (lastBlockLength > 0) {
            iy++;
          } else {
            lastBlockLength = 1024;
          }
        }
        work_re = ex[0];
        for (int m{2}; m <= nx; m++) {
          work_re += ex[m - 1];
        }
        for (int s{2}; s <= iy; s++) {
          nx = (s - 1) << 10;
          work_im = ex[nx];
          if (s == iy) {
            ixLead = lastBlockLength;
          } else {
            ixLead = 1024;
          }
          for (int m{2}; m <= ixLead; m++) {
            work_im += ex[(nx + m) - 1];
          }
          work_re += work_im;
        }
      }
      params->movingThreshold = work_re / static_cast<double>(ex.size(1)) * 1.5;
      params->caliSucceeded = true;
      //  fprintf('Calibration succeed in the %ith frame\n',
      //  params.frame_counter); keyboard
    }
    coder::fft(params->zc_up, taps);
    r.set_size(taps.size(1));
    ixLead = taps.size(1);
    for (i = 0; i < ixLead; i++) {
      r[i].re = taps[i].re;
      r[i].im = -taps[i].im;
    }
    coder::fft(rxbb, cir);
    if (r.size(0) == 480) {
      creal_T b_cir[480];
      for (i = 0; i < 480; i++) {
        tmp2_im = r[i].im;
        work_re = r[i].re;
        work_im = cir[i].re;
        tmp2_re = cir[i].im;
        b_cir[i].re = work_im * work_re - tmp2_re * tmp2_im;
        b_cir[i].im = work_im * tmp2_im + tmp2_re * work_re;
      }
      coder::ifft(b_cir, cir);
    } else {
      binary_expand_op(cir, r);
    }
    if (last_cir.size(0) == 480) {
      for (i = 0; i < 480; i++) {
        cirdiff[i].re = cir[i].re - last_cir[i].re;
        cirdiff[i].im = cir[i].im - last_cir[i].im;
      }
    } else {
      minus(cirdiff, cir, last_cir);
    }
    for (int m{0}; m < 200; m++) {
      varargin_1_data[m] = rt_hypotd(cirdiff[m].re, cirdiff[m].im);
    }
    work_re = varargin_1_data[0];
    for (int m{0}; m < 199; m++) {
      tmp2_im = varargin_1_data[m + 1];
      if (work_re < tmp2_im) {
        work_re = tmp2_im;
      }
    }
    params->current_moving_frame = (work_re > params->movingThreshold);
    last_cir.set_size(480);
    for (i = 0; i < 480; i++) {
      last_cir[i] = cir[i];
    }
  }
}

void calibrateFrame_init()
{
  rxbb_calib_data_not_empty = false;
}

void synchronizeFrame_init()
{
  last_pks = 0.0;
  last_locs = 0.0;
}

// End of code generation (calibrateFrame.cpp)
