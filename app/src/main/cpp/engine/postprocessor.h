#ifndef ACOUSTICTRACKER_POSTPROCESSOR_H
#define ACOUSTICTRACKER_POSTPROCESSOR_H

// #include "logger_util.hpp"
#include "matrix_util.hpp"
#include "histories.hpp"
#include "NoiseSupression.h"
#include "denoiser.h"
#include <stdio.h>

#define M_PI		3.14159265358979323846

class Postprocessor {
  
public:
    Postprocessor(int N_ZC_UP_);

    ~Postprocessor();

    double ProcessCIRSignal(const MatrixX<complex<double>> &cir_signal, bool is_moving);

    // operating histories
    Histories & GetHistories(int history_type);

    void PaddingZero();

    void GetCIR(double *cir_abs, int n);

    void GetBeta(double* beta_real, double* beta_imag);

    void GetHistoryData(double *history, int n, Histories &history_type, HistoryType h_type);

private:
    void CutOffCIRSignal(const MatrixX<complex<double>> &cir_signal);

    void CalcPhase();

    double CalcPhase(complex<double> tapSel, double & pre_phase);

    complex<double> LeastSquare();

    void PhaseTransform();

    void TapSelectionTOF();

    void BasicChannelEstimation(int rows, int tap);

    void MotionCoeff2(complex<double> beta);

    void get_history(double *history, int n, vector<double> & profiles);

    void PaddingZero(Histories &history_type);

    // bool USE_KALMAN = false;


    const double C = 34300.0;
    const double FC = 20000.0;
    const double FS = 48000.0;
    const double T = 0.01;
    const double PHASE_DIFF_THRESHOLD = 1.0 * M_PI;
    const double PHASE_DIFF_2_THRESHOLD = 1.0 * M_PI;
    const double use_diff_flag = true;
    const int tail_tap = 20;
    int N_ZC_UP;
    int N_IRS;

    const double complementary_factor = 0.995;

    int last_tap = 0;

    double prev_phase_in_wrap_;
    double prev_velocity;
    
    Pre_Data_Store Strata_pre_;


    Histories swifttrack_history_;
    Histories TOF_history_;
    Histories Strata_history_;

    MatrixX<complex<double>> prev_irs_signal_;
    MatrixX<complex<double>> prev_irs_signal_diff;
    MatrixX<complex<double>> irs_signal_;
    MatrixX<complex<double>> irs_signal_diff;

    complex<double> prev_motion2;
    complex<double> prev_beta;


    // For move median filter
    mvMedian_data mvTOF;
    mvMedian_data mvVelocity;
    mvMedian_data mvAcc;
    double mvMedian(double x, double buffer[5], double* iter);

    double lowpass_taps_v[22];
    double lowpass_taps_a[22];
    double lowpass_taps_fingerIO[22];
    double lowpass(double x, double taps[22]);

    bool is_moving_;


};



#endif
