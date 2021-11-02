#ifndef ACOUSTICTRACKER_DENOISER_H
#define ACOUSTICTRACKER_DENOISER_H

#include "matrix_util.hpp"

class Denoiser {
public:
    enum status_type {
        CALI_1 = 1,
        CALI_2 = 2,
        CALI_SUCCESS = 3,
        CALI_FAILED = 4
    };

    Denoiser();

    ~Denoiser();

    void FeedSignal(const MatrixX<complex<double>> &signal);

    status_type GetStatus();

    vector<MatrixX<complex<double>>> GetOfflineDenoiseSignals();

    MatrixX<complex<double>> GetOnlineDenoiseSignal();

    vector<double> GetDiffHistory();

private:
    void ProcessCalibration1();

    void ProcessCalibration2();

    void ProcessCalibrationSuccess();

    void OfflineCalcThreshold();

    void OfflineCalcStaticSignal();

    void OfflineRemoveStaticSignal();

    void OnlineUpdateStaticSignal();

    void OnlineRemoveStaticSignal();

    void CheckMoving();

    double MaxDiff(const MatrixX<complex<double>> &m1, const MatrixX<complex<double>> &m2);

    const int FRAME_SIZE = 480;
    const int CALI_1_FRAMES = 50;
    const int CALI_2_PERIODS = 4;
    const int CALI_2_MAX_FRAMES = 2000;
    const int MOVING_PERIOD_MIN_FRAMES = 20;
    const int UPDATED_MOVING_PERIODS = 10;
    const double UPDATE_FACTOR = 0.0;

    // Remark the denoiser working stage
    status_type status_;

    // Store the prev raw signal
    MatrixX<complex<double>> prev_signal_;

    // Store the cur raw signal
    MatrixX<complex<double>> signal_;

    // Store the cur denoise signal
    MatrixX<complex<double>> online_denoise_signal_;

    // Store the max diff history
    vector<double> diff_history_;

    // Store the prev moving status
    bool prev_is_moving_;

    // Store the cur moving status
    bool is_moving_;

    // Variables for offline calculate moving threshold
    int calibration_1_frame_count_;
    vector<double> calibration_1_diff_history_;
    double moving_threshold_;

    // Variables for offline calculate static signal
    int calibration_2_moving_periods_;
    vector<bool> calibration_2_moving_history_;
    vector<MatrixX<complex<double>>> calibration_2_signal_history_;
    vector<MatrixX<complex<double>>> offline_denoise_signals_;

    // Variables for online update static signal
    int moving_frames_;
    int total_moving_frames_;
    vector<int> moving_frame_history_;
    vector<MatrixX<complex<double>>> moving_signal_history_;
    MatrixX<complex<double>> updated_static_signal_;
    MatrixX<complex<double>> static_signal_;
};

#endif
