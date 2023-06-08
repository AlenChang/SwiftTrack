#include "postprocessor.h"
#include <stdio.h>
#include "logger_util.hpp"

Postprocessor::Postprocessor(int id, int N_ZC_UP_) {
    N_ZC_UP = N_ZC_UP_;
    T = N_ZC_UP / FS;
    engine_id = id;

    // CIR cutoff number
    if(N_ZC_UP > 60){
        N_IRS = 60; // corresponding distance is N_IRS * c / Fs / 2; N_IRS = 200 for 60cm
    }else{
        N_IRS = N_ZC_UP;
    }

    // initialize bandpass filter buffer
    for(int ti = 0; ti < 9; ti++){
        xtmp[ti] = 0.0;
        ytmp[ti] = 0.0;
    }

    prev_phase_in_wrap_ = 0.0;
    phase_prev_motion2 = 0.0;
    init_velocity = 0.0;
    prev_motion2 = complex<double>(0, 0);
    prev_beta = complex<double>(0, 0);

    prev_irs_signal_ = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));
    // prev_irs_signal_diff = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));
    irs_signal_ = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));
    irs_signal_diff = MatrixX<complex<double>>::Constant(1, N_IRS, complex<double>(0, 0));
    least_square_weights = MatrixX<double>::Constant(1, N_IRS, double(0.0));

//    time(&start_now);
    start_now = high_resolution_clock::now();
    double max_detection_range = N_IRS * C / FS / 2;
    string prompt = "Maximum detection range: " + to_string(max_detection_range) + " cm";
    LoggerUtil::Log("SwifTrackINFO", prompt.c_str());

    LoggerUtil::Log("SwifTrack", "Postprocessor is constructed.");
}

Postprocessor::~Postprocessor() {
    LoggerUtil::Log("SwifTrack", "Postprocessor is destructed.");
}

//* entry point
double Postprocessor::ProcessCIRSignal(const MatrixX<complex<double>> &cir_signal, bool is_moving) {
    is_moving_ = is_moving;
    //* Preprocessing
    CutOffCIRSignal(cir_signal);
    //* swifttrack implementation
//    swifttrack();

    //* strata and tof implementation
//    TapSelectionTOF();

    BasicChannelEstimation(0, 150);

    //* popout redundant data
//    swifttrack_history_.check_size();
//    TOF_history_.check_size();
    Strata_history_.check_size();

    return 1;
}


//* Preprocessing
void Postprocessor::CutOffCIRSignal(const MatrixX<complex<double>> &cir_signal) {
    //* tap clipping
    for (int i = start_tap; i < N_IRS; i++) {
        irs_signal_(0, i) = cir_signal(0, i);
    }

    //* compute diff signal for tap selection
    for (int i = start_tap; i < N_IRS; i++) {
        irs_signal_diff(0, i) = irs_signal_(0, i) - prev_irs_signal_(0, i);
    }

}


//* SwiftTrack Implementation
void Postprocessor::swifttrack() {
    // define the first motion coefficient
    complex<double> beta;

    // Least square for prev_irs_signal_ and irs_signal
    beta = LeastSquare();
    prev_irs_signal_ = irs_signal_;

    // phase unwrapping
    double phase_diff = unwrap(beta, prev_phase_in_wrap_);

    //* second order motion coefficient
//    MotionCoeff2(beta);

    double phase_unwrapped;
    if(is_moving_){
        //* if is moving, check the acceleration value
        if( abs(swifttrack_history_.acc_phase_history_.back()) < (2.0 / 3.0 * M_PI) ){
            //* phase integration
            phase_unwrapped = swifttrack_history_.phase_history_.back() + phase_diff;
        }else{
            //* if acceleration exceed the maximum, turn to the second motion coefficient
            phase_unwrapped = swifttrack_history_.phase_history_.back() + swifttrack_history_.acc_phase_history_.back();
        }

    }else{
        //* if is not moving, do not need unwrap
        phase_unwrapped = arg(beta);
    }

    //* assume the velocity of the first 20 samples do not exceed the maximum *//
    if(swifttrack_history_.phase_history_.size() < 20){
        phase_unwrapped = arg(beta);
    }

    assert( !isnan(phase_unwrapped) );
    swifttrack_history_.phase_history_.push_back(phase_unwrapped);
//    swifttrack_history_.phase_history_.push_back(arg(beta));

    if(!init_velocity_flag && phase_unwrapped != 0){
        init_velocity = -phase_unwrapped * C / (4 * M_PI * FC * T);;
    }

    //* convert the phase of first order motion coefficient to distance
    Phase2Dist();


}


//* Least square between irs_signal_ and prev_irs_signal_
complex<double> Postprocessor::LeastSquare(){
    complex<double> beta1;
    complex<double> beta2;

    // regression to estimate first motion coefficients
    MatrixX<complex<double>> tmp_signal_ = prev_irs_signal_.conjugate();
    

    beta1 = MatrixUtil::DotSum(tmp_signal_, irs_signal_);
    beta2 = MatrixUtil::DotSum(tmp_signal_, prev_irs_signal_);

    complex<double> beta = beta1 * conj(beta2);
    if(isnan(beta.real())){
        beta = complex<double>(0, 0);
    }

    return beta;
}


double Postprocessor::unwrap(complex<double> tapSel, double & pre_phase) {

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


void Postprocessor::Phase2Dist() {
    double velocity;

    velocity = -swifttrack_history_.phase_history_.back() * C / (4 * M_PI * FC * T);
    if(init_velocity == 0){
        init_velocity = velocity;
    }

    velocity = bandpassfilter_resp(velocity, xtmp, ytmp);
    double dist = swifttrack_history_.dist_history_.back() + velocity * T;
    if(reset_result_flag && !is_moving_ && velocity * swifttrack_history_.velocity_history_.back() <= 0){
        velocity = 0;
        dist = 0;
        reset_result_flag = false;
        swifttrack_history_.clearbuff();
        TOF_history_.clearbuff();
        Strata_history_.clearbuff();


        for(int ti = 0; ti < respfilter_len; ti++){
            xtmp[ti] = 0;
            ytmp[ti] = 0;
        }
    }else{
        if(reset_result_flag){
            return;
        }
    }
    swifttrack_history_.velocity_history_.push_back(velocity);
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
        
//        dist = mvMedian(dist, mvTOF.buffer, &mvTOF.iter);
//        dist = lowpass(dist, lowpass_taps_fingerIO);
        TOF_history_.dist_history_.push_back(dist);
        TOF_history_.velocity_history_.push_back(0);
        TOF_history_.phase_history_.push_back(0);
//        BasicChannelEstimation(i, tap);

        tap = 0;
    }
}

//* Strata implementation
void Postprocessor::BasicChannelEstimation(int rows, int tap){
//    tap = 30;
    tap = 30;
    complex<double> tapSel = irs_signal_(rows, tap);

//    string prompt = "Selected tap, real: " + to_string(tapSel.real()) + " imag: " + to_string(tapSel.imag());
//    LoggerUtil::Log("postprocessing", prompt.c_str());


    double phase_diff = unwrap(tapSel, Strata_pre_.pre_phase_in_wrap_);
    double phase_unwrapped = Strata_history_.phase_history_.back() + phase_diff;
    Strata_history_.phase_history_.push_back(phase_unwrapped);

    // double dist = - phase_unwrapped * C / 2 / M_PI / FC / 2;

    double velocity = - phase_diff * C / 2 / M_PI / FC / 2;

//    double dist = (1 - complementary_factor) * TOF_history_.dist_history_.back() + complementary_factor * (Strata_history_.dist_history_.back() + velocity * T);
    double dist = Strata_history_.dist_history_.back() + velocity * T;
    Strata_history_.dist_history_.push_back(dist);
    Strata_history_.velocity_history_.push_back(phase_diff);

    //    time_t now;
//    time(&now);                 // get current time
    high_resolution_clock::time_point now = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(start_now - now);
//    double seconds = difftime(start_now, now);
    double seconds = duration.count();
    Strata_history_.time_stamp.push_back(seconds);

}

//double Postprocessor::mvMedian(double x, double buffer[5], double* iter){
//    return NoiseSupression::mvMedian(x, buffer, iter);
//}

//double Postprocessor::lowpass(double x, double taps[22]){
//    return NoiseSupression::lowPass6_15_60_100(x, taps);
//}


// *********************************************
// *********************************************
// *********************************************
// * data requisition
// *********************************************
// *********************************************
// *********************************************

Histories & Postprocessor::GetHistories(int history_type){
    switch(history_type){
        case 0:
            return TOF_history_;
        case 1:
            return Strata_history_;
        case 2:
            return swifttrack_history_;
        default:
            return Strata_history_;

    }
}


void Postprocessor::PaddingZero(Histories &history_type){
    history_type.phase_history_.push_back(0.0);
    history_type.velocity_history_.push_back(0.0);
    history_type.dist_history_.push_back(0.0);
    history_type.acc_history_.push_back(0.0);
    history_type.acc2velocity_history_.push_back(0.0);
    history_type.acc2dist_history_.push_back(0.0);
    history_type.acc_phase_history_.push_back(0.0);
    history_type.time_stamp.push_back(0.0);
}

void Postprocessor::get_history(double *history, int n, deque<double> & profiles){
    int l = (int) profiles.size();

    int buffer_counter = 0;
    if (n >= l) {
        auto iter = profiles.begin();
        for (int i = 0; i < l; i++) {
            *(history + i) = *iter;
            iter++;
            buffer_counter++;
        }
    } else {
        auto iter = profiles.end();
        for (int i = 0; i < n; i++) {
            iter--;
            *(history + n - 1 - i) = *iter;
            buffer_counter++;
        }
    }
    for(int i = 0; i < 5; i++){
        *(history+i) = *(history+2);
    }
    for(int i = 0; i < buffer_counter; i++){
        *(history+i) = *(history+i) - *(history);
    }


}

