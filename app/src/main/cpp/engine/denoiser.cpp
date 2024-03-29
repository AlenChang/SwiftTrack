#include "include/denoiser.h"
#include "logger_util.hpp"

Denoiser::Denoiser(int id, int N_ZC_UP_) {
    // first stage calibration -> estimate threshold to determine moving period.
    status_ = CALI_1;
    engine_id = id;

    N_ZC_UP = N_ZC_UP_;
    prev_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
    signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
    signa_diff = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
    online_denoise_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));

    prev_is_moving_ = false;
    is_moving_ = false;

//    genparams(N_ZC_UP, 60, &params);

    calibration_1_frame_count_ = 0;
    moving_threshold_ = 0.0;

    // median filter initialization
    mvMedian_iter = 0;
    for (int i = 0; i < 5; i++) {
        mvMedian_buffer[i] = 0.0;
    }

    calibration_2_moving_periods_ = 0;

    moving_frames_ = 0;
    total_moving_frames_ = 0;
    updated_static_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
    static_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));


    LoggerUtil::Log("SwifTrack", "Denoiser is constructed.");
}

Denoiser::~Denoiser() {
    LoggerUtil::Log("SwifTrack", "Denoiser is destructed.");
}

void Denoiser::GetMovingStatus(bool * status){
    *status = is_moving_;
}

void Denoiser::FeedSignal(const MatrixX<complex<double>> &signal) {
    assert(status_ != CALI_FAILED);

    signal_ = signal;

    if (status_ == CALI_1) {
        // compute moving threshold
        // ProcessCalibration1();
        compute_thre();


    } else if (status_ == CALI_2) {
        // compute background noise
        ProcessCalibration2();
    } else if (status_ == CALI_SUCCESS) {
        // remove background noise and update background noise
        ProcessCalibrationSuccess();
    }

    prev_signal_ = signal_;
}

Denoiser::status_type Denoiser::GetStatus() {
    return status_;
}

vector<MatrixX<complex<double>>> Denoiser::GetOfflineDenoiseSignals() {
    return offline_denoise_signals_;
}

MatrixX<complex<double>> Denoiser::GetOnlineDenoiseSignal() {
    return online_denoise_signal_;
}

MatrixX<complex<double>> Denoiser::GetDiffSignal(){
    return signa_diff;
}

vector<double> Denoiser::GetDiffHistory() {
    vector<double> history_out;
    for(auto item : diff_history_){
        history_out.push_back(item);
    }
    return history_out;
}

void Denoiser::compute_thre(){
    calibration_1_frame_count_++;
    if (calibration_1_frame_count_ == 1) {
        return;
    }

    // find maximum taps in each frame
    signa_diff = signal_ - prev_signal_;
//    compute_thre_taps[compute_thre_iter] = 0;
    calibration_1_singal_history_.push_back(signal_);
//    compute_thre_taps[compute_thre_iter] = abs(signa_diff(selected_tap_for_thresholding));
    compute_thre_taps[compute_thre_iter] = MaxDiff(signal_, prev_signal_);
    compute_thre_iter++;
    cout << "compute_thre_iter = " << compute_thre_iter << endl;

    if(compute_thre_iter >= CALI_1_FRAMES){
        // compute mean values
        double sum_value = 0;
        for(int ti = 0; ti < CALI_1_FRAMES; ti++){
            sum_value += compute_thre_taps[ti];
        }
        mean_compute_thre = sum_value / compute_thre_iter;

//        // compute standard deviation
//        double sum_square_diff = 0;
//        for(int ti = 0; ti < CALI_1_FRAMES; ti++){
//            sum_square_diff += pow(compute_thre_taps[ti] - mean_compute_thre, 2);
//        }
//        double std_value = sqrt(sum_square_diff / (compute_thre_iter - 1));

        OfflineCalcStaticSignal();
        OfflineRemoveStaticSignal();

//        for(int ti = 0; ti < CALI_1_FRAMES; ti++){
//
//        }

        // compute threshold
//        moving_threshold_ = mean_value + std_factor * std_value;
        moving_threshold_ = mean_compute_thre;
        max_diff_histry = moving_threshold_;
//        moving_threshold_ = 0;

        init1_flag = true;

//        Log("Threshold", char *s)
    }else{
        init1_flag = false;
        moving_threshold_ = 0;
    }


    if(init1_flag){
//        status_ = CALI_2;
        status_ = CALI_SUCCESS;
        cout << "Threshold is " << moving_threshold_ << endl;
    }
}


