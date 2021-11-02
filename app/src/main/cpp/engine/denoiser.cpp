#include "denoiser.h"

Denoiser::Denoiser() {
    status_ = CALI_1;

    prev_signal_ = MatrixX<complex<double>>::Constant(1, FRAME_SIZE, complex<double>(0, 0));
    signal_ = MatrixX<complex<double>>::Constant(1, FRAME_SIZE, complex<double>(0, 0));
    online_denoise_signal_ = MatrixX<complex<double>>::Constant(1, FRAME_SIZE, complex<double>(0, 0));

    prev_is_moving_ = false;
    is_moving_ = false;

    calibration_1_frame_count_ = 0;
    moving_threshold_ = 0.0;

    calibration_2_moving_periods_ = 0;

    moving_frames_ = 0;
    total_moving_frames_ = 0;
    updated_static_signal_ = MatrixX<complex<double>>::Constant(1, FRAME_SIZE, complex<double>(0, 0));
    static_signal_ = MatrixX<complex<double>>::Constant(1, FRAME_SIZE, complex<double>(0, 0));

    cout << "Denoiser was initiated." << endl;
}

Denoiser::~Denoiser() {
    cout << "Denoiser was recycled." << endl;
}

void Denoiser::FeedSignal(const MatrixX<complex<double>> &signal) {
    assert(status_ != CALI_FAILED);

    signal_ = signal;

    if (status_ == CALI_1) {
        ProcessCalibration1();
    } else if (status_ == CALI_2) {
        ProcessCalibration2();
    } else if (status_ == CALI_SUCCESS) {
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

void Denoiser::ProcessCalibration1() {
    calibration_1_frame_count_++;
    if (calibration_1_frame_count_ == 1) {
        return;
    }

    calibration_1_diff_history_.push_back(MaxDiff(prev_signal_, signal_));

    if (calibration_1_frame_count_ == CALI_1_FRAMES) {
        OfflineCalcThreshold();
        status_ = CALI_2;
    }
}

void Denoiser::ProcessCalibration2() {
    CheckMoving();

    calibration_2_moving_history_.push_back(is_moving_);
    calibration_2_signal_history_.push_back(signal_);

    if (prev_is_moving_ && !is_moving_) {
        if (moving_frames_ >= MOVING_PERIOD_MIN_FRAMES) {
            calibration_2_moving_periods_++;
        } else {
            int k = (int) calibration_2_moving_history_.size() - 1;
            while (k >= 0 && calibration_2_moving_history_[k]) {
                calibration_2_moving_history_[k] = !calibration_2_moving_history_[k];
                k--;
            }
        }
        moving_frames_ = 0;
    }

    if (calibration_2_moving_periods_ == CALI_2_PERIODS) {
        OfflineCalcStaticSignal();
        OfflineRemoveStaticSignal();
        status_ = CALI_SUCCESS;
    }

    prev_is_moving_ = is_moving_;

    if (calibration_2_moving_history_.size() > CALI_2_MAX_FRAMES) {
        status_ = CALI_FAILED;
    }
}

void Denoiser::ProcessCalibrationSuccess() {
    // cout << moving_frames_ << endl;

    CheckMoving();

    if (is_moving_) {
        updated_static_signal_ = (signal_ + moving_frames_ * updated_static_signal_) / (moving_frames_ + 1);
        moving_frames_++;
    }

    if (prev_is_moving_ && !is_moving_) {
        if (moving_frames_ >= MOVING_PERIOD_MIN_FRAMES) {
            OnlineUpdateStaticSignal();
        }
        OnlineRemoveStaticSignal();
        updated_static_signal_ = MatrixX<complex<double>>::Constant(1, FRAME_SIZE, complex<double>(0, 0));
        moving_frames_ = 0;
    } else {
        OnlineRemoveStaticSignal();
    }

    prev_is_moving_ = is_moving_;
}

void Denoiser::OfflineCalcThreshold() {
    double mean_diff = 0.0;
    for (double diff : calibration_1_diff_history_) {
        mean_diff += diff;
    }
    mean_diff /= (calibration_1_frame_count_ - 1);

    double var_diff = 0.0;
    for (double diff : calibration_1_diff_history_) {
        var_diff += pow(diff - mean_diff, 2);
    }
    var_diff /= (calibration_1_frame_count_ - 1);

    // moving_threshold_ = mean_diff + 3 * sqrt(var_diff);
    moving_threshold_ = 3 * mean_diff;
}

void Denoiser::OfflineCalcStaticSignal() {
    for (int i = 0; i < calibration_2_moving_history_.size(); i++) {
        if (calibration_2_moving_history_[i]) {
            static_signal_ += calibration_2_signal_history_[i];
            total_moving_frames_++;
        }
    }

    static_signal_ /=  total_moving_frames_;
}

void Denoiser::OfflineRemoveStaticSignal() {
    for (const MatrixX<complex<double>>& calibration_2_signal : calibration_2_signal_history_) {
        offline_denoise_signals_.emplace_back(calibration_2_signal - static_signal_);
    }
}

void Denoiser::OnlineUpdateStaticSignal() {
    moving_frame_history_.push_back(moving_frames_);
    moving_signal_history_.push_back(updated_static_signal_);

    // Online update static signal every UPDATED_MOVING_PERIODS periods
    if (moving_frame_history_.size() == UPDATED_MOVING_PERIODS) {
        // Calculate update static signal on the weighted average of moving signal history
        updated_static_signal_.setConstant(complex<double>(0, 0));
        int stage_moving_frames = 0;
        for (int i = 0; i < UPDATED_MOVING_PERIODS; i++) {
            stage_moving_frames += moving_frame_history_[i];
            updated_static_signal_ += moving_frame_history_[i] * moving_signal_history_[i];
        }
        updated_static_signal_ /= stage_moving_frames;

        // Apply balance filter method
        static_signal_ = static_signal_ * (1 - UPDATE_FACTOR) + updated_static_signal_ * UPDATE_FACTOR;

        // Apply stream mean update method
        // static_signal_ = static_signal_ * total_moving_frames_ + updated_static_signal_ * stage_moving_frames;
        // total_moving_frames_ += stage_moving_frames;
        // static_signal_ /= total_moving_frames_;

        moving_frame_history_.clear();
        moving_signal_history_.clear();
    }
}

void Denoiser::OnlineRemoveStaticSignal() {
    online_denoise_signal_ = signal_ - static_signal_;
}

void Denoiser::CheckMoving() {
    double max_diff = MaxDiff(prev_signal_, signal_);
    is_moving_ = max_diff > moving_threshold_;

    if (is_moving_) {
        moving_frames_++;
    }
}

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
