//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// ifft.cpp
//
// Code generation for function 'ifft'
//

// Include files
#include "ifft.h"
#include "processFramePipe_data.h"
#include "coder_array.h"
#include "fftw3.h"

// Function Definitions
namespace coder {
void ifft(const creal_T x[480], creal_T y[480])
{
  fftw_plan plan;
  int b_sign;
  fftw_iodim howmany_iodims;
  fftw_iodim iodims;
  b_sign = FFTW_BACKWARD;
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
  for (int i{0}; i < 480; i++) {
    double ai;
    double im;
    double re;
    im = y[i].re;
    ai = y[i].im;
    if (ai == 0.0) {
      re = im / 480.0;
      im = 0.0;
    } else if (im == 0.0) {
      re = 0.0;
      im = ai / 480.0;
    } else {
      re = im / 480.0;
      im = ai / 480.0;
    }
    y[i].re = re;
    y[i].im = im;
  }
}

void ifft(const ::coder::array<creal_T, 1U> &x, ::coder::array<creal_T, 1U> &y)
{
  fftw_iodim howmany_iodims;
  fftw_iodim iodims;
  if (x.size(0) == 0) {
    y.set_size(0);
  } else {
    fftw_plan plan;
    int b_sign;
    int loop_ub;
    int x_re;
    b_sign = FFTW_BACKWARD;
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
    x_re = x.size(0);
    loop_ub = y.size(0);
    for (int i{0}; i < loop_ub; i++) {
      double ai;
      double im;
      double re;
      im = y[i].re;
      ai = y[i].im;
      if (ai == 0.0) {
        re = im / static_cast<double>(x_re);
        im = 0.0;
      } else if (im == 0.0) {
        re = 0.0;
        im = ai / static_cast<double>(x_re);
      } else {
        re = im / static_cast<double>(x_re);
        im = ai / static_cast<double>(x_re);
      }
      y[i].re = re;
      y[i].im = im;
    }
  }
}

} // namespace coder

// End of code generation (ifft.cpp)
