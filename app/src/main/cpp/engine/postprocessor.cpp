#include "postprocessor.h"
#include <stdio.h>

Postprocessor::Postprocessor(int N_ZC_UP_) {
    N_ZC_UP = N_ZC_UP_;
    if(N_ZC_UP > 150){
        N_IRS = 150;
    }else{
        N_IRS = N_ZC_UP;
    }

    prev_phase_in_wrap_ = 0.0;
    prev_motion2 = complex<double>(0, 0);
    prev_beta = complex<double>(0, 0);

    prev_irs_signal_ = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));
    prev_irs_signal_diff = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));
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
    is_moving_ = is_moving;
    // choose the first N_IRS taps
    CutOffCIRSignal(cir_signal);
    
    // calculate motion coefficients
    CalcPhase();

    TapSelectionTOF();

    PhaseTransform();

    return swifttrack_history_.dist_history_.back();
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
    for (int i = 0; i < N_IRS; i++) {
        *(cir_abs + i) = abs(irs_signal_(0, i));
    }

}

void Postprocessor::GetBeta(double* beta_real, double* beta_imag){
    *beta_real = prev_beta.real();
    *beta_imag = prev_beta.imag();
}

// do not need a special version
void Postprocessor::CutOffCIRSignal(const MatrixX<complex<double>> &cir_signal) {
    for (int i = - tail_tap; i < N_IRS - tail_tap; i++) {
        irs_signal_(0, i + tail_tap) = cir_signal(0, (N_ZC_UP + i) % N_ZC_UP);
    }
}

complex<double> Postprocessor::LeastSquare(){
    complex<double> beta1;
    complex<double> beta2;

    // regression to estimate first motion coefficients
    beta1 = MatrixUtil::DotSum(prev_irs_signal_.conjugate(), irs_signal_);
    beta2 = MatrixUtil::DotSum(prev_irs_signal_.conjugate(), prev_irs_signal_);


    complex<double> beta = beta1 / beta2;
    if(isnan(beta.real())){
        beta = complex<double>(0, 0);
    }

    return beta;
}

void Postprocessor::CalcPhase() {
    complex<double> beta;
    
    
    for (int i = 0; i < N_IRS; i++) {
        irs_signal_diff(0, i) = irs_signal_(0, i) - prev_irs_signal_(0, i);
    }

    beta = LeastSquare();

    prev_irs_signal_ = irs_signal_;
    prev_irs_signal_diff = irs_signal_diff;

    // second order motion coefficients
    MotionCoeff2(beta);

    double phase_diff = CalcPhase(beta, prev_phase_in_wrap_);

    double phase_unwrapped;
    if(is_moving_){
        phase_unwrapped = - prev_velocity / C * (4 * M_PI * FC * T) + phase_diff;
    }else{
        phase_unwrapped = arg(beta);
    }


    assert( !isnan(phase_unwrapped) );

    // todo: to implement with kalman filter
    swifttrack_history_.phase_history_.push_back(phase_unwrapped);
}

