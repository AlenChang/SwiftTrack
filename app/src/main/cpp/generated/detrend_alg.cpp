//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// detrend_alg.cpp
//
// Code generation for function 'detrend_alg'
//

// Include files
#include "detrend_alg.h"
#include "fftw3.h"

// Function Definitions
void detrend_alg(double data[500])
{
  fftw_plan plan;
  fftw_iodim howmany_iodims;
  fftw_iodim iodims;
  creal_T datafreq[500];
  creal_T y[500];
  int ySize_idx_0;
  // codegen
  iodims.n = 500;
  iodims.is = 1;
  iodims.os = 1;
  howmany_iodims.n = 1;
  howmany_iodims.is = 500;
  howmany_iodims.os = 500;
  plan = fftw_plan_guru_dft_r2c(
      1, &iodims, 1, &howmany_iodims, (double *)&data[0],
      (fftw_complex *)&datafreq[0],
      (unsigned int)FFTW_PRESERVE_INPUT | (unsigned int)FFTW_ESTIMATE);
  fftw_execute(plan);
  fftw_destroy_plan(plan);
  ySize_idx_0 = 252;
  int conjIndex_idx_0;
  do {
    conjIndex_idx_0 = 502 - ySize_idx_0;
    if (502 - ySize_idx_0 == 501) {
      conjIndex_idx_0 = 1;
    }
    datafreq[ySize_idx_0 - 1].re = datafreq[conjIndex_idx_0 - 1].re;
    datafreq[ySize_idx_0 - 1].im = -datafreq[conjIndex_idx_0 - 1].im;
    ySize_idx_0++;
  } while (!(ySize_idx_0 == 501));
  datafreq[0].re = 0.0;
  datafreq[0].im = 0.0;
  iodims.n = 500;
  iodims.is = 1;
  iodims.os = 1;
  howmany_iodims.n = 1;
  howmany_iodims.is = 500;
  howmany_iodims.os = 500;
  plan = fftw_plan_guru_dft(
      1, &iodims, 1, &howmany_iodims, (fftw_complex *)&datafreq[0],
      (fftw_complex *)&y[0], FFTW_BACKWARD,
      (unsigned int)FFTW_PRESERVE_INPUT | (unsigned int)FFTW_ESTIMATE);
  fftw_execute(plan);
  fftw_destroy_plan(plan);
  for (ySize_idx_0 = 0; ySize_idx_0 < 500; ySize_idx_0++) {
    double ai;
    double im;
    double re;
    im = y[ySize_idx_0].re;
    ai = y[ySize_idx_0].im;
    if (ai == 0.0) {
      re = im / 500.0;
      im = 0.0;
    } else if (im == 0.0) {
      re = 0.0;
      im = ai / 500.0;
    } else {
      re = im / 500.0;
      im = ai / 500.0;
    }
    y[ySize_idx_0].re = re;
    y[ySize_idx_0].im = im;
    data[ySize_idx_0] = re;
  }
  //      wdata = wdenoise(data);
  //      data = data - min(data);
  //      data = data / max(data);
}

// End of code generation (detrend_alg.cpp)
