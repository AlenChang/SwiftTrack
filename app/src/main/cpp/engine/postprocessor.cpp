#include "postprocessor.h"

Postprocessor::Postprocessor() {
    prev_phase_in_wrap_ = 0.0;
    prev_motion2 = complex<double>(0, 0);
    mvMedian_iter = 0;

    for (int i = 0; i < 5; i++) {
        mvMedian_buffer[i] = 0.0;
    }
    

    phase_history_.push_back(0.0);
    velocity_history_.push_back(0.0);
    dist_history_.push_back(0.0);

    prev_irs_signal_ = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));
    irs_signal_ = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));
    irs_signal_diff = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));

    cout << "Postprocessor was initiated." << endl;
}

Postprocessor::~Postprocessor() {
    cout << "Postprocessor was recycled." << endl;
}

Histories & Postprocessor::GetHistories(int history_type){
    switch(history_type){
        case 0:
            return TOF_history_;
        case 1:
            return Strata_history_;
        case 2:
            return swifttrack_history_;
        default:
            return swifttrack_history_;
    }
}

void Postprocessor::PaddingZero() {
    phase_history_.push_back(0.0);
    velocity_history_.push_back(0.0);
    dist_history_.push_back(0.0);

    PaddingZero(TOF_history_);
    PaddingZero(Strata_history_);
    PaddingZero(swifttrack_history_);

}

void Postprocessor::PaddingZero(Histories &history_type){
    history_type.phase_history_.push_back(0.0);
    history_type.velocity_history_.push_back(0.0);
    history_type.dist_history_.push_back(0.0);
    history_type.acc_history_.push_back(0.0);
    history_type.acc2velocity_history_.push_back(0.0);
    history_type.acc2dist_history_.push_back(0.0);
    history_type.acc_phase_history_.push_back(0.0);
}

double Postprocessor::ProcessCIRSignal(const MatrixX<complex<double>> &cir_signal, bool is_moving) {
    CutOffCIRSignal(cir_signal);

    CalcPhase();

    TapSelectionTOF(is_moving);

    PhaseTransform();

    return dist_history_.back();
}



void Postprocessor::GetPhaseHistory(double *history, int n) {
    get_history(history, n, phase_history_);
}

void Postprocessor::GetPhaseHistory(double *history, int n, Histories &history_type) {
    get_history(history, n, history_type.phase_history_);
}

void Postprocessor::GetVelocityHistory(double *history, int n) {
    get_history(history, n, velocity_history_);
}

void Postprocessor::GetVelocityHistory(double *history, int n, Histories &history_type) {
    get_history(history, n, history_type.velocity_history_);
}

void Postprocessor::GetDistHistory(double *history, int n) {
    get_history(history, n, dist_history_);
    
}

void Postprocessor::GetDistHistory(double *history, int n, Histories &history_type) {
    get_history(history, n, history_type.dist_history_);
    
}

void Postprocessor::GetHistoryData(double *history, int n, Histories &history_type, HistoryType h_type){
    switch (h_type) {
        case phase_v:
            get_history(history, n, history_type.phase_history_);
            break;
        case velocity_:
            get_history(history, n, history_type.velocity_history_);
            break;
        case dist_v:
            get_history(history, n, history_type.dist_history_);
            break;
        case acceleration_:
            get_history(history, n, history_type.acc_history_);
            break;
        case velocity_a:
            get_history(history, n, history_type.acc2velocity_history_);
            break;
        case dist_a:
            get_history(history, n, history_type.acc2dist_history_);
            break;
        case phase_acc:
            get_history(history, n, history_type.acc_phase_history_);
            break;
        default:
            break;
    }
}

void Postprocessor::get_history(double *history, int n, vector<double> & profiles){
    int l = (int) profiles.size();

    if (n >= l) {
        for (int i = 0; i < l; i++) {
            *(history + i) = profiles[i];
        }
    } else {
        for (int i = 0; i < n; i++) {
            *(history + n - 1 - i) = profiles[l - 1 - i];
        }
    }
}

void Postprocessor::GetCIR(double *cir_abs, int n){
    if (use_diff_flag){
        for (int i = 0; i < N_IRS; i++) {
            *(cir_abs + i) = abs(irs_signal_diff(0, i));
        }
    } else{
        for (int i = 0; i < N_IRS; i++) {
            *(cir_abs + i) = abs(irs_signal_(0, i));
        }
    }

}

// do not need a special version
void Postprocessor::CutOffCIRSignal(const MatrixX<complex<double>> &cir_signal) {
    for (int i = 0; i < N_IRS; i++) {
        irs_signal_(0, i) = cir_signal(0, i);
    }
}

