#ifndef ACOUSTICTRACKER_POSTPROCESSOR_H
#define ACOUSTICTRACKER_POSTPROCESSOR_H

#include "logger_util.hpp"
#include "matrix_util.hpp"

class Postprocessor {
public:
    Postprocessor();

    ~Postprocessor();

    void PaddingZero();

    double ProcessCIRSignal(const MatrixX<complex<double>> &cir_signal);

    void GetPhaseHistory(double *history, int n);

    void GetVelocityHistory(double *history, int n);

    void GetDistHistory(double *history, int n);

private:
    void CutOffCIRSignal(const MatrixX<complex<double>> &cir_signal);

    void CalcPhase();

    void PhaseTransform();

    const int N_IRS = 150;
    const double C = 34300.0;
    const double FC = 20000.0;
    const double T = 0.01;
    const double PHASE_DIFF_THRESHOLD = 1.0 * M_PI;
    const double PHASE_DIFF_2_THRESHOLD = 1.0 * M_PI;

    double prev_phase_in_wrap_;

    vector<double> phase_history_;
    vector<double> velocity_history_;
    vector<double> dist_history_;

    MatrixX<complex<double>> prev_irs_signal_;
    MatrixX<complex<double>> irs_signal_;
};

#endif
