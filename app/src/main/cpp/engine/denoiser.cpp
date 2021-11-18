#include "denoiser.h"
#include "BackgroundRemoval.h"

Denoiser::Denoiser() {
    // first stage calibration -> estimate threshold to determine moving period.
    status_ = CALI_1;

    N_ZC_UP = 480;

    prev_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
    signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
    online_denoise_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));

    prev_is_moving_ = false;
    is_moving_ = false;

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

    cout << "Denoiser was initiated." << endl;
}

Denoiser::~Denoiser() {
    cout << "Denoiser was recycled." << endl;
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

vector<double> Denoiser::GetDiffHistory() {
    return diff_history_;
}

void Denoiser::compute_thre(){
    calibration_1_frame_count_++;
    if (calibration_1_frame_count_ == 1) {
        return;
    }

    double signa_diff[2 * N_ZC_UP];
    complex2double(signal_ - prev_signal_, signa_diff);
    BackgroundRemoval::compute_thre(signa_diff, compute_thre_taps, compute_thre_iter, 6, &init1_flag ,&moving_threshold_);
    compute_thre_iter++;

    if(init1_flag){
        status_ = CALI_2;
        cout << "Threshold is " << moving_threshold_ << endl;
    }
}

void Denoiser::complex2double(const MatrixX<complex<double>> &x, double* out){
    int rows = (int) x.rows(), cols = (int) x.cols();
    assert(rows == 1 && cols * 2 == 2 * N_ZC_UP);

    for (int j = 0; j < cols; j++) {
        *(out + 2*j) = x(j).real();
        *(out + 2*j + 1) = x(j).imag();
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
        status_ = CALI_SUCCESS;
        moving_frames_ = 0;
    }

    prev_is_moving_ = is_moving_;

    // case 3: if moving for a long time, break 
    if (calibration_2_signal_history_.size() > CALI_2_MAX_FRAMES) {
        status_ = CALI_FAILED;
        cout << "Motion is not detected for " << CALI_2_MAX_FRAMES * 0.01 << " seconds!" << endl;
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

    // if need updating
    if (moving_frames_ >= MOVING_PERIOD_MIN_FRAMES) {
        OnlineUpdateStaticSignal();
        // if update sucess, reset these two parameters
        updated_static_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
        moving_frames_ = 0;
    }

    // non-sufficient moving frames
    if (prev_is_moving_ && !is_moving_) {
        updated_static_signal_ = MatrixX<complex<double>>::Constant(1, N_ZC_UP, complex<double>(0, 0));
        moving_frames_ = 0;
    }

    OnlineRemoveStaticSignal();

    prev_is_moving_ = is_moving_;
}

// * pass
void Denoiser::OfflineCalcStaticSignal() {
    int len = calibration_2_signal_history_.size() - 1;
    for (int i = 0; i <= MOVING_PERIOD_MIN_FRAMES; i++) {
        static_signal_ += calibration_2_signal_history_[len - i];
        total_moving_frames_++;
    }

    cout << "Cali2 frame size: " << total_moving_frames_ << endl;

    static_signal_ /=  total_moving_frames_;

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
}

// * pass
void Denoiser::CheckMoving() {
    double max_diff = MaxDiff(prev_signal_, signal_);

    is_moving_ = max_diff > moving_threshold_;

    if (is_moving_) {
        moving_frames_++;
    }
}

// * pass
double Denoiser::MaxDiff(const MatrixX<complex<double>> &m1, const MatrixX<complex<double>> &m2) {
    int rows = (int) m1.rows(), cols = (int) m1.cols();
    assert(m2.rows() == rows && m2.cols() == cols);

    double res = 0.0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double diff = abs(m1(i, j) - m2(i, j));
            if (diff > res) {
                res = diff;
            }
        }
    }

    diff_history_.push_back(res);

    return res;
}

double Denoiser::mvMedian(double x){
    return NoiseSupression::mvMedian(x, mvMedian_buffer, &mvMedian_iter);
}

bool Denoiser::getMovingStatus(){
    return is_moving_;
}