complex<double> Postprocessor::LeastSquare(){
    // regression to estimate first motion coefficients
    complex<double> beta = MatrixUtil::DotSum(prev_irs_signal_.conjugate(), irs_signal_);
    for (int i = 0; i < N_IRS; i++) {
        irs_signal_diff(0, i) = irs_signal_(0, i) - prev_irs_signal_(0, i);
    }
    
    prev_irs_signal_ = irs_signal_;
    return beta;
}

void Postprocessor::CalcPhase() {
    
    complex<double> beta = LeastSquare();
    MotionCoeff2(beta);

    double phase_diff = CalcPhase(beta, prev_phase_in_wrap_);
    double phase_unwrapped = phase_history_.back() + phase_diff;
    phase_history_.push_back(phase_unwrapped);

    // todo: to implement with kalman filter
    swifttrack_history_.phase_history_.push_back(phase_unwrapped);
}

void Postprocessor::MotionCoeff2(complex<double> beta){
    double phase_prev_motion2 = arg(prev_motion2);
    complex<double> motion2 = beta / prev_motion2;
    prev_motion2 = beta;

    double phase_diff = CalcPhase(motion2, phase_prev_motion2);
    double phase_unwrapped = swifttrack_history_.acc_phase_history_.back() + phase_diff;
    double acc = phase_unwrapped * C / (4 * M_PI * FC * T);
    double velocity = swifttrack_history_.acc2velocity_history_.back() + acc * T;
    double dist = swifttrack_history_.acc2dist_history_.back() + velocity * T;

    swifttrack_history_.acc_phase_history_.push_back(phase_unwrapped);
    swifttrack_history_.acc_history_.push_back(acc);
    swifttrack_history_.acc2velocity_history_.push_back(velocity);
    swifttrack_history_.acc2dist_history_.push_back(dist);
    
}


double Postprocessor::CalcPhase(complex<double> tapSel, double & pre_phase) {

    double phase_in_wrap = arg(tapSel);

    // phase unwrap
    double phase_diff = phase_in_wrap - pre_phase;
    pre_phase = phase_in_wrap;

    // constraint phase diff by phase' threshold
    if (phase_diff > PHASE_DIFF_THRESHOLD) {
        phase_diff -= 2 * M_PI;
    } else if (phase_diff < -PHASE_DIFF_THRESHOLD) {
        phase_diff += 2 * M_PI;
    }

    return phase_diff;
}


void Postprocessor::PhaseTransform() {
    double velocity = -phase_history_.back() * C / (4 * M_PI * FC * T);
    velocity_history_.push_back(velocity);

    swifttrack_history_.velocity_history_.push_back(velocity);

    double dist = swifttrack_history_.dist_history_.back() + velocity * T;
    dist_history_.push_back(dist);

    swifttrack_history_.dist_history_.push_back(dist);
}


void Postprocessor::TapSelectionTOF(bool is_moving){
    MatrixX<complex<double>> signal;
    if(use_diff_flag){
        signal = irs_signal_diff;
    } else {
        signal = irs_signal_;
    }
    int rows = (int) signal.rows(), cols = (int) signal.cols();

    double res = 0.0;
    int tap = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double amp = abs(signal(i, j));
            if (amp > res) {
                res = amp;
                tap = j;
            }
        }
        // choose taps according to moving status
        if(is_moving){
            last_tap = tap;
        } else{
            tap = last_tap;
        }

        // we only care about distance history for TOF method
        double dist;
        if(tap > 240){
            dist = ((double) tap - 480) / FC * C / 2;
        } else{
            dist = (double) tap / FC * C / 2;
        }
        
        dist = mvMedian(dist);
        TOF_history_.dist_history_.push_back(dist);
        TOF_history_.velocity_history_.push_back(0);
        TOF_history_.phase_history_.push_back(0);
        BasicChannelEstimation(i, tap);

        tap = 0;
    }
}

void Postprocessor::BasicChannelEstimation(int rows, int tap){
    complex<double> tapSel = irs_signal_(rows, tap);
    double phase_diff = CalcPhase(tapSel, Strata_pre_.pre_phase_in_wrap_);
    double phase_unwrapped = Strata_history_.phase_history_.back() + phase_diff;
    Strata_history_.phase_history_.push_back(phase_unwrapped);
    Strata_history_.dist_history_.push_back(- phase_unwrapped * C / 2 / M_PI / FC / 2);
    Strata_history_.velocity_history_.push_back(0);

}

double Postprocessor::mvMedian(double x){
    return classInstance->mvMedian(x, mvMedian_buffer, &mvMedian_iter);
}