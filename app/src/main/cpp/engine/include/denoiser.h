#ifndef ACOUSTICTRACKER_DENOISER_H
#define ACOUSTICTRACKER_DENOISER_H

#include "matrix_util.hpp"
#include "NoiseSupression.h"
//#include "logger_util.hpp"

class Denoiser {
public:
    enum status_type {
        CALI_1 = 1,
        CALI_2 = 2,
        CALI_SUCCESS = 3,
        CALI_FAILED = 4
    };

    double moving_threshold_;

    Denoiser(int N_ZC_UP_);

    ~Denoiser();

    void GetMovingStatus(bool * status);

    void FeedSignal(const MatrixX<complex<double>> &signal);

    status_type GetStatus();

    vector<MatrixX<complex<double>>> GetOfflineDenoiseSignals();

    MatrixX<complex<double>> GetOnlineDenoiseSignal();

    vector<double> GetDiffHistory();

    bool getMovingStatus();

//    void main_offerRxUpsample(void);
    void cdouble2creal_T(MatrixX<complex<double>> &v, creal_T R[480]);

private:
    // void ProcessCalibration1();

    void ProcessCalibration2();

    void ProcessCalibrationSuccess();

    // void OfflineCalcThreshold();

    void OfflineCalcStaticSignal();

    void OfflineRemoveStaticSignal();

    void OnlineUpdateStaticSignal();

    void OnlineRemoveStaticSignal();

    void CheckMoving();

    double MaxDiff(const MatrixX<complex<double>> &m1, const MatrixX<complex<double>> &m2);
    double TapDiff(const MatrixX<complex<double>> &m1, const MatrixX<complex<double>> &m2);

    // const int FRAME_SIZE = 480;
    int Fs = 48e3;
    int N_ZC_UP;
    static const int CALI_1_FRAMES = 500;
    const double thre_factor = 2;
    static const int CALI_2_PERIODS = 1;
    const int CALI_2_MAX_FRAMES = ceil(40 * Fs / N_ZC_UP);
    const int MOVING_PERIOD_MIN_FRAMES = ceil(0.1 * Fs / N_ZC_UP);
    const int UPDATED_MOVING_PERIODS = 1;
    const double UPDATE_FACTOR = 0.0;
    const double updata_factor = 0.01;
    const double std_factor = 4;
    double mean_compute_thre = 0.0;

    int moving_frames_counter = 0;
    bool BackgroundNotRemove = true;

    // Remark the denoiser working stage
    status_type status_;

    // Store the prev raw signal
    MatrixX<complex<double>> prev_signal_;

    // Store the cur raw signal
    MatrixX<complex<double>> signal_;

    // Store the cur denoise signal
    MatrixX<complex<double>> online_denoise_signal_;

    // Store the max diff history
    list<double> diff_history_;

    // Store the prev moving status
    bool prev_is_moving_;

    // Store the cur moving status
    bool is_moving_;

    // Variables for offline calculate moving threshold
    int calibration_1_frame_count_;
    // vector<double> calibration_1_diff_history_;


    // Variables for offline calculate static signal
    int calibration_2_moving_periods_;
    vector<MatrixX<complex<double>>> calibration_2_signal_history_;
    vector<MatrixX<complex<double>>> calibration_1_singal_history_;
    vector<MatrixX<complex<double>>> offline_denoise_signals_;

    // Variables for online update static signal
    int moving_frames_;
    int total_moving_frames_;
    MatrixX<complex<double>> updated_static_signal_;
    MatrixX<complex<double>> static_signal_;

    double mvMedian_iter;
    double mvMedian_buffer[5];
    double mvMedian(double x);

    double compute_thre_taps[CALI_1_FRAMES];
    int compute_thre_iter = 0;
    bool init1_flag = false;
    const int selected_tap_for_thresholding = 40;
    double thresholding_factor = 1.2;
    const double thresholding_update = 0.02;
    const double ave_update_factor = 0.01;
    double max_diff_histry = 0.0;

    void compute_thre();

//    ofstream fout;
    
};

#endif