void Postprocessor::GetCIR(double *cir_abs, int n){
//    for (int i = 0; i < N_IRS; i++) {
//        *(cir_abs + i) = abs(irs_signal_(0, i));
//    }
//    if(n > 2 * N_IRS){
//        for (int i = 0; i < N_IRS; i++) {
//            *(cir_abs + i + N_IRS) = arg(irs_signal_(0, i));
//        }
//    }

    for (int i = 0; i < N_IRS; i++) {
//        *(cir_abs + i) = abs(irs_signal_diff(0, i));
        *(cir_abs + i) = abs(irs_signal_diff(0, i));
    }
//    if(n > 2 * N_IRS){
//        for (int i = 0; i < N_IRS; i++) {
//            *(cir_abs + i + N_IRS) = arg(irs_signal_(0, i));
//        }
//    }

}

void Postprocessor::GetBeta(double* beta_real, double* beta_imag){
    *beta_real = prev_beta.real();
    *beta_imag = prev_beta.imag();
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
        case time_stamp:
            get_history(history, n, history_type.time_stamp);
        default:
            break;
    }
}

int Postprocessor::GetHistoryLength(Histories &history_type){
    return history_type.getLength();
}

double Postprocessor::bandpassfilter_resp(double x, double xtmp[respfilter_len], double ytmp[respfilter_len]) {
    // bandpass filter
//    double b[9] = {0.00010958156812944314171653220624236,0,-0.00043832627251777256686612882496945,0,0.00065748940877665882319413892531657,0,-0.00043832627251777256686612882496945,0,0.00010958156812944314171653220624236};;
//    double a[9] = {1.0,-7.4095732886727185828590336313937,24.051598062643432029972245800309,-44.674600809450318195104046026245,51.937580026394755350338527932763,-38.701058484781242441385984420776,18.05077527858343700017940136604,-4.8182664583192069684969283116516,0.5635456740956032994915858580498};

    // high pass filter
//    double b[6] = {0.96831969148056540319657869986258,-4.8415984574028270159828934993129,9.6831969148056540319657869986258,-9.6831969148056540319657869986258,4.8415984574028270159828934993129,-0.96831969148056540319657869986258};
//    double a[6] = {1.0,-4.9356156496366665464847756084055,9.7445304036221589427668732241727,-9.6198563876336820044343767222017,4.7485846615765634481931556365453,-0.93764302490901729747463377862005};

    // band pass filter2
    double b[respfilter_len] = {
            -0.007364071461727886638848339373453200096264481544
            ,0.002541500194949617135592445649194814905058592558
            ,0.004953507163435371361825954039659336558543145657
            ,0.008947394881960720608438109024973527994006872177
            ,0.014426922936018028720273065346191287972033023834
            ,0.021249707616999933407209155689088220242410898209
            ,0.029184229724439569864102850260678678750991821289
            ,0.037897175197590275697034911672744783572852611542
            ,0.046962973988042895634453799402763252146542072296
            ,0.055888113179456168355674350323170074261724948883
            ,0.064145765296197795879429293108842102810740470886
            ,0.071218026036823686708565617209387710317969322205
            ,0.076641780452006813550980268701096065342426300049
            ,0.080051895399725958579750795252039097249507904053
            ,0.081215276588081253272832782386103644967079162598
            ,0.080051895399725958579750795252039097249507904053
            ,0.076641780452006813550980268701096065342426300049
            ,0.071218026036823686708565617209387710317969322205
            ,0.064145765296197795879429293108842102810740470886
            ,0.055888113179456168355674350323170074261724948883
            ,0.046962973988042895634453799402763252146542072296
            ,0.037897175197590275697034911672744783572852611542
            ,0.029184229724439569864102850260678678750991821289
            ,0.021249707616999933407209155689088220242410898209
            ,0.014426922936018028720273065346191287972033023834
            ,0.008947394881960720608438109024973527994006872177
            ,0.004953507163435371361825954039659336558543145657
            ,0.002541500194949617135592445649194814905058592558
            ,-0.007364071461727886638848339373453200096264481544
    };

    double y = 0.0;

    xtmp[0] = x;
    for(int ti = 0; ti < respfilter_len; ti++){
        y += b[ti] * xtmp[ti];
    }
    for(int ti = respfilter_len-1; ti > 0; ti--){
        xtmp[ti] = xtmp[ti-1];
    }

    if(filter_delay_counter < respfilter_len){
        y = 0;
        filter_delay_counter++;
    }
    return y;

}

void Postprocessor::reset_results(void){
    reset_result_flag = true;
}
