#include "postprocessor.h"

Postprocessor::Postprocessor() {
    prev_phase_in_wrap_ = 0.0;

    // // todo: use union to wrap these 3 variables
    // Histories swifttrack_history_;
    // init_history(swifttrack_history_);

    phase_history_.push_back(0.0);
    velocity_history_.push_back(0.0);
    dist_history_.push_back(0.0);

    prev_irs_signal_ = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));
    irs_signal_ = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));

    cout << "Postprocessor was initiated." << endl;
}

Postprocessor::Postprocessor(bool useKalman) {
    // to differentiate from original constructor
    USE_KALMAN = useKalman;
    prev_phase_in_wrap_ = 0.0;

    init_history(swifttrack_history_);
    init_history(TOF_history_);
    init_history(Strata_history_);


    prev_irs_signal_ = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));
    irs_signal_ = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));

    cout << "Postprocessor was initiated." << endl;
}

Postprocessor::~Postprocessor() {
    cout << "Postprocessor was recycled." << endl;
}

Histories & Postprocessor::GetHistories(string history_name){
    if (history_name == "TOF"){
        return TOF_history_;
    }else if(history_name == "STRATA"){
        return Strata_history_;
    } else {
        return swifttrack_history_;
    }
}

void Postprocessor::PaddingZero() {
    phase_history_.push_back(0.0);
    velocity_history_.push_back(0.0);
    dist_history_.push_back(0.0);
}

void Postprocessor::PaddingZero(Histories &history_type){
    history_type.phase_history_.push_back(0.0);
    history_type.velocity_history_.push_back(0.0);
    history_type.dist_history_.push_back(0.0);
}

double Postprocessor::ProcessCIRSignal(const MatrixX<complex<double>> &cir_signal) {
    CutOffCIRSignal(cir_signal);

    TapSelectionTOF();

    CalcPhase();

    PhaseTransform();

    return dist_history_.back();
}

double Postprocessor::ProcessCIRSignal2(const MatrixX<complex<double>> &cir_signal) {
    CutOffCIRSignal(cir_signal);

    // select channel taps with maximum amplitude

    CalcPhase();

    PhaseTransform();

    return dist_history_.back();
}


void Postprocessor::GetPhaseHistory(double *history, int n) {
    int l = (int) phase_history_.size();

    if (n >= l) {
        for (int i = 0; i < l; i++) {
            *(history + i) = phase_history_[i];
        }
    } else {
        for (int i = 0; i < n; i++) {
            *(history + n - 1 - i) = phase_history_[l - 1 - i];
        }
    }
}

void Postprocessor::GetPhaseHistory(double *history, int n, Histories &history_type) {
    int l = (int) history_type.phase_history_.size();

    if (n >= l) {
        for (int i = 0; i < l; i++) {
            *(history + i) = history_type.phase_history_[i];
        }
    } else {
        for (int i = 0; i < n; i++) {
            *(history + n - 1 - i) = history_type.phase_history_[l - 1 - i];
        }
    }
}

void Postprocessor::GetVelocityHistory(double *history, int n) {
    int l = (int) velocity_history_.size();

    if (n >= l) {
        for (int i = 0; i < l; i++) {
            *(history + i) = velocity_history_[i];
        }
    } else {
        for (int i = 0; i < n; i++) {
            *(history + n - 1 - i) = velocity_history_[l - 1 - i];
        }
    }
}

void Postprocessor::GetVelocityHistory(double *history, int n, Histories &history_type) {
    int l = (int) history_type.velocity_history_.size();

    if (n >= l) {
        for (int i = 0; i < l; i++) {
            *(history + i) = history_type.velocity_history_[i];
        }
    } else {
        for (int i = 0; i < n; i++) {
            *(history + n - 1 - i) = history_type.velocity_history_[l - 1 - i];
        }
    }
}

void Postprocessor::GetDistHistory(double *history, int n) {
    int l = (int) dist_history_.size();

    if (n >= l) {
        for (int i = 0; i < l; i++) {
            *(history + i) = dist_history_[i];
        }
    } else {
        for (int i = 0; i < n; i++) {
            *(history + n - 1 - i) = dist_history_[l - 1 - i];
        }
    }
}

void Postprocessor::GetDistHistory(double *history, int n, Histories &history_type) {
    int l = (int) history_type.dist_history_.size();

    if (n >= l) {
        for (int i = 0; i < l; i++) {
            *(history + i) = history_type.dist_history_[i];
        }
    } else {
        for (int i = 0; i < n; i++) {
            *(history + n - 1 - i) = history_type.dist_history_[l - 1 - i];
        }
    }
}

// do not need a special version
void Postprocessor::CutOffCIRSignal(const MatrixX<complex<double>> &cir_signal) {
    for (int i = 0; i < N_IRS; i++) {
        irs_signal_(0, i) = cir_signal(0, i);
    }
}

void Postprocessor::CalcPhase() {
    // regression to phase
    complex<double> beta = MatrixUtil::DotSum(prev_irs_signal_.conjugate(), irs_signal_);
    prev_irs_signal_ = irs_signal_;
    double phase_in_wrap = arg(beta);

    // phase unwrap
    double phase_diff = phase_in_wrap - prev_phase_in_wrap_;
    prev_phase_in_wrap_ = phase_in_wrap;

    // constraint phase diff by phase' threshold
    if (phase_diff > PHASE_DIFF_THRESHOLD) {
        phase_diff -= 2 * M_PI;
    } else if (phase_diff < -PHASE_DIFF_THRESHOLD) {
        phase_diff += 2 * M_PI;
    }

    // constraint phase diff by phase'' threshold
//    int n = (int) phase_history_.size();
//    double prev_phase_diff = (n > 1) ? phase_history_[n - 1] - phase_history_[n - 2] : 0.0;
//    if (phase_diff - prev_phase_diff > PHASE_DIFF_2_THRESHOLD) {
//        phase_diff -= 2 * M_PI;
//    } else if (phase_diff - prev_phase_diff < -PHASE_DIFF_2_THRESHOLD) {
//        phase_diff += 2 * M_PI;
//    }

    double phase_unwrapped = phase_history_.back() + phase_diff;

    phase_history_.push_back(phase_unwrapped);
}


void Postprocessor::PhaseTransform() {
    double velocity = -phase_history_.back() * C / (4 * M_PI * FC * T);
    velocity_history_.push_back(velocity);

    double dist = dist_history_.back() + velocity * T;
    dist_history_.push_back(dist);
}

void Postprocessor::PhaseTransform(Histories &history_type){
    double velocity = -history_type.phase_history_.back() * C / (4 * M_PI * FC * T);
    history_type.velocity_history_.push_back(velocity);

    double dist = history_type.dist_history_.back() + velocity * T;
    history_type.dist_history_.push_back(dist);
}

void Postprocessor::init_history(Histories &history_type) {
    history_type.phase_history_.push_back(0.0);
    history_type.velocity_history_.push_back(0.0);
    history_type.dist_history_.push_back(0.0);

}

void Postprocessor::TapSelectionTOF(){
    int rows = (int) irs_signal_.rows(), cols = (int) irs_signal_.cols();

    double res = 0.0;
    int tap = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double amp = abs(irs_signal_(i, j));
            if (amp > res) {
                res = amp;
                tap = j;
            }
        }
        // we only care about distance history for TOF method
        double dist = (double) (tap+1) / FC * C / 2;
        TOF_history_.dist_history_.push_back(dist);
        TOF_history_.velocity_history_.push_back(0);
        TOF_history_.phase_history_.push_back(0);
        tap = 0;
    }
    
    
    
}