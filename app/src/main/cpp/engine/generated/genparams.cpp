//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// genparams.cpp
//
// Code generation for function 'genparams'
//

// Include files
#include "genparams.h"
#include "processFramePipe_data.h"
#include "processFramePipe_internal_types.h"
#include "coder_array.h"
#include "fftw3.h"
#include <cmath>

// Function Definitions
void genparams(double Nzc_up, double down_sample_factor, ATParameters *params)
{
  fftw_plan plan;
  int b_sign;
  coder::array<creal_T, 2U> *y;
  coder::array<creal_T, 2U> b_xPadded;
  coder::array<creal_T, 2U> fseq;
  coder::array<creal_T, 2U> xPadded;
  coder::array<creal_T, 2U> zc;
  coder::array<double, 2U> n;
  coder::array<int, 2U> b_y;
  coder::array<int, 1U> b_r;
  fftw_iodim b_howmany_iodims;
  fftw_iodim b_iodims;
  fftw_iodim howmany_iodims;
  fftw_iodim iodims;
  double a;
  double ai;
  double g;
  double r;
  double y_im;
  int i;
  int k;
  int loop_ub_tmp;
  int nx;
  int yk;
  // %% parameter initialization
  //     %% baisc parameters
  params->Nzc_up = Nzc_up;
  params->BW = 4000.0;
  params->fs = 48000.0;
  params->fc = 19000.0;
  params->c = 34300.0;
  params->down_sample_factor = down_sample_factor;
  //     %% parameters for offline analysis
  params->read_channel = 1.0;
  params->start_cut_time_duration = 7.0;
  params->end_cut_time_duration = 8.0;
  params->PLOT = false;
  //     %% we focus on taps from idlow to idhigh
  params->idlow = 1.0;
  params->idhigh = 200.0;
  //     %% inverse frequency response
  params->COMP_FREQ = true;
  loop_ub_tmp = static_cast<int>(Nzc_up);
  params->inv_freq.set_size(loop_ub_tmp);
  for (i = 0; i < loop_ub_tmp; i++) {
    params->inv_freq[i] = 1.0;
  }
  //     %% add window to remove side lobes
  params->win.kaiser.set_size(loop_ub_tmp);
  for (i = 0; i < loop_ub_tmp; i++) {
    params->win.kaiser[i] = 1.0;
  }
  //     %% doppler search size
  params->doppler_search_step_size = 10.0;
  //     %% time duration for calibration
  params->cali_time = 1.0;
  //  calibrate 1 second
  //     %% counters to denote time stamp of each frame
  params->time_counter = 0.0;
  params->frame_counter = 0.0;
  //     %% flags
  params->caliSucceeded = false;
  params->syncSucceeded = false;
  params->backgroundInitSucceeded = false;
  params->bufferInitSucceeded = false;
  //     %% the start tap of LOS signal
  params->sync_id = 1.0;
  //     %% threshold to determine motions
  params->movingThreshold = 0.0;
  params->movingThresholdFacor = 1.5;
  //  params.movingThreshold = params.movingThresholdFacor * mean(estimation);
  //     %% flag to record the moving status of current frame
  params->current_moving_frame = false;
  // %% UPDATE parameters after initialization
  params->Nzc = Nzc_up * 4000.0 / 48000.0 - 1.0;
  params->U = (params->Nzc - 1.0) / 2.0;
  params->T = Nzc_up / 48000.0;
  //  if (nargin < 2)
  //      Nzc    = 127;    %% length of ZC
  //  %     Nzc_up = 1024;   %% length after up-sampling to 6KHz: Nzc * Fs / bw
  //  = 127 * 48000 / 6000 = 1016 end Fs = 48000;
  //  -------------------
  //  ZC sequence
  //  guard  = 0;     %% guard band to avoid inter-symbol interference
  //    %% 0 < u < Nzc, gcd(u, Nzc) = 1
  if (params->Nzc - 1.0 < 0.0) {
    n.set_size(1, 0);
  } else {
    n.set_size(1, static_cast<int>(params->Nzc - 1.0) + 1);
    yk = static_cast<int>(params->Nzc - 1.0);
    for (i = 0; i <= yk; i++) {
      n[i] = i;
    }
  }
  //  if (mod(Nzc,2) == 0)
  //      zc  = exp(-1i * pi * u * n .* (n  + 2 * q) / Nzc);
  //  else
  //      zc  = exp(-1i * pi * u * n .* (n + 1 + 2 * q) / Nzc);
  //  end
  y_im = params->U * -3.1415926535897931;
  if (params->U == 0.0) {
    g = params->Nzc;
  } else {
    g = params->U;
    for (r = params->Nzc - params->U * std::trunc(params->Nzc / params->U);
         r != 0.0; r = a - r * std::trunc(a / r)) {
      a = g;
      g = r;
    }
    if (g < 0.0) {
      g = -g;
    }
  }
  zc.set_size(1, n.size(1));
  yk = n.size(1);
  for (i = 0; i < yk; i++) {
    r = n[i];
    ai = r * y_im * (r + g);
    if (ai == 0.0) {
      zc[i].re = 0.0 / params->Nzc;
      zc[i].im = 0.0;
    } else {
      zc[i].re = 0.0;
      zc[i].im = ai / params->Nzc;
    }
  }
  nx = zc.size(1);
  for (k = 0; k < nx; k++) {
    if (zc[k].im == 0.0) {
      zc[k].re = std::exp(zc[k].re);
      zc[k].im = 0.0;
    } else {
      r = std::exp(zc[k].re / 2.0);
      zc[k].re = r * (r * std::cos(zc[k].im));
      zc[k].im = r * (r * std::sin(zc[k].im));
    }
  }
  //   %% -------------------
  //   %% 1) pad 0s to avoid inter-symbol interference
  //   %% -------------------
  //   %% 2) Up-sampling
  //   %%   convert to frequency
  if (zc.size(1) == 0) {
    fseq.set_size(1, 0);
  } else {
    b_sign = FFTW_FORWARD;
    howmany_iodims.n = 1;
    howmany_iodims.is = 1;
    howmany_iodims.os = 1;
    iodims.n = zc.size(1);
    iodims.is = 1;
    iodims.os = 1;
    fseq.set_size(1, zc.size(1));
    plan = fftw_plan_guru_dft(
        1, &iodims, 1, &howmany_iodims, (fftw_complex *)&zc[0],
        (fftw_complex *)&fseq[0], b_sign,
        (unsigned int)FFTW_PRESERVE_INPUT | (unsigned int)FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
  }
  //   %%   0-padding between positive frequency and negative frequency
  //   %%     positive freq:
  xPadded.set_size(1, loop_ub_tmp);
  for (i = 0; i < loop_ub_tmp; i++) {
    xPadded[i].re = 0.0;
    xPadded[i].im = 0.0;
  }
  nx = static_cast<int>(std::ceil(static_cast<double>(zc.size(1)) / 2.0));
  if (nx < 1) {
    yk = 0;
  } else {
    yk = nx;
  }
  for (i = 0; i < yk; i++) {
    xPadded[i] = fseq[i];
  }
  //   %%     negative freq:
  nx = (zc.size(1) - nx) - 1;
  i = fseq.size(1) - nx;
  if (i > fseq.size(1)) {
    i = 0;
    k = 0;
  } else {
    i--;
    k = fseq.size(1);
  }
  nx = xPadded.size(1) - nx;
  if (nx > xPadded.size(1)) {
    nx = 1;
  }
  yk = k - i;
  for (k = 0; k < yk; k++) {
    xPadded[(nx + k) - 1] = fseq[i + k];
  }
  y = &params->zc_up;
  if ((xPadded.size(1) == 0) || (static_cast<int>(Nzc_up) == 0)) {
    params->zc_up.set_size(1, loop_ub_tmp);
    for (i = 0; i < loop_ub_tmp; i++) {
      (*y)[i].re = 0.0;
      (*y)[i].im = 0.0;
    }
  } else if (xPadded.size(1) < static_cast<int>(Nzc_up)) {
    nx = xPadded.size(1);
    if (static_cast<int>(Nzc_up) < xPadded.size(1)) {
      nx = static_cast<int>(Nzc_up);
    }
    b_xPadded.set_size(1, loop_ub_tmp);
    for (i = 0; i < loop_ub_tmp; i++) {
      b_xPadded[i].re = 0.0;
      b_xPadded[i].im = 0.0;
    }
    if (nx < 1) {
      nx = 0;
    }
    b_y.set_size(1, nx);
    if (nx > 0) {
      b_y[0] = 1;
      yk = 1;
      for (k = 2; k <= nx; k++) {
        yk++;
        b_y[k - 1] = yk;
      }
    }
    b_r.set_size(b_y.size(1));
    yk = b_y.size(1);
    for (i = 0; i < yk; i++) {
      b_r[i] = b_y[i];
    }
    yk = b_r.size(0);
    for (i = 0; i < yk; i++) {
      b_xPadded[b_r[i] - 1] = xPadded[b_r[i] - 1];
    }
    b_sign = FFTW_BACKWARD;
    b_howmany_iodims.n = 1;
    b_howmany_iodims.is = 1;
    b_howmany_iodims.os = 1;
    b_iodims.n = static_cast<int>(Nzc_up);
    b_iodims.is = 1;
    b_iodims.os = 1;
    params->zc_up.set_size(1, loop_ub_tmp);
    plan = fftw_plan_guru_dft(
        1, &b_iodims, 1, &b_howmany_iodims, (fftw_complex *)&b_xPadded[0],
        (fftw_complex *)&params->zc_up[0], b_sign,
        (unsigned int)FFTW_PRESERVE_INPUT | (unsigned int)FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
    params->zc_up.set_size(1, params->zc_up.size(1));
    yk = params->zc_up.size(1) - 1;
    for (i = 0; i <= yk; i++) {
      r = (*y)[i].re;
      ai = (*y)[i].im;
      if (ai == 0.0) {
        a = r / static_cast<double>(static_cast<int>(Nzc_up));
        r = 0.0;
      } else if (r == 0.0) {
        a = 0.0;
        r = ai / static_cast<double>(static_cast<int>(Nzc_up));
      } else {
        a = r / static_cast<double>(static_cast<int>(Nzc_up));
        r = ai / static_cast<double>(static_cast<int>(Nzc_up));
      }
      (*y)[i].re = a;
      (*y)[i].im = r;
    }
  } else {
    b_sign = FFTW_BACKWARD;
    b_howmany_iodims.n = 1;
    b_howmany_iodims.is = 1;
    b_howmany_iodims.os = 1;
    b_iodims.n = static_cast<int>(Nzc_up);
    b_iodims.is = 1;
    b_iodims.os = 1;
    params->zc_up.set_size(1, loop_ub_tmp);
    plan = fftw_plan_guru_dft(
        1, &b_iodims, 1, &b_howmany_iodims, (fftw_complex *)&xPadded[0],
        (fftw_complex *)&params->zc_up[0], b_sign,
        (unsigned int)FFTW_PRESERVE_INPUT | (unsigned int)FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
    params->zc_up.set_size(1, params->zc_up.size(1));
    yk = params->zc_up.size(1) - 1;
    for (i = 0; i <= yk; i++) {
      r = (*y)[i].re;
      ai = (*y)[i].im;
      if (ai == 0.0) {
        a = r / static_cast<double>(static_cast<int>(Nzc_up));
        r = 0.0;
      } else if (r == 0.0) {
        a = 0.0;
        r = ai / static_cast<double>(static_cast<int>(Nzc_up));
      } else {
        a = r / static_cast<double>(static_cast<int>(Nzc_up));
        r = ai / static_cast<double>(static_cast<int>(Nzc_up));
      }
      (*y)[i].re = a;
      (*y)[i].im = r;
    }
  }
  //     %% -------------------
  //    %% 2) Up-sampling
  //    %%   convert to frequency
  //    fseq = fft(seq, Nseq);
  //    %%   0-padding between positive frequency and negative frequency
  //    %%     positive freq:
  //    idx = int32(ceil(Nseq/2));
  //    fseq_up = fseq(1:idx);
  //    %%     negative freq:
  //    idx = Nseq - idx;
  //    fseq_up(end-idx+1:end) = fseq(end-idx+1:end);
  //
  //    seq_up = ifft(fseq_up, Nseq_up);
  params->up_sample_factor = Nzc_up / down_sample_factor;
}

// End of code generation (genparams.cpp)
