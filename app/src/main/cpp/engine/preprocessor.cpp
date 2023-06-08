#include "include/preprocessor.h"
#include "logger_util.hpp"


Preprocessor::Preprocessor(int id, int N_ZC_UP_, int FC_, int BW_) {
    N_ZC_UP = N_ZC_UP_;
    FC = FC_ * 1.0;
    B = BW_ * 1.0;
    N_ZC = N_ZC_UP * B / FS - 1;
    U = (int) (N_ZC - 1) / 2;
    phase_ = 0.0;
    center_tap_ = -1;
    DELTA_PHASE = -2 * M_PI * FC / FS;

    engine_id = id;

    

    freq_ref_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
    rxbb_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
    filtered_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
    cir_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));

    GenerateRefSignal();

    LoggerUtil::Log("SwifTrack", "Preprocessor is constructed.");
}

Preprocessor::~Preprocessor() {
    LoggerUtil::Log("SwifTrack", "Preprocessor is destructed.");
}

MatrixX<complex<double>> Preprocessor::GenerateCIRSignal(const MatrixX<double> &rx_signal) {
    // perform down-conversion by cumulating phase
    DownConversion(rx_signal);

    LowPassFilter();

    CircularConvolution();

    // shift the direct path to the first tap
    CenterShift();
    

    return cir_signal_;
}

void Preprocessor::GenerateRefSignal() {
    // Generate raw zc sequence
    MatrixX<complex<double>> raw_zc(1, N_ZC);
    for (int i = 0; i < N_ZC; i++) {
        raw_zc(0, i) = exp(complex<double>(0, -M_PI * U * i * (i + 1) / N_ZC));
    }

    // Transfer raw zc to freq domain
    MatrixX<complex<double>> freq_zc(1, N_ZC);
    MatrixUtil::RowFFT(freq_zc, raw_zc);

    // Padding zeros in freq domain
    // Perform conjugation when padding
    MatrixUtil::FreqPadding(freq_ref_signal_, freq_zc, N_ZC_UP - N_ZC);
//    if( N_ZC_UP == 480){
//        addWindow();
//    }
    
}

void Preprocessor::DownConversion(const MatrixX<double> &rx_signal) {

    // for channel 0
    for (int i = 0; i < N_ZC_UP; i++) {
        phase_ += DELTA_PHASE;
        rxbb_signal_(0, i) = rx_signal(0, i) * complex<double>(cos(phase_), sin(phase_));
    }
    phase_ = fmod(phase_, 2.0 * M_PI);
}

void Preprocessor::LowPassFilter() {
//    FilterUtil::Filter(filtered_signal_, rxbb_signal_);
     filtered_signal_ = rxbb_signal_;
}

void Preprocessor::CircularConvolution() {
    MatrixX<complex<double>> freq_filtered_signal(1, N_ZC_UP);
    MatrixUtil::RowFFT(freq_filtered_signal, filtered_signal_);
    // MatrixX<complex<double>> conj_data = freq_ref_signal_;
    MatrixUtil::RowIFFT(cir_signal_, MatrixUtil::Dot(freq_filtered_signal, freq_ref_signal_));
}

void Preprocessor::CenterShift() {
    if (center_tap_ < 0) {
        MatrixX<double> abs_cir_signal(1, N_ZC_UP);
        MatrixUtil::ComplexToAbs(abs_cir_signal, cir_signal_);
        center_tap_ = MatrixUtil::RowArgmax(abs_cir_signal)[0];
    }

    MatrixUtil::CyclicShift(cir_signal_, 0, center_tap_);
}

void Preprocessor::addWindow(){
    double win[480];
    genWindow(win);
    for (int i = 0; i < N_ZC_UP; i++) {
        freq_ref_signal_(0, i) = freq_ref_signal_(0, i) * win[i];
    }
}

void Preprocessor::genWindow(double win[480])
{
  // Call the entry-point 'genWindow'.
//  NoiseSupression::genWindow(win);
}

MatrixX<complex<double>> Preprocessor::getCIRSignal(){
    return cir_signal_;
}
