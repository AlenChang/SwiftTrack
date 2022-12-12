#include "include/engine.h"
//#include "logger_util.hpp"
#include <string>
#include "recalibrateHistory.h"
#include "fftw3.h"
#include <vector>
#include "recalibrateHistory.h"
#include "recalibrateHistory_emxAPI.h"
#include "recalibrateHistory_terminate.h"
#include "recalibrateHistory_types.h"
#include "rt_nonfinite.h"
using namespace std::chrono;

Engine *Engine::instance1 = nullptr;
Engine *Engine::instance2 = nullptr;

using namespace std;

Engine::Engine() {

//     setup();
    // --------------------------------
    // Check if SDK is expired
    time_t now;
    time(&now);                 // get current time

    struct tm timeinfo;
    timeinfo = *localtime(&now); // convert from time_t to struct tm
    timeinfo.tm_year = expiryYear - 1900;
    timeinfo.tm_mon  = expiryMon - 1;
    timeinfo.tm_mday = expiryDay;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min  = 0;
    timeinfo.tm_sec  = 0;
    expiryDate = mktime(&timeinfo);

    double seconds = difftime(expiryDate, now);
    if(seconds < 0) {
        ifExpiry = true;
//        Util::showDebugMsg("[ERROR] Hauoli", "Hauoli Tracking SDK has expired");
    }else{
        ifExpiry = false;
    }

    prev_status_ = Denoiser::CALI_1;
    cur_status_ = Denoiser::CALI_1;

}

void Engine::setup(int id, int N, int FC, int BW){
    if(ifExpiry) {return;}
    N_ZC_UP_ = N;
    FC_ = FC;
    BW_ = BW;

    time_count = 0;
    // from real-valued audio signal to complex-valued cir
    // methods irrelevant
    preprocessor_ = new Preprocessor(id, N_ZC_UP_, FC_, BW_);

    // remove background noise
    // two different approaches - DDBR or our method
    denoiser_ = new Denoiser(id, N_ZC_UP_);
    
    // deploy different methods
    // Four different approaches - ToF, Strata, SwiftTrack w/o TOF, SwiftTrack
    postprocessor_ = new Postprocessor(id, N_ZC_UP_);

//    processFramePipe_init();
}


Engine::~Engine() {
    delete preprocessor_;
    delete denoiser_;
    delete postprocessor_;

    cout << "Engine::~Engine() Destroyed OK." << endl;
}


Engine* Engine::GetInstance(int id, int N, int FC, int BW) {
//    LoggerUtil::Log("in_c_test", "ready to create instance.");
//    if(ifExpiry) {return;}
    if (id == 0) {
        if (instance1 == nullptr) {
            instance1 = new Engine();
            instance1->setup(id, N, FC, BW);
//            LoggerUtil::Log("in_c_test", "initialize left channel success.");
        }
//        LoggerUtil::Log("in_c_test", "return instance 1.");
        return instance1;
    }

    if (id == 1) {
        if (instance2 == nullptr) {
            instance2 = new Engine();
            instance2->setup(id, N, FC, BW);
//            LoggerUtil::Log("in_c_test", "initialize right channel success.");
        }
//        LoggerUtil::Log("in_c_test", "return instance 2.");
        return instance2;
    }

    cout << "Engine::GetInstance Invalid engine id." << endl;

    return nullptr;
}

Engine* Engine::GetInstance(int id) {

    if (id == 0) {
        return instance1;
    }

    if (id == 1) {
        return instance2;
    }

    cout << "Engine::GetInstance Invalid engine id." << endl;

    return nullptr;
}

void Engine::ProcessFrame(int id, const double *data, int n, int N, int FC, int BW) {
//    if(ifExpiry) {return;}
    Engine *engine = Engine::GetInstance(id, N, FC, BW);

    // 把数据从double array转成eigen能处理的格式
    MatrixX<double> rx_signal(1, n);
    for (int i = 0; i < n; i++) {
        rx_signal(0, i) = *(data + i);
    }
//    LoggerUtil::Log("in_c_test", "process data...");
    engine->ProcessFrameCore(rx_signal);

//    double rx_signal[480];
//    for (int i = 0; i < n; i++) {
//        rx_signal[i] = *(data + i);
//    }
//    engine->ProcessFrameUpsample(rx_signal);
}

void Engine::ProcessFrame_02(int id, const double *data, int n, int N, int FC, int BW){
//    if(ifExpiry) {return;}
    Engine *engine = Engine::GetInstance(id, N, FC, BW);
    double rx_signal[480];
    for (int i = 0; i < n; i++) {
        rx_signal[i] = *(data + i);
    }
//    engine->ProcessFrameUpsample(rx_signal);
}

int Engine::GetHistoryLength(int id, int history_id) {
    Engine *engine = Engine::GetInstance(id);
    Histories history_profile = engine->postprocessor_->GetHistories(history_id);
    return engine->postprocessor_->GetHistoryLength(history_profile);
}

void Engine::GetHistoryData(int id, double *history, int n, int history_id, int history_type){
//    if(ifExpiry) {return;}
    Engine *engine = Engine::GetInstance(id);
    Histories history_profile = engine->postprocessor_->GetHistories(history_id);


    switch(history_type){
        case 0:
            engine->postprocessor_->GetHistoryData(history, n, history_profile, phase_v);
            break;
        case 1:
            engine->postprocessor_->GetHistoryData(history, n, history_profile, velocity_);
            break;
        case 2:
            engine->postprocessor_->GetHistoryData(history, n, history_profile, dist_v);
            break;
        case 3:
            engine->postprocessor_->GetHistoryData(history, n, history_profile, acceleration_);
            break;
        case 4:
            engine->postprocessor_->GetHistoryData(history, n, history_profile, velocity_a);
            break;
        case 5:
            engine->postprocessor_->GetHistoryData(history, n, history_profile, dist_a);
            break;
        case 6:
            engine->postprocessor_->GetHistoryData(history, n, history_profile, phase_acc);
            break;
        case 7:
            engine->postprocessor_->GetHistoryData(history, n, history_profile, time_stamp);
        default:
            break;
    }

}

void Engine::reCalibrate(int id, double* history, int n, double *next_waveform, double *resp_wave, bool *is_body_moving_, bool *is_new_waveform, double *resp_freq){
    Engine *engine = Engine::GetInstance(id);
    const int hist_length = n;

    emxArray_boolean_T *is_body_moving;
    emxArray_real_T *hist;
    emxArray_real_T *hist_out;

    hist = Engine::argInit_Unboundedx1_real_T(hist_length);
//        hist_out = Engine::argInit_Unboundedx1_real_T(hist_length);
//        is_body_moving = Engine::argInit_Unboundedx1_real_T(hist_length);

    // Engine::PrintString("Engine Debug", "emxArray define success!");

//        emxInitArray_real_T(&hist, 1);
    emxInitArray_real_T(&hist_out, 1);
    emxInitArray_boolean_T(&is_body_moving, 1);
    // Engine::PrintString("Engine Debug", "emxArray initialize success!");
//        double hist_in[hist_length];
//        boolean_T is_body_moving[hist_length];

    double waveform[100];
    double resp_freq_ = 0;
    boolean_T new_waveform = false;
//        Engine::PrintDoubleArray(hist->data, 100, "hist value");
    for(int i=0;i<hist_length;i++) {
        hist->data[i] = history[i];
    }
//        Engine::PrintDoubleArray(hist->data, hist_length, "hist value");
//        Engine::PrintString("Engine Debug", "emxArray hist assignment success!");

    recalibrateHistory(hist, 1.0, engine->last_waveform, engine->resp_waveform, hist_out, is_body_moving,waveform,&new_waveform,&resp_freq_);
//        Engine::PrintString("Engine Debug", "Recalibration success!");
    for(int ti = 0; ti < hist_length; ti++){
        is_body_moving_[ti] = is_body_moving->data[ti];
        history[ti] = hist_out->data[ti];
    }
//        Engine::PrintString("Engine Debug", "emxArray output assignment success!");
    *is_new_waveform = new_waveform;
    *resp_freq = resp_freq_;
    for(int ti = 0; ti < 100; ti ++){
        next_waveform[ti] = waveform[ti];
        resp_wave[ti] = engine->resp_waveform[ti];
    }
    emxDestroyArray_real_T(hist);
    emxDestroyArray_real_T(hist_out);
    emxDestroyArray_boolean_T(is_body_moving);
//        Engine::PrintString("Engine Debug", "emxArray destroy success!");

}

void Engine::GetCIR(int id, double *cir_abs, int n){
//    if(ifExpiry) {return;}
    Engine *engine = Engine::GetInstance(id);
    if(engine->denoiser_->GetStatus() == Denoiser::CALI_SUCCESS){
        engine->postprocessor_->GetCIR(cir_abs, n);
    }
    
}

void Engine::GetThreshold(int id, double *thre){
    Engine *engine = Engine::GetInstance(id);
    if(engine->denoiser_->GetStatus() == Denoiser::CALI_SUCCESS){
        *(thre) = engine->denoiser_->moving_threshold_;
        *(thre+1) = engine->denoiser_->max_diff;
        if(engine->denoiser_->moving_threshold_ != 0){
            *(thre+2) = engine->denoiser_->max_diff / engine->denoiser_->moving_threshold_;
        }else{
            *(thre+2) = engine->denoiser_->max_diff;
        }
        
    }
}

MatrixX<complex<double>> Engine::GetCIR_Preprocessor(int id){
    Engine *engine = Engine::GetInstance(id);
    return engine->preprocessor_->getCIRSignal();
}

void Engine::getMovingStatus(int id, bool *status){
//    if(ifExpiry) {return;}
    Engine *engine = Engine::GetInstance(id);
    engine->denoiser_->GetMovingStatus(status);
}

void Engine::GetBeta(int id, double* beta_real, double* beta_imag){
//    if(ifExpiry) {return;}
    Engine *engine = Engine::GetInstance(id);
    engine->postprocessor_->GetBeta(beta_real, beta_imag);
}

void Engine::getTime(int id, double* time){
//    if(ifExpiry) {return;}
    Engine *engine = Engine::GetInstance(id);
    *time = engine->time_count;
}


void Engine::Reset(int id, int N, int FC, int BW) {
//    if(ifExpiry) {return;}
    if (id == 0) {
        delete instance1;
        instance1 = new Engine();
        instance1->setup(id, N, FC, BW);
//         LoggerUtil::Log("in_c_test", "Reset instance1 OK.");
        return;
    }

    if (id == 1) {
        delete instance2;
        instance2 = new Engine();
        instance2->setup(id, N, FC, BW);
//         LoggerUtil::Log("in_c_test", "Reset instance2 OK.");
        return;
    }

    // LoggerUtil::Log("Engine::Reset", "Invalid engine id.");
}

//void Engine::ProcessFrameUpsample(double *data){
//    if(ifExpiry) {return;}
//    coder::array<creal_T, 2U> dist;
//    boolean_T isValid;
//    processFramePipe(data, 480, 60, dist, &isValid);
//}


void Engine::ProcessFrameCore(const MatrixX<double> &rx_signal) {
    if(ifExpiry) {return;}

    auto start = high_resolution_clock::now();
    // compute CIR
    MatrixX<complex<double>> cir_signal = preprocessor_->GenerateCIRSignal(rx_signal);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    time_count = duration.count();

    // simply feed signal to the denoiser
    // the denoiser will determine the next step
    // todo: rewrite denoiser to make it compatible with matlab results.
    denoiser_->FeedSignal(cir_signal);
    cur_status_ = denoiser_->GetStatus();
    bool is_moving = denoiser_->getMovingStatus();

    if (cur_status_ == Denoiser::CALI_SUCCESS) {
        MatrixX<complex<double>> denoise_signal = denoiser_->GetOnlineDenoiseSignal();
        postprocessor_->ProcessCIRSignal(denoise_signal, is_moving);
        // }
    } else if (cur_status_ == Denoiser::CALI_1) {
//         postprocessor_->PaddingZero();
    }

    prev_status_ = cur_status_;

}


//void Engine::detrend_frames(double *data){
//    detrend_alg(data);
//}

void Engine::reset_results() {
    if(instance1 != nullptr){
        instance1->postprocessor_->reset_results();
    }
    if(instance2 != nullptr){
        instance2->postprocessor_->reset_results();
    }

}

// void Engine::PrintString(string val, string name) {
//     ostringstream strs;
//     strs << val << endl;
//     string message=strs.str();
//     LOGD((name+" %s").c_str(),message.c_str());
// }

// void Engine::PrintDoubleArray(double* farray, int n, string name) {
//     ostringstream strs;
//     int numPerLine = 10;
//     for (int i=0; i<n; i++) {
//         strs << farray[i] << " ";
//         if ((i%numPerLine) == numPerLine-1) {
//             strs << endl;
//         }
//     }
//     string message=strs.str();
//     LOGD((name+" %s").c_str(),message.c_str());
// }

