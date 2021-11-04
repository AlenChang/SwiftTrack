#ifndef ACOUSTICTRACKER_POSTPROCESSOR_H
#define ACOUSTICTRACKER_POSTPROCESSOR_H

// #include "logger_util.hpp"
#include "matrix_util.hpp"
#include "histories.h"

class Postprocessor {
  
public:
    Postprocessor();

    ~Postprocessor();

    double ProcessCIRSignal(const MatrixX<complex<double>> &cir_signal);

    double ProcessCIRSignal2(const MatrixX<complex<double>> &cir_signal);

    // operating histories
    Histories & GetHistories(string history_name);

    void PaddingZero();

    void PaddingZero(Histories &history_type);

    void GetPhaseHistory(double *history, int n);

    void GetPhaseHistory(double *history, int n, Histories &history_type);

    void GetVelocityHistory(double *history, int n);

    void GetVelocityHistory(double *history, int n, Histories &history_type);

    void GetDistHistory(double *history, int n);

    void GetDistHistory(double *history, int n, Histories &history_type);

private:
    void CutOffCIRSignal(const MatrixX<complex<double>> &cir_signal);

    void CalcPhase();

    double CalcPhase(complex<double> tapSel, double & pre_phase);

    complex<double> LeastSquare();

    void PhaseTransform();

    void TapSelectionTOF();

    void BasicChannelEstimation(int rows, int tap);

    void CallPhaseStrata();

    // bool USE_KALMAN = false;

    const int N_IRS = 150;
    const double C = 34300.0;
    const double FC = 20000.0;
    const double FS = 48000.0;
    const double T = 0.01;
    const double PHASE_DIFF_THRESHOLD = 1.0 * M_PI;
    const double PHASE_DIFF_2_THRESHOLD = 1.0 * M_PI;

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
};

#endif
