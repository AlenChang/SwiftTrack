//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// hilbert.cpp
//
// Code generation for function 'hilbert'
//

// Include files
#include "hilbert.h"
#include "ifft.h"
#include "fftw3.h"
#include <algorithm>

// Function Definitions
namespace coder {
void hilbert(const double xr[480], creal_T x[480])
{
  fftw_plan plan;
  fftw_iodim howmany_iodims;
  fftw_iodim iodims;
  creal_T b_x[480];
  int ySize_idx_0;
  iodims.n = 480;
  iodims.is = 1;
  iodims.os = 1;
  howmany_iodims.n = 1;
  howmany_iodims.is = 480;
  howmany_iodims.os = 480;
  plan = fftw_plan_guru_dft_r2c(
      1, &iodims, 1, &howmany_iodims, (double *)&xr[0], (fftw_complex *)&x[0],
      (unsigned int)FFTW_PRESERVE_INPUT | (unsigned int)FFTW_ESTIMATE);
  fftw_execute(plan);
  fftw_destroy_plan(plan);
  ySize_idx_0 = 242;
  int conjIndex_idx_0;
  do {
    conjIndex_idx_0 = 482 - ySize_idx_0;
    if (482 - ySize_idx_0 == 481) {
      conjIndex_idx_0 = 1;
    }
    x[ySize_idx_0 - 1].re = x[conjIndex_idx_0 - 1].re;
    x[ySize_idx_0 - 1].im = -x[conjIndex_idx_0 - 1].im;
    ySize_idx_0++;
  } while (!(ySize_idx_0 == 481));
  for (ySize_idx_0 = 0; ySize_idx_0 < 239; ySize_idx_0++) {
    x[ySize_idx_0 + 1].re *= 2.0;
    x[ySize_idx_0 + 1].im *= 2.0;
    x[ySize_idx_0 + 241].re = 0.0;
    x[ySize_idx_0 + 241].im = 0.0;
  }
  std::copy(&x[0], &x[480], &b_x[0]);
  ifft(b_x, x);
}

} // namespace coder

// End of code generation (hilbert.cpp)
