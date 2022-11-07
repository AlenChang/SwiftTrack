//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// fft.cpp
//
// Code generation for function 'fft'
//

// Include files
#include "fft.h"
#include "coder_array.h"
#include "fftw3.h"

// Function Definitions
namespace coder {
void fft(const creal_T x[480], creal_T y[480])
{
  fftw_plan plan;
  int b_sign;
  fftw_iodim howmany_iodims;
  fftw_iodim iodims;
  b_sign = FFTW_FORWARD;
  iodims.n = 480;
  iodims.is = 1;
  iodims.os = 1;
  howmany_iodims.n = 1;
  howmany_iodims.is = 480;
  howmany_iodims.os = 480;
  plan = fftw_plan_guru_dft(
      1, &iodims, 1, &howmany_iodims, (fftw_complex *)&x[0],
      (fftw_complex *)&y[0], b_sign,
      (unsigned int)FFTW_PRESERVE_INPUT | (unsigned int)FFTW_ESTIMATE);
  fftw_execute(plan);
  fftw_destroy_plan(plan);
}

void fft(const ::coder::array<creal_T, 2U> &x, ::coder::array<creal_T, 2U> &y)
{
  fftw_iodim howmany_iodims;
  fftw_iodim iodims;
  if (x.size(1) == 0) {
    y.set_size(1, 0);
  } else {
    fftw_plan plan;
    int b_sign;
    b_sign = FFTW_FORWARD;
    howmany_iodims.n = 1;
    howmany_iodims.is = 1;
    howmany_iodims.os = 1;
    iodims.n = x.size(1);
    iodims.is = 1;
    iodims.os = 1;
    y.set_size(1, x.size(1));
    plan = fftw_plan_guru_dft(
        1, &iodims, 1, &howmany_iodims, (fftw_complex *)&x[0],
        (fftw_complex *)&y[0], b_sign,
        (unsigned int)FFTW_PRESERVE_INPUT | (unsigned int)FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
  }
}

void fft(const ::coder::array<creal_T, 1U> &x, ::coder::array<creal_T, 1U> &y)
{
  fftw_iodim howmany_iodims;
  fftw_iodim iodims;
  if (x.size(0) == 0) {
    y.set_size(0);
  } else {
    fftw_plan plan;
    int b_sign;
    b_sign = FFTW_FORWARD;
    iodims.n = x.size(0);
    iodims.is = 1;
    iodims.os = 1;
    howmany_iodims.n = 1;
    howmany_iodims.is = x.size(0);
    howmany_iodims.os = x.size(0);
    y.set_size(x.size(0));
    plan = fftw_plan_guru_dft(
        1, &iodims, 1, &howmany_iodims,
        (fftw_complex *)&(((::coder::array<creal_T, 1U> *)&x)->data())[0],
        (fftw_complex *)&(y.data())[0], b_sign,
        (unsigned int)FFTW_PRESERVE_INPUT | (unsigned int)FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
  }
}

} // namespace coder

// End of code generation (fft.cpp)
