#ifndef ACOUSTICTRACKER_PREPROCESSOR_H
#define ACOUSTICTRACKER_PREPROCESSOR_H

#include "filter_util.hpp"
#include "matrix_util.hpp"
//#include "NoiseSupression.h"



#define M_PI		3.14159265358979323846

class Preprocessor {
public:
    Preprocessor(int N_ZC_UP_, int FC_, int BW_);

    ~Preprocessor();

    MatrixX<complex<double>> GenerateCIRSignal(const MatrixX<double> &rx_signal);

private:
    void GenerateRefSignal();

    void DownConversion(const MatrixX<double> &rx_signal);

    void LowPassFilter();

    void CircularConvolution();

    void CenterShift();

    int N_ZC;
    int N_ZC_UP;
    int U;
    double FC;
    double B;
    double FS = 48000.0;
    double DELTA_PHASE;


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