// * pass
void Denoiser::ProcessCalibration2() {
    CheckMoving();

    // store moving flags
    // store cir from moving periods
    calibration_2_signal_history_.push_back(signal_);

    // case 1: last frame is moving and this frame is not moving
    if(!is_moving_ && prev_is_moving_){
        moving_frames_ = 0;
    }

    // case 2: compute static vector if we have enough moving data
    if (moving_frames_ > MOVING_PERIOD_MIN_FRAMES) {
        OfflineCalcStaticSignal();
        OfflineRemoveStaticSignal();
//        BackgroundNotRemove = false;
        status_ = CALI_SUCCESS;
        moving_frames_ = 0;
    }

//    if(!BackgroundNotRemove && !is_moving_ && prev_is_moving_){
//        status_ = CALI_SUCCESS;
//        moving_frames_ = 0;
//    }

    prev_is_moving_ = is_moving_;

    // case 3: if moving for a long time, break 
    if (calibration_2_signal_history_.size() > CALI_2_MAX_FRAMES) {
        status_ = CALI_FAILED;
        cout << "Motion is not detected for " << CALI_2_MAX_FRAMES * N_ZC_UP / 48e3 << " seconds!" << endl;
        cout << "Program terminated!" << endl;
    }
}

void Denoiser::ProcessCalibrationSuccess() {
    // cout << moving_frames_ << endl;

    CheckMoving();

    if (is_moving_) {
        // mean vector of cir in moving periods
        updated_static_signal_ = (signal_ + (moving_frames_ - 1) * updated_static_signal_) / (moving_frames_);
    }
//    updated_static_signal_ = (signal_ + (moving_frames_ - 1) * updated_static_signal_) / (moving_frames_);

    // if need updating
    if (moving_frames_ >= MOVING_PERIOD_MIN_FRAMES) {
        OnlineUpdateStaticSignal();
        // if update sucess, reset these two parameters
        updated_static_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
        moving_frames_ = 0;
    }

    // // non-sufficient moving frames
    if (prev_is_moving_ && !is_moving_) {
        updated_static_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
        moving_frames_ = 0;
    }

    OnlineRemoveStaticSignal();

    prev_is_moving_ = is_moving_;
}

// * pass
void Denoiser::OfflineCalcStaticSignal() {
//    int len = calibration_2_signal_history_.size() - 1;
//    for (int i = 0; i <= MOVING_PERIOD_MIN_FRAMES; i++) {
//        static_signal_ += calibration_2_signal_history_[len - i];
//        total_moving_frames_++;
//    }
//
//    cout << "Cali2 frame size: " << total_moving_frames_ << endl;
//
//    static_signal_ /=  total_moving_frames_;

    int len = calibration_1_singal_history_.size() - 1;
    int moving_frame_counter = 0;
    for (int i = 0; i <= len; i++) {
        if(compute_thre_taps[len - i] > mean_compute_thre){
            static_signal_ += calibration_1_singal_history_[len - i];
            moving_frame_counter++;
        }
    }

    cout << "Cali2 frame size: " << moving_frame_counter << endl;

    static_signal_ /=  moving_frame_counter;

}

// donot imapct on the calculation of static signal
// * pass
void Denoiser::OfflineRemoveStaticSignal() {
    for (const MatrixX<complex<double>>& calibration_2_signal : calibration_2_signal_history_) {
        offline_denoise_signals_.emplace_back(calibration_2_signal - static_signal_);
    }
}

// offline first - we will have a static signal before online algorithm
// * pass
void Denoiser::OnlineUpdateStaticSignal() {
    moving_frames_counter ++;
    static_signal_ = (1 - updata_factor) * static_signal_ + updata_factor * updated_static_signal_;
}

// * pass
void Denoiser::OnlineRemoveStaticSignal() {
     online_denoise_signal_ = signal_ - static_signal_;
//    online_denoise_signal_ = signal_;

//    std::string s2 = std::to_string(out->size[1]);
//    char const *pchar2 = s2.c_str();
//    LoggerUtil::Log("generated dim 2", pchar2);
}

//void Denoiser::cdouble2creal_T(MatrixX<complex<double>> &v, creal_T R[480]){
//    for (int ti = 0; ti < N_ZC_UP; ti++){
//        R[ti].re = v(0, ti).real();
//        R[ti].im = v(0, ti).imag();
//    }
//}


// * pass
void Denoiser::CheckMoving() {
    bool update_moving_threshold = false;
    max_diff = MaxDiff(prev_signal_, signal_);
//    double max_diff = TapDiff(prev_signal_, signal_);

    double max_diff_update_factore = 0.04;
    max_diff_histry = max_diff_histry * (1-max_diff_update_factore) + max_diff_update_factore * max_diff;


    is_moving_ = max_diff > moving_threshold_;
    string prompt;
    if(is_moving_){
        prompt = "Moving Threshold is " + to_string(moving_threshold_) + " Is Moving!";
    }else{
        prompt = "Moving Threshold is " + to_string(moving_threshold_) + " No motion is detected!";
    }

    LoggerUtil::Log("MovingThreshold", prompt.c_str());

    if (is_moving_) {
        moving_frames_++;
    }


    if(update_moving_threshold){
        double minimum_update = 0.0;
        if(moving_threshold_ > thresholding_factor * max_diff_histry){
            minimum_update = max_diff_histry * thresholding_factor * thresholding_update;
        }
        double ave_update = max_diff_histry * ave_update_factor;
        moving_threshold_ = moving_threshold_ * (1 - ave_update_factor - thresholding_update) + ave_update + minimum_update;
    }
}

// * pass
double Denoiser::MaxDiff(const MatrixX<complex<double>> &m1, const MatrixX<complex<double>> &m2) {
    int rows = (int) m1.rows(), cols = (int) m1.cols();
    assert(m2.rows() == rows && m2.cols() == cols);

    double res = 0.0;

    for (int i = 0; i < rows; i++) {
        for (int j = 30; j < 40; j++) {
            double diff = abs(m1(i, j) - m2(i, j));
            if (diff > res) {
                res = diff;
            }
        }
    }
    diff_history_.push_back(res);
    while(diff_history_.size() > 1024){
        diff_history_.pop_front();
    }
    // cout << "diff_history_ capacity: " << diff_history_.size() << endl;

    return res;
}

// * pass
double Denoiser::TapDiff(const MatrixX<complex<double>> &m1, const MatrixX<complex<double>> &m2) {
    int rows = (int) m1.rows(), cols = (int) m1.cols();
    assert(m2.rows() == rows && m2.cols() == cols);
    int tap = selected_tap_for_thresholding;

    double res = 0.0;

    for (int i = 0; i < rows; i++) {
//        for (int j = 0; j < 100; j++) {
            double diff = abs(m1(i, tap) - m2(i, tap));

            if (diff > res) {
                res = diff;
            }
//        }
    }
    diff_history_.push_back(res);
    while(diff_history_.size() > 1024){
        diff_history_.pop_front();
    }
    // cout << "diff_history_ capacity: " << diff_history_.size() << endl;

    return res;
}

//double Denoiser::mvMedian(double x){
//    return NoiseSupression::mvMedian(x, mvMedian_buffer, &mvMedian_iter);
//}

bool Denoiser::getMovingStatus(){
    return is_moving_;
}
