#include "preprocessor.h"

Preprocessor::Preprocessor() {
    phase_ = 0.0;
    center_tap_ = -1;

    freq_ref_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
    rxbb_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
    filtered_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
    cir_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));

    GenerateRefSignal();

    cout << "Preprocessor was initiated." << endl;
}

Preprocessor::~Preprocessor() {
    cout << "Preprocessor was recycled." << endl;
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
}

void Preprocessor::DownConversion(const MatrixX<double> &rx_signal) {

    // for channel 0
    for (int i = 0; i < N_ZC_UP; i++) {
        phase_ += DELTA_PHASE;
        rxbb_signal_(0, i) = rx_signal(0, i) * complex<double>(cos(phase_), sin(phase_));
    }
}

void Preprocessor::LowPassFilter() {
//    FilterUtil::Filter(filtered_signal_, rxbb_signal_);
     filtered_signal_ = rxbb_signal_;
}

void Preprocessor::CircularConvolution() {
    MatrixX<complex<double>> freq_filtered_signal(1, N_ZC_UP);
    MatrixUtil::RowFFT(freq_filtered_signal, filtered_signal_);
    MatrixX<complex<double>> conj_data = freq_filtered_signal.conjugate();
    MatrixUtil::RowIFFT(cir_signal_, MatrixUtil::Dot(conj_data, freq_ref_signal_));
}

void Preprocessor::CenterShift() {
    if (center_tap_ < 0) {
        MatrixX<double> abs_cir_signal(1, N_ZC_UP);
        MatrixUtil::ComplexToAbs(abs_cir_signal, cir_signal_);
        center_tap_ = MatrixUtil::RowArgmax(abs_cir_signal)[0];
    }

    MatrixUtil::CyclicShift(cir_signal_, 0, center_tap_);
}
