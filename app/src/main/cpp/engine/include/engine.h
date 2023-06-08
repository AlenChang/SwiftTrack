#ifndef ACOUSTICTRACKER_ENGINE_H
#define ACOUSTICTRACKER_ENGINE_H

#include "denoiser.h"
#include "postprocessor.h"
#include "preprocessor.h"
#include "filter_util.hpp"
#include "matrix_util.hpp"
// #include <android/log.h>
// #include <jni.h>
//#include "processFramePipe.h"
//#include "processFramePipe_terminate.h"
//#include "coder_array.h"
//#include "detrend_alg.h"
#include "fftw3.h"
#include "recalibrateHistory.h"
#include "recalibrateHistory_emxAPI.h"
#include "recalibrateHistory_terminate.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"


#define REAL 0
#define IMAG 1

#ifndef M_PI
#define M_PI acos(-1)*1.0
#endif

#define  LOG_TAG    "NDKOutput"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

class Engine {
public:
    static Engine* GetInstance(int id, int N, int FC, int BW);

    static Engine* GetInstance(int id);

    static void ProcessFrame(int id, const double *data, int n, int N, int FC, int BW);
//    static void ProcessFrame(int id, const double *data, int n, int N, int FC, int BW, bool *is_moving_);
    static void ProcessFrame_02(int id, const double *data, int n, int N, int FC, int BW);

    static void GetCIR(int id, double *cir_abs, int n);
    
    static MatrixX<complex<double>> GetCIR_Preprocessor(int id);

    static void getMovingStatus(int id, bool *status);

    static void GetBeta(int id, double* beta_real, double* beta_imag);

    static void GetThreshold(int id, double* thre);

    static void Reset(int id, int N, int FC, int BW);

    static void GetHistoryData(int id, double *history, int n, int history_id, int history_type);

    void setup(int id, int N, int FC, int BW);

    static void getTime(int id, double* time);

    static int GetHistoryLength(int id, int history_id);

    static void reCalibrate(int id, double* history, int n, double *next_waveform, double *resp_wave, bool *is_body_moving_, bool *is_new_waveform, double *resp_freq, bool need_bp);

    // static void PrintString(string val, string name);
    // static void PrintDoubleArray(double* farray, int n, string name);
    // static void PrintIntArray(int* farray, int n, string name);
    static emxArray_real_T* argInit_Unboundedx1_real_T(int len);
//    static void detrend_frames(double *data);

    static void reset_results(void);

    static void genZC(int N_ZC, int N_ZC_UP, int U, int FC, int SAMPLE_RATE, const bool* SPEAKER_CHANNEL, bool USE_WINDOW,double SCALE, vector<vector<double>> & TX_SEQ);

    double time_count;
    double last_waveform[100];
    double resp_waveform[100];

//    static bool is_moving;

private:
    Engine();

    ~Engine();

    int N_ZC_UP_;
    int BW_;
    int FC_;


//    double moving_thre = 1.0;

    // -------------------
    // expiry
    bool ifExpiry = false;
    int expiryYear              = 2023;
    int expiryMon               = 12;
    int expiryDay               = 30;
    time_t expiryDate;
    // -------------------

    void ProcessFrameCore(const MatrixX<double> &rx_signal);
//    void ProcessFrameUpsample(double *data);

    static Engine *instance1, *instance2;

    Denoiser::status_type prev_status_;
    Denoiser::status_type cur_status_;

    Preprocessor *preprocessor_;
    Denoiser *denoiser_;
    Postprocessor *postprocessor_;
};

#endif
