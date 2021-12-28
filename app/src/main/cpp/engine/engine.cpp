#include "engine.h"
//#include "logger_util.hpp"
#include <string>

using namespace std::chrono;

Engine *Engine::instance1 = nullptr;
Engine *Engine::instance2 = nullptr;

using namespace std;

Engine::Engine() {

//     setup();

    prev_status_ = Denoiser::CALI_1;
    cur_status_ = Denoiser::CALI_1;

}

void Engine::setup(int N){
    N_ZC_UP_ = N;
    time_count = 0;
    // from real-valued audio signal to complex-valued cir
    // methods irrelevant
    preprocessor_ = new Preprocessor(N_ZC_UP_);

    // remove background noise
    // two different approaches - DDBR or our method
    denoiser_ = new Denoiser(N_ZC_UP_);
    
    // deploy different methods
    // Four different approaches - ToF, Strata, SwiftTrack w/o TOF, SwiftTrack
    postprocessor_ = new Postprocessor(N_ZC_UP_);
}


Engine::~Engine() {
    delete preprocessor_;
    delete denoiser_;
    delete postprocessor_;

    cout << "Engine::~Engine() Destroyed OK." << endl;
}


Engine* Engine::GetInstance(int id, int N) {
//    LoggerUtil::Log("in_c_test", "ready to create instance.");
    if (id == 0) {
        if (instance1 == nullptr) {
            instance1 = new Engine();
            instance1->setup(N);
//            LoggerUtil::Log("in_c_test", "initialize left channel success.");
        }
//        LoggerUtil::Log("in_c_test", "return instance 1.");
        return instance1;
    }

    if (id == 1) {
        if (instance2 == nullptr) {
            instance2 = new Engine();
            instance2->setup(N);
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

void Engine::ProcessFrame(int id, const double *data, int n, int N) {
    Engine *engine = Engine::GetInstance(id, N);
    // 把数据从double array转成eigen能处理的格式
    MatrixX<double> rx_signal(1, n);
    for (int i = 0; i < n; i++) {
        rx_signal(0, i) = *(data + i);
    }
//    LoggerUtil::Log("in_c_test", "process data...");
    engine->ProcessFrameCore(rx_signal);
}

void Engine::GetHistoryData(int id, double *history, int n, int history_id, int history_type){
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
        default:
            break;
    }
}

void Engine::GetCIR(int id, double *cir_abs, int n){
    Engine *engine = Engine::GetInstance(id);
    if(engine->denoiser_->GetStatus() == Denoiser::CALI_SUCCESS){
        engine->postprocessor_->GetCIR(cir_abs, n);
    }
    
}

void Engine::getMovingStatus(int id, bool *status){
    Engine *engine = Engine::GetInstance(id);
    engine->denoiser_->GetMovingStatus(status);
}

void Engine::GetBeta(int id, double* beta_real, double* beta_imag){
    Engine *engine = Engine::GetInstance(id);
    engine->postprocessor_->GetBeta(beta_real, beta_imag);
}

void Engine::getTime(int id, double* time){
    Engine *engine = Engine::GetInstance(id);
    *time = engine->time_count;
}


void Engine::Reset(int id, int N) {
    if (id == 0) {
        delete instance1;
        instance1 = new Engine();
        instance1->setup(N);
//         LoggerUtil::Log("in_c_test", "Reset instance1 OK.");
        return;
    }

    if (id == 1) {
        delete instance2;
        instance2 = new Engine();
        instance2->setup(N);
//         LoggerUtil::Log("in_c_test", "Reset instance2 OK.");
        return;
    }

    // LoggerUtil::Log("Engine::Reset", "Invalid engine id.");
}


void Engine::ProcessFrameCore(const MatrixX<double> &rx_signal) {

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
        // postprocessor_->PaddingZero();
    }

    prev_status_ = cur_status_;

}