// for motion2, the phase do not produce ambiguity
void Postprocessor::MotionCoeff2(complex<double> beta){
    double phase_diff, phase_unwrapped, acc, velocity, dist;
    if(abs(prev_beta) != 0 & abs(beta) != 0){
//        double phase_prev_motion2 = arg(prev_motion2);
        complex<double> motion2 = beta / prev_beta;
//        prev_motion2 = motion2;
//
//        phase_diff = CalcPhase(motion2, phase_prev_motion2);
//        phase_unwrapped = swifttrack_history_.acc_phase_history_.back() + phase_diff;
        acc = -arg(motion2);
        // if(abs(acc) > 2){
        //     acc = 0;
        // }
        acc = acc * C / (4 * M_PI * FC * T * T);

        acc = mvMedian(acc, mvAcc.buffer, &mvAcc.iter);
        // acc = lowpass(acc, lowpass_taps_a);

        velocity = swifttrack_history_.acc2velocity_history_.back() + acc * T;
        dist = swifttrack_history_.acc2dist_history_.back() + velocity * T;
    }else{
        
        phase_unwrapped = 0;
        acc = 0;
        velocity = 0;
        dist = 0;
    }

    prev_beta = beta;
    

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
    double velocity;

    // if(abs(swifttrack_history_.acc_history_.back()) > 2000 && is_moving_){
    //     velocity = swifttrack_history_.velocity_history_.back() +
    //         swifttrack_history_.acc_history_.back() * 0.01;
    // } else {
    //     velocity = -swifttrack_history_.phase_history_.back() * C / (4 * M_PI * FC * T);
    // }

    velocity = -swifttrack_history_.phase_history_.back() * C / (4 * M_PI * FC * T);

    // if(is_moving_){
    //     if(velocity - swifttrack_history_.velocity_history_.back() > 35 ){
    //         velocity = velocity - 2 * M_PI * C / (4 * M_PI * FC * T);
    //     }else{
    //         if(velocity - swifttrack_history_.velocity_history_.back() < -35){
    //             velocity = velocity + 2 * M_PI * C / (4 * M_PI * FC * T);
    //         }
    //     }
    // }

    
    prev_velocity = velocity;


    // velocity = mvMedian(velocity, mvVelocity.buffer, &mvVelocity.iter);
    // velocity = lowpass(velocity, lowpass_taps_v);

    swifttrack_history_.velocity_history_.push_back(velocity);

//    double dist = (1 - complementary_factor) * TOF_history_.dist_history_.back() + complementary_factor * (swifttrack_history_.dist_history_.back() + velocity * T);
    double dist = swifttrack_history_.dist_history_.back() + velocity * T;
    swifttrack_history_.dist_history_.push_back(dist);
}


// FingerIO implementation
void Postprocessor::TapSelectionTOF(){
    MatrixX<complex<double>> signal;
    // determine is use DDBR to remove background noise
    if(use_diff_flag){
        signal = irs_signal_diff;
    } else {
        signal = irs_signal_;
    }
    int rows = (int) signal.rows(), cols = (int) signal.cols();

    double res = 0.0;
    int tap = 0;

    // for each row
    for (int i = 0; i < rows; i++) {

        // for each column, find the maximum
        for (int j = 0; j < cols; j++) {
            double amp = abs(signal(i, j));
            if (amp > res) {
                res = amp;
                tap = j;
            }
        }

        // choose taps according to moving status
        // if we use DDBR
        if(use_diff_flag){
            if(is_moving_){
                last_tap = tap;
            } else{
                tap = last_tap;
            }
        }
        

        // we only care about distance history for TOF method
        double dist;
        if(tap > N_ZC_UP / 2){
            dist = ((double) tap - N_ZC_UP) / FC * C / 2;
        } else{
            dist = (double) tap / FC * C / 2;
        }
        
        dist = mvMedian(dist, mvTOF.buffer, &mvTOF.iter);
        dist = lowpass(dist, lowpass_taps_fingerIO);
        TOF_history_.dist_history_.push_back(dist);
        TOF_history_.velocity_history_.push_back(0);
        TOF_history_.phase_history_.push_back(0);
        BasicChannelEstimation(i, tap);

        tap = 0;
    }
}

// Strata implementation
void Postprocessor::BasicChannelEstimation(int rows, int tap){
    complex<double> tapSel = irs_signal_(rows, tap);
    double phase_diff = CalcPhase(tapSel, Strata_pre_.pre_phase_in_wrap_);
    double phase_unwrapped = Strata_history_.phase_history_.back() + phase_diff;
    Strata_history_.phase_history_.push_back(phase_unwrapped);

    // double dist = - phase_unwrapped * C / 2 / M_PI / FC / 2;

    double velocity = - phase_diff * C / 2 / M_PI / FC / 2;

//    double dist = (1 - complementary_factor) * TOF_history_.dist_history_.back() + complementary_factor * (Strata_history_.dist_history_.back() + velocity * T);
    double dist = Strata_history_.dist_history_.back() + velocity * T;
    Strata_history_.dist_history_.push_back(dist);
    Strata_history_.velocity_history_.push_back(0);

}

double Postprocessor::mvMedian(double x, double buffer[5], double* iter){
    return NoiseSupression::mvMedian(x, buffer, iter);
}

double Postprocessor::lowpass(double x, double taps[22]){
    return NoiseSupression::lowPass6_15_60_100(x, taps);
}