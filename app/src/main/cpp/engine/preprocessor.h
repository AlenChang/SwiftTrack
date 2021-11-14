#ifndef ACOUSTICTRACKER_PREPROCESSOR_H
#define ACOUSTICTRACKER_PREPROCESSOR_H

#include "filter_util.hpp"
#include "matrix_util.hpp"
#include "NoiseSupression.h"


#define M_PI		3.14159265358979323846

class Preprocessor {
public:
    Preprocessor();

    ~Preprocessor();

    MatrixX<complex<double>> GenerateCIRSignal(const MatrixX<double> &rx_signal);

private:
    void GenerateRefSignal();

    void DownConversion(const MatrixX<double> &rx_signal);

    void LowPassFilter();

    void CircularConvolution();

    void CenterShift();

    const int N_ZC = 59;
    const int N_ZC_UP = 480;
    const int U = 1;
    const double FC = 20000.0;
    const double B = 6000.0;
    const double FS = 48000.0;
    const double DELTA_PHASE = -2 * M_PI * FC / FS;


    double phase_;
    int center_tap_;

    void addWindow();
    void genWindow(double win[480]);

    MatrixX<complex<double>> freq_ref_signal_;
    MatrixX<complex<double>> rxbb_signal_;
    MatrixX<complex<double>> filtered_signal_;
    MatrixX<complex<double>> cir_signal_;
};

#endif
