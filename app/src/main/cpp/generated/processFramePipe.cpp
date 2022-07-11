//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// processFramePipe.cpp
//
// Code generation for function 'processFramePipe'
//

// Include files
#include "processFramePipe.h"
#include "Doppler_Compensation_01.h"
#include "backgroundSubtractFrame.h"
#include "calibrateFrame.h"
#include "cir2dist.h"
#include "circshift.h"
#include "colon.h"
#include "fft.h"
#include "genparams.h"
#include "hilbert.h"
#include "ifft.h"
#include "minOrMax.h"
#include "mod.h"
#include "normalize.h"
#include "pass2baseFrame.h"
#include "processFramePipe_data.h"
#include "processFramePipe_initialize.h"
#include "processFramePipe_internal_types.h"
#include "processFramePipe_rtwutil.h"
#include "coder_array.h"
#include <cmath>

// Variable Definitions
static boolean_T params_not_empty;

// Function Definitions
void processFramePipe(const double frame[480], double Nzc_up,
                      double down_sample_factor,
                      coder::array<creal_T, 2U> &dist, boolean_T *isValid)
{
  static coder::array<creal_T, 1U> lastRx;
  static coder::array<creal_T, 1U> static_vec;
  static coder::array<double, 1U> delta_phase;
  static ATParameters params;
  static double b_last_phase;
  static double moving_frame_counter;
  coder::array<creal_T, 2U> Re;
  coder::array<creal_T, 2U> b_r;
  coder::array<creal_T, 2U> r1;
  coder::array<creal_T, 2U> rxbb_up;
  coder::array<creal_T, 1U> b_rxbb_up;
  coder::array<creal_T, 1U> refsig;
  coder::array<creal_T, 1U> tmp_freq;
  coder::array<double, 2U> pdif_mat;
  coder::array<double, 2U> v_range;
  coder::array<double, 2U> y;
  creal_T rxbb[480];
  creal_T x[480];
  double b_re_tmp;
  double id_h;
  double r;
  double re_tmp;
  int i;
  int i1;
  int loop_ub;
  int nx;
  int ti;
  if (!isInitialized_processFramePipe) {
    processFramePipe_initialize();
  }
  if (!params_not_empty) {
    genparams(Nzc_up, down_sample_factor, &params);
    params_not_empty = true;
  }
  dist.set_size(1, static_cast<int>(params.up_sample_factor));
  loop_ub = static_cast<int>(params.up_sample_factor);
  for (i = 0; i < loop_ub; i++) {
    dist[i].re = 0.0;
    dist[i].im = 0.0;
  }
  //  assert(size(dist, 2) <= 480);
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%
  // %%% Section 1: Preprocess
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%
  //     %% STEP0: Compute Rxbb signal
  if (!delta_phase_not_empty) {
    if (params.Nzc_up < 1.0) {
      y.set_size(1, 0);
    } else {
      y.set_size(1, static_cast<int>(params.Nzc_up - 1.0) + 1);
      loop_ub = static_cast<int>(params.Nzc_up - 1.0);
      for (i = 0; i <= loop_ub; i++) {
        y[i] = static_cast<double>(i) + 1.0;
      }
    }
    delta_phase.set_size(y.size(1));
    loop_ub = y.size(1);
    for (i = 0; i < loop_ub; i++) {
      delta_phase[i] = y[i] * 2.0 * 3.1415926535897931 * 19000.0 / 48000.0;
    }
    delta_phase_not_empty = (delta_phase.size(0) != 0);
    b_last_phase = 0.0;
  }
  refsig.set_size(delta_phase.size(0));
  loop_ub = delta_phase.size(0);
  for (i = 0; i < loop_ub; i++) {
    refsig[i].re = 0.0;
    refsig[i].im = -(delta_phase[i] + b_last_phase);
  }
  nx = refsig.size(0);
  for (int k{0}; k < nx; k++) {
    if (refsig[k].im == 0.0) {
      refsig[k].re = std::exp(refsig[k].re);
      refsig[k].im = 0.0;
    } else {
      r = std::exp(refsig[k].re / 2.0);
      refsig[k].re = r * (r * std::cos(refsig[k].im));
      refsig[k].im = r * (r * std::sin(refsig[k].im));
    }
  }
  coder::hilbert(frame, rxbb);
  if (refsig.size(0) == 480) {
    for (i = 0; i < 480; i++) {
      r = rxbb[i].re;
      id_h = refsig[i].im;
      re_tmp = rxbb[i].im;
      b_re_tmp = refsig[i].re;
      rxbb[i].re = r * b_re_tmp - re_tmp * id_h;
      rxbb[i].im = r * id_h + re_tmp * b_re_tmp;
    }
  } else {
    times(rxbb, refsig);
  }
  params.frame_counter++;
  params.time_counter = params.frame_counter * params.Nzc_up / 48000.0;
  b_last_phase =
      std::atan2(refsig[refsig.size(0) - 1].im, refsig[refsig.size(0) - 1].re);
  //     %% STEP1: Determin motions
  // * 1. CALIBRATION for the first 1 second
  // * 2. DETERMINE MOTION
  calibrateFrame(rxbb, &params);
  //     %% STEP2: Compute and subtract background (for rxbb signal)
  // * 1. Update Static Vector if no moving is detected.
  // * 2. Subtract Static Vector
  if (!static_vec_not_empty) {
    nx = static_cast<int>(params.Nzc_up);
    static_vec.set_size(nx);
    for (i = 0; i < nx; i++) {
      static_vec[i].re = 0.0;
      static_vec[i].im = 0.0;
    }
    static_vec_not_empty = (static_vec.size(0) != 0);
    moving_frame_counter = 0.0;
  }
  if (params.caliSucceeded) {
    if (params.current_moving_frame) {
      if (static_vec.size(0) == 480) {
        loop_ub = static_vec.size(0);
        for (i = 0; i < loop_ub; i++) {
          x[i].re = moving_frame_counter * static_vec[i].re + rxbb[i].re;
          x[i].im = moving_frame_counter * static_vec[i].im + rxbb[i].im;
        }
      } else {
        binary_expand_op(x, moving_frame_counter, static_vec, rxbb);
      }
      r = moving_frame_counter + 1.0;
      moving_frame_counter++;
      static_vec.set_size(480);
      for (i = 0; i < 480; i++) {
        id_h = x[i].re;
        re_tmp = x[i].im;
        if (re_tmp == 0.0) {
          static_vec[i].re = id_h / r;
          static_vec[i].im = 0.0;
        } else if (id_h == 0.0) {
          static_vec[i].re = 0.0;
          static_vec[i].im = re_tmp / r;
        } else {
          static_vec[i].re = id_h / r;
          static_vec[i].im = re_tmp / r;
        }
      }
      static_vec_not_empty = true;
    }
    if (!params.backgroundInitSucceeded) {
      if ((moving_frame_counter > 24000.0 / params.Nzc_up) &&
          (!params.current_moving_frame)) {
        params.backgroundInitSucceeded = true;
        if (static_vec.size(0) == 480) {
          for (i = 0; i < 480; i++) {
            rxbb[i].re -= static_vec[i].re;
            rxbb[i].im -= static_vec[i].im;
          }
        } else {
          minus(rxbb, static_vec);
        }
        //  fprintf('Background initialization succeed in the %ith frame\n',
        //  params.frame_counter);
      }

      //  Subtract the static vector
    } else if (static_vec.size(0) == 480) {
      for (i = 0; i < 480; i++) {
        rxbb[i].re -= static_vec[i].re;
        rxbb[i].im -= static_vec[i].im;
      }
    } else {
      minus(rxbb, static_vec);
    }
  }
  //     %% STEP3: Buffer management
  // * 1. generate rxbb with up-sampling scheme
  rxbb_up.set_size(static_cast<int>(params.Nzc_up),
                   static_cast<int>(params.up_sample_factor));
  nx = static_cast<int>(params.Nzc_up) *
       static_cast<int>(params.up_sample_factor);
  for (i = 0; i < nx; i++) {
    rxbb_up[i].re = 0.0;
    rxbb_up[i].im = 0.0;
  }
  if (params.backgroundInitSucceeded) {
    if (!frame_counter_not_empty) {
      frame_counter_not_empty = true;
      lastRx.set_size(static_cast<int>(params.Nzc_up));
      loop_ub = static_cast<int>(params.Nzc_up);
      for (i = 0; i < loop_ub; i++) {
        lastRx[i].re = 0.0;
        lastRx[i].im = 0.0;
      }
    }
    if (frame_counter > 1.0) {
      refsig.set_size(lastRx.size(0) + 480);
      loop_ub = lastRx.size(0);
      for (i = 0; i < loop_ub; i++) {
        refsig[i] = lastRx[i];
      }
      for (i = 0; i < 480; i++) {
        refsig[i + lastRx.size(0)] = rxbb[i];
      }
      i = rxbb_up.size(1);
      for (ti = 0; ti < i; ti++) {
        if (params.Nzc_up < 1.0) {
          y.set_size(1, 0);
        } else {
          y.set_size(1, static_cast<int>(params.Nzc_up - 1.0) + 1);
          loop_ub = static_cast<int>(params.Nzc_up - 1.0);
          for (i1 = 0; i1 <= loop_ub; i1++) {
            y[i1] = static_cast<double>(i1) + 1.0;
          }
        }
        r = ((static_cast<double>(ti) + 1.0) - 1.0) * params.down_sample_factor;
        loop_ub = y.size(1);
        for (i1 = 0; i1 < loop_ub; i1++) {
          rxbb_up[i1 + rxbb_up.size(0) * ti] =
              refsig[static_cast<int>(r + y[i1]) - 1];
        }
      }
      params.bufferInitSucceeded = true;
    }
    lastRx.set_size(480);
    for (i = 0; i < 480; i++) {
      lastRx[i] = rxbb[i];
    }
    frame_counter++;
  }
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%
  // %%% Section 2: Core algorithm of Sensing fast movement
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%
  //     %% STEP4: Doppler Compensation
  Re.set_size(static_cast<int>(params.Nzc_up),
              static_cast<int>(params.up_sample_factor));
  for (i = 0; i < nx; i++) {
    Re[i].re = 0.0;
    Re[i].im = 0.0;
  }
  if (params.bufferInitSucceeded) {
    i = static_cast<int>(params.up_sample_factor);
    for (int zi{0}; zi < i; zi++) {
      double varargin_1_data[200];
      int i2;
      //  vmax = 34300/20e3 / params.Nzc_up * 48e3/2;
      if (vest + 50.0 < vest - 50.0) {
        v_range.set_size(1, 0);
      } else if (std::floor(vest - 50.0) == vest - 50.0) {
        loop_ub = static_cast<int>(((vest + 50.0) - (vest - 50.0)) / 10.0);
        v_range.set_size(1, loop_ub + 1);
        for (i1 = 0; i1 <= loop_ub; i1++) {
          v_range[i1] = (vest - 50.0) + 10.0 * static_cast<double>(i1);
        }
      } else {
        coder::eml_float_colon(vest - 50.0, vest + 50.0, v_range);
      }
      pdif_mat.set_size(1, v_range.size(1));
      loop_ub = v_range.size(1);
      for (i1 = 0; i1 < loop_ub; i1++) {
        pdif_mat[i1] = 0.0;
      }
      //  velcoity search
      b_r.set_size(1, params.zc_up.size(1));
      loop_ub = params.zc_up.size(1);
      for (i1 = 0; i1 < loop_ub; i1++) {
        b_r[i1] = params.zc_up[i1];
      }
      coder::circshift(b_r, coder::b_mod(-(b_frame_counter - 1.0) *
                                             params.down_sample_factor,
                                         params.Nzc_up));
      coder::fft(b_r, r1);
      tmp_freq.set_size(r1.size(1));
      loop_ub = r1.size(1);
      for (i1 = 0; i1 < loop_ub; i1++) {
        tmp_freq[i1].re = r1[i1].re;
        tmp_freq[i1].im = -r1[i1].im;
      }
      i1 = v_range.size(1);
      for (int bi{0}; bi < i1; bi++) {
        if (params.Nzc_up - 1.0 < 0.0) {
          y.set_size(1, 0);
        } else {
          y.set_size(1, static_cast<int>(params.Nzc_up - 1.0) + 1);
          loop_ub = static_cast<int>(params.Nzc_up - 1.0);
          for (i2 = 0; i2 <= loop_ub; i2++) {
            y[i2] = i2;
          }
        }
        r = (19000.0 - (34300.0 - v_range[bi]) / 34300.0 * 19000.0) *
            6.2831853071795862;
        refsig.set_size(y.size(1));
        loop_ub = y.size(1);
        for (i2 = 0; i2 < loop_ub; i2++) {
          re_tmp = r * y[i2];
          if (re_tmp == 0.0) {
            refsig[i2].re = 0.0;
            refsig[i2].im = 0.0;
          } else {
            refsig[i2].re = 0.0;
            refsig[i2].im = re_tmp / 48000.0;
          }
        }
        nx = refsig.size(0);
        for (int k{0}; k < nx; k++) {
          if (refsig[k].im == 0.0) {
            refsig[k].re = std::exp(refsig[k].re);
            refsig[k].im = 0.0;
          } else {
            r = std::exp(refsig[k].re / 2.0);
            refsig[k].re = r * (r * std::cos(refsig[k].im));
            refsig[k].im = r * (r * std::sin(refsig[k].im));
          }
        }
        //  keyboard
        loop_ub = rxbb_up.size(0);
        if (rxbb_up.size(0) == refsig.size(0)) {
          b_rxbb_up.set_size(rxbb_up.size(0));
          for (i2 = 0; i2 < loop_ub; i2++) {
            b_re_tmp = rxbb_up[i2 + rxbb_up.size(0) * zi].re;
            r = refsig[i2].im;
            id_h = rxbb_up[i2 + rxbb_up.size(0) * zi].im;
            re_tmp = refsig[i2].re;
            b_rxbb_up[i2].re = b_re_tmp * re_tmp - id_h * r;
            b_rxbb_up[i2].im = b_re_tmp * r + id_h * re_tmp;
          }
          coder::fft(b_rxbb_up, refsig);
        } else {
          binary_expand_op(refsig, rxbb_up, zi);
        }
        if (refsig.size(0) == 1) {
          i2 = tmp_freq.size(0);
        } else {
          i2 = refsig.size(0);
        }
        if ((refsig.size(0) == tmp_freq.size(0)) &&
            (i2 == params.inv_freq.size(0))) {
          b_rxbb_up.set_size(refsig.size(0));
          loop_ub = refsig.size(0);
          for (i2 = 0; i2 < loop_ub; i2++) {
            r = refsig[i2].re;
            id_h = tmp_freq[i2].im;
            re_tmp = refsig[i2].im;
            b_re_tmp = tmp_freq[i2].re;
            b_rxbb_up[i2].re =
                params.inv_freq[i2] * (r * b_re_tmp - re_tmp * id_h);
            b_rxbb_up[i2].im =
                params.inv_freq[i2] * (r * id_h + re_tmp * b_re_tmp);
          }
          coder::ifft(b_rxbb_up, refsig);
        } else {
          binary_expand_op(refsig, tmp_freq, &params);
        }
        //  maximum correlation peak
        if (b_frame_counter == 1.0) {
          for (int k{0}; k < 151; k++) {
            varargin_1_data[k] =
                rt_hypotd(refsig[k + 49].re, refsig[k + 49].im);
          }
          coder::internal::maximum(varargin_1_data, 151, &pdif_mat[bi], &nx);
        } else {
          r = last_idmax - 10.0;
          id_h = last_idmax + 10.0;
          if (last_idmax - 10.0 < 1.0) {
            r = 1.0;
          }
          if (last_idmax + 10.0 > 200.0) {
            id_h = 200.0;
          }
          if (r > id_h) {
            i2 = 0;
            nx = -1;
          } else {
            i2 = static_cast<int>(r) - 1;
            nx = static_cast<int>(id_h) - 1;
          }
          loop_ub = nx - i2;
          nx = loop_ub + 1;
          for (int k{0}; k <= loop_ub; k++) {
            ti = i2 + k;
            varargin_1_data[k] = rt_hypotd(refsig[ti].re, refsig[ti].im);
          }
          if (loop_ub + 1 <= 2) {
            if (loop_ub + 1 == 1) {
              pdif_mat[bi] = varargin_1_data[0];
            } else if (varargin_1_data[0] < varargin_1_data[loop_ub]) {
              pdif_mat[bi] = varargin_1_data[loop_ub];
            } else {
              pdif_mat[bi] = varargin_1_data[0];
            }
          } else {
            r = varargin_1_data[0];
            for (int k{2}; k <= nx; k++) {
              b_re_tmp = varargin_1_data[k - 1];
              if (r < b_re_tmp) {
                r = b_re_tmp;
              }
            }
            pdif_mat[bi] = r;
          }
        }
      }
      //  pdif_mat = smoothdata(pdif_mat, 'movmean', 10);
      //  maximum peak
      //  pdif_mat = (pdif_mat - min(pdif_mat)) / (max(pdif_mat) -
      //  min(pdif_mat));
      y.set_size(1, pdif_mat.size(1));
      loop_ub = pdif_mat.size(0) * pdif_mat.size(1) - 1;
      for (i1 = 0; i1 <= loop_ub; i1++) {
        y[i1] = pdif_mat[i1];
      }
      coder::normalize(y, pdif_mat);
      if (b_frame_counter >= 2.0) {
        nx = pdif_mat.size(1);
        if (pdif_mat.size(1) <= 2) {
          if (pdif_mat.size(1) == 1) {
            ti = 1;
          } else if (pdif_mat[0] < pdif_mat[pdif_mat.size(1) - 1]) {
            ti = pdif_mat.size(1);
          } else {
            ti = 1;
          }
        } else {
          r = pdif_mat[0];
          ti = 1;
          for (int k{2}; k <= nx; k++) {
            b_re_tmp = pdif_mat[k - 1];
            if (r < b_re_tmp) {
              r = b_re_tmp;
              ti = k;
            }
          }
        }
        vest = v_range[ti - 1];
      } else {
        vest = 0.0;
      }
      //  compensate Doppler effect
      if (params.Nzc_up - 1.0 < 0.0) {
        y.set_size(1, 0);
      } else {
        y.set_size(1, static_cast<int>(params.Nzc_up - 1.0) + 1);
        loop_ub = static_cast<int>(params.Nzc_up - 1.0);
        for (i1 = 0; i1 <= loop_ub; i1++) {
          y[i1] = i1;
        }
      }
      r = (19000.0 - (34300.0 - vest) / 34300.0 * 19000.0) * 6.2831853071795862;
      refsig.set_size(y.size(1));
      loop_ub = y.size(1);
      for (i1 = 0; i1 < loop_ub; i1++) {
        re_tmp = r * y[i1];
        if (re_tmp == 0.0) {
          refsig[i1].re = 0.0;
          refsig[i1].im = 0.0;
        } else {
          refsig[i1].re = 0.0;
          refsig[i1].im = re_tmp / 48000.0;
        }
      }
      nx = refsig.size(0);
      for (int k{0}; k < nx; k++) {
        if (refsig[k].im == 0.0) {
          refsig[k].re = std::exp(refsig[k].re);
          refsig[k].im = 0.0;
        } else {
          r = std::exp(refsig[k].re / 2.0);
          refsig[k].re = r * (r * std::cos(refsig[k].im));
          refsig[k].im = r * (r * std::sin(refsig[k].im));
        }
      }
      //  keyboard
      loop_ub = rxbb_up.size(0);
      if (rxbb_up.size(0) == refsig.size(0)) {
        b_rxbb_up.set_size(rxbb_up.size(0));
        for (i1 = 0; i1 < loop_ub; i1++) {
          b_re_tmp = rxbb_up[i1 + rxbb_up.size(0) * zi].re;
          r = refsig[i1].im;
          id_h = rxbb_up[i1 + rxbb_up.size(0) * zi].im;
          re_tmp = refsig[i1].re;
          b_rxbb_up[i1].re = b_re_tmp * re_tmp - id_h * r;
          b_rxbb_up[i1].im = b_re_tmp * r + id_h * re_tmp;
        }
        coder::fft(b_rxbb_up, refsig);
      } else {
        binary_expand_op(refsig, rxbb_up, zi);
      }
      if (refsig.size(0) == 1) {
        i1 = tmp_freq.size(0);
      } else {
        i1 = refsig.size(0);
      }
      if ((refsig.size(0) == tmp_freq.size(0)) &&
          (i1 == params.inv_freq.size(0))) {
        tmp_freq.set_size(refsig.size(0));
        loop_ub = refsig.size(0);
        for (i1 = 0; i1 < loop_ub; i1++) {
          r = refsig[i1].re;
          id_h = tmp_freq[i1].im;
          re_tmp = refsig[i1].im;
          b_re_tmp = tmp_freq[i1].re;
          tmp_freq[i1].re =
              params.inv_freq[i1] * (r * b_re_tmp - re_tmp * id_h);
          tmp_freq[i1].im =
              params.inv_freq[i1] * (r * id_h + re_tmp * b_re_tmp);
        }
        coder::ifft(tmp_freq, refsig);
      } else {
        binary_expand_op(refsig, tmp_freq, &params);
      }
      if (b_frame_counter == 1.0) {
        for (int k{0}; k < 150; k++) {
          varargin_1_data[k] = rt_hypotd(refsig[k + 50].re, refsig[k + 50].im);
        }
        coder::internal::maximum(varargin_1_data, 150, &r, &ti);
        last_idmax = ti;
        last_idmax = ((last_idmax + 1.0) + 50.0) - 1.0;
      } else {
        r = last_idmax - 10.0;
        id_h = last_idmax + 10.0;
        if (last_idmax - 10.0 < 1.0) {
          r = 1.0;
        }
        if (last_idmax + 10.0 > 200.0) {
          id_h = 200.0;
        }
        if (r > id_h) {
          i1 = 0;
          i2 = -1;
        } else {
          i1 = static_cast<int>(r) - 1;
          i2 = static_cast<int>(id_h) - 1;
        }
        loop_ub = i2 - i1;
        for (int k{0}; k <= loop_ub; k++) {
          ti = i1 + k;
          varargin_1_data[k] = rt_hypotd(refsig[ti].re, refsig[ti].im);
        }
        coder::internal::maximum(varargin_1_data, loop_ub + 1, &r, &ti);
        last_idmax = ((static_cast<double>(ti) + last_idmax) - 10.0) - 1.0;
        if (last_idmax < 1.0) {
          last_idmax = 1.0;
        }
        if (last_idmax > 200.0) {
          last_idmax = 200.0;
        }
      }
      b_frame_counter++;
      loop_ub = refsig.size(0);
      for (i1 = 0; i1 < loop_ub; i1++) {
        Re[i1 + Re.size(0) * zi] = refsig[i1];
      }
    }
  }
  //     %% STEP5: Phase derivative
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%
  // %%% Section 3: Generate Motion
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%
  //     %% STEP6: Extract movement information
  if (params.bufferInitSucceeded) {
    cir2dist(Re, y);
    dist.set_size(1, y.size(1));
    loop_ub = y.size(1);
    for (i = 0; i < loop_ub; i++) {
      dist[i].re = y[i];
      dist[i].im = 0.0;
    }
  }
  *isValid = params.bufferInitSucceeded;
}

void processFramePipe_init()
{
  params_not_empty = false;
}

// End of code generation (processFramePipe.cpp)