// void Engine::PrintIntArray(int* farray, int n, string name) {
//     ostringstream strs;
//     int numPerLine = 10;
//     for (int i=0; i<n; i++) {
//         strs << farray[i] << " ";
//         if ((i%numPerLine) == numPerLine-1) {
//             strs << endl;
//         }
//     }
//     string message=strs.str();
//     LOGD((name+" %s").c_str(),message.c_str());
// }

emxArray_real_T* Engine::argInit_Unboundedx1_real_T(int len)
{
    emxArray_real_T *result;
    double *result_data;
    const int i = len;
    int idx0;
    /* Set the size of the array.
  Change this size to the value that the application requires. */
    result = emxCreateND_real_T(1, &i);
    result_data = result->data;
    /* Loop over the array to initialize each element. */
    for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
        /* Set the value of the array element.
    Change this value to the value that the application requires. */
        result_data[idx0] = 0.0;
    }
    return result;
}

void Engine::genZC(int N_ZC, int N_ZC_UP, int U, int FC, int SAMPLE_RATE, const bool* SPEAKER_CHANNEL, bool USE_WINDOW,double SCALE, vector<vector<double>> & TX_SEQ) {
    // Generate raw zc seq
    fftw_complex * rawZC;
    rawZC = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N_ZC);

    // Transfer raw zc to freq domain
    fftw_complex* output_fft;
    output_fft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N_ZC);

    int b_sign = FFTW_FORWARD;
    fftw_plan plan = fftw_plan_dft_1d(N_ZC,rawZC , output_fft, b_sign,FFTW_MEASURE);
    for(int i = 0; i < N_ZC; i++){
        double theta = - M_PI * U * i * (i + 1) / N_ZC;
        rawZC[i][REAL] = cos(theta);
        rawZC[i][IMAG] = sin(theta);
    }
    fftw_execute(plan);
    fftw_destroy_plan(plan);
    // Apply hann window based on the reference url: https://ww2.mathworks.cn/help/signal/ref/hann.html
    if (USE_WINDOW) {
        for (int i = 0; i < N_ZC; i++) {
            output_fft[i][REAL] = 0.5 * (1 - cos(2 * M_PI  * i / (N_ZC - 1)));
            output_fft[i][IMAG] = 0.5 * (1 - cos(2 * M_PI  * i / (N_ZC - 1)));
        }
    }
    // Padding zeros in freq domain
    fftw_complex * freqPadZC;
    freqPadZC = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N_ZC_UP);

    // Back to time domain

    fftw_complex* output_ifft;
    output_ifft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N_ZC_UP);
    b_sign = FFTW_BACKWARD;
    plan = fftw_plan_dft_1d(N_ZC_UP,freqPadZC , output_ifft, b_sign,FFTW_MEASURE);

    int len = N_ZC_UP - N_ZC;
    for (int i = 0; i < N_ZC_UP;i++){
        freqPadZC[i][REAL] = 0.0;
        freqPadZC[i][IMAG] = 0.0;
    }
    for (int i = 0; i < N_ZC; i++) {
        if (i < (N_ZC + 1) / 2 ) {
            freqPadZC[i][REAL] = output_fft[i][REAL];
            freqPadZC[i][IMAG] = output_fft[i][IMAG];
        } else{
            freqPadZC[i + len][REAL]  = output_fft[i][REAL];
            freqPadZC[i + len][IMAG]  = output_fft[i][IMAG];
        }
    }

    fftw_execute(plan);
    fftw_destroy_plan(plan);

    // Up conversion

    vector<double> seqFrame(N_ZC_UP,0.0);
    double maxValue = 0.0;
    for (int i = 0; i < N_ZC_UP; i++) {
        double theta =  - 2 * M_PI * FC * (i + 1) / SAMPLE_RATE;
        double real = cos(theta);
        double imag = sin(theta);
        seqFrame[i] = output_ifft[i][REAL] * real + output_ifft[i][IMAG] * imag;
        maxValue = max(maxValue, abs(seqFrame[i]));
        if(maxValue == 0){
            maxValue = 1;
        }
    }
    for (int i = 0; i < N_ZC_UP; i++) {
        if (SPEAKER_CHANNEL[0]) {
            TX_SEQ[i][0] = seqFrame[i] / maxValue * SCALE;
        }
        if (SPEAKER_CHANNEL[1]) {
            TX_SEQ[i][1] = seqFrame[i] / maxValue * SCALE;
        }
    }
}