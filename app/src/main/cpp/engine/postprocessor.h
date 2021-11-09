#ifndef ACOUSTICTRACKER_POSTPROCESSOR_H
#define ACOUSTICTRACKER_POSTPROCESSOR_H

// #include "logger_util.hpp"
#include "matrix_util.hpp"
#include "histories.h"
#include "codeGen.h"
#include "denoiser.h"
#include <stdio.h>


class Postprocessor {
  
public:
    Postprocessor();

    ~Postprocessor();

    double ProcessCIRSignal(const MatrixX<complex<double>> &cir_signal, bool is_moving);

    // operating histories
    Histories & GetHistories(int history_type);

    void PaddingZero();

    void PaddingZero(Histories &history_type);

    void GetPhaseHistory(double *history, int n);

    void GetVelocityHistory(double *history, int n);

    void GetDistHistory(double *history, int n);

    void GetCIR(double *cir_abs, int n);

    void GetHistoryData(double *history, int n, Histories &history_type, HistoryType h_type);

private:
    void CutOffCIRSignal(const MatrixX<complex<double>> &cir_signal);

    void CalcPhase();

    double CalcPhase(complex<double> tapSel, double & pre_phase);

    complex<double> LeastSquare();

    void PhaseTransform();

    void TapSelectionTOF(bool is_moving);

    void BasicChannelEstimation(int rows, int tap);

    void CallPhaseStrata();

    void MotionCoeff2(complex<double> beta);

    void get_history(double *history, int n, vector<double> & profiles);

    // bool USE_KALMAN = false;

    const int N_IRS = 480;
    const double C = 34300.0;
    const double FC = 20000.0;
    const double FS = 48000.0;
    const double T = 0.01;
    const double PHASE_DIFF_THRESHOLD = 1.0 * M_PI;
    const double PHASE_DIFF_2_THRESHOLD = 1.0 * M_PI;
    const double use_diff_flag = true;

    int last_tap = 0;

    double prev_phase_in_wrap_;
    
    Pre_Data_Store Strata_pre_;


    Histories swifttrack_history_;
    Histories TOF_history_;
    Histories Strata_history_;


    vector<double> phase_history_;
    vector<double> velocity_history_;
    vector<double> dist_history_;


    MatrixX<complex<double>> prev_irs_signal_;
    MatrixX<complex<double>> irs_signal_;
    MatrixX<complex<double>> irs_signal_diff;

    complex<double> prev_motion2;
    complex<double> prev_beta;

    codeGen *classInstance = new codeGen;
    double mvMedian_iter;
    double mvMedian_buffer[5];
    double mvMedian(double x, double buffer[5], double* iter);

    double mvMedian_iter_v_;
    double mvMedian_buffer_v_[5];
    double last_v_ = 0;

    double mvMedian_iter_a_;
    double mvMedian_buffer_a_[5];

};

#endif
