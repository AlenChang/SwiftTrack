#include "engine.h"

Engine *Engine::instance1 = nullptr;
Engine *Engine::instance2 = nullptr;

using namespace std;

Engine::Engine() {

    // from real-valued audio signal to complex-valued cir
    // methods irrelevant
    preprocessor_ = new Preprocessor();

    // remove background noise
    // two different approaches - DDBR or our method
    denoiser_ = new Denoiser();

    // deploy different methods
    // Four different approaches - ToF, Strata, SwiftTrack w/o TOF, SwiftTrack
    postprocessor_ = new Postprocessor();


    prev_status_ = Denoiser::CALI_1;
    cur_status_ = Denoiser::CALI_1;

}

Engine::~Engine() {
    delete preprocessor_;
    delete denoiser_;
    delete postprocessor_;

    cout << "Engine::~Engine() Destroyed OK." << endl;
}


Engine* Engine::GetInstance(int id) {
    if (id == 1) {
        if (instance1 == nullptr) {
            instance1 = new Engine();
        }
        return instance1;
    }

    if (id == 2) {
        if (instance2 == nullptr) {
            instance2 = new Engine();
        }
        return instance2;
    }

    cout << "Engine::GetInstance Invalid engine id." << endl;

    return nullptr;
}

void Engine::ProcessFrame(int id, const double *data, int n) {
    Engine *engine = Engine::GetInstance(id);
    // 把数据从double array转成eigen能处理的格式
    MatrixX<double> rx_signal(1, n);
    for (int i = 0; i < n; i++) {
        rx_signal(0, i) = *(data + i);
    }

    engine->ProcessFrameCore(rx_signal);
}

void Engine::GetVelocityHistory(int id, double *history, int n) {
    Engine *engine = Engine::GetInstance(id);

    engine->postprocessor_->GetVelocityHistory(history, n);
}

void Engine::GetVelocityHistory(int id, double *history, int n, int history_id) {
    Engine *engine = Engine::GetInstance(id);
    Histories history_profile = engine->postprocessor_->GetHistories(history_id);

    engine->postprocessor_->GetVelocityHistory(history, n, history_profile);
}


void Engine::GetDistHistory(int id, double *history, int n) {
    Engine *engine = Engine::GetInstance(id);

    engine->postprocessor_->GetDistHistory(history, n);
}

void Engine::GetDistHistory(int id, double *history, int n, int history_id) {
    Engine *engine = Engine::GetInstance(id);
    Histories history_profile = engine->postprocessor_->GetHistories(history_id);

    engine->postprocessor_->GetDistHistory(history, n, history_profile);
}

void Engine::GetCIR(int id, double *cir_abs, int n){
    Engine *engine = Engine::GetInstance(id);
    engine->postprocessor_->GetCIR(cir_abs, n);
}

void Engine::Reset(int id) {
    if (id == 1) {
        delete instance1;
        instance1 = new Engine();
        // LoggerUtil::Log("Engine::Reset", "Reset instance1 OK.");
        return;
    }

    if (id == 2) {
        delete instance2;
        instance2 = new Engine();
        // LoggerUtil::Log("Engine::Reset", "Reset instance2 OK.");
        return;
    }

    // LoggerUtil::Log("Engine::Reset", "Invalid engine id.");
}


void Engine::ProcessFrameCore(const MatrixX<double> &rx_signal) {

    // compute CIR
    MatrixX<complex<double>> cir_signal = preprocessor_->GenerateCIRSignal(rx_signal);

    // simply feed signal to the denoiser
    // the denoiser will determine the next step
    denoiser_->FeedSignal(cir_signal);
    cur_status_ = denoiser_->GetStatus();
    bool is_moving = denoiser_->getMovingStatus();

    // if calibration is not ready, the distance will always be 0
    if (cur_status_ == Denoiser::CALI_SUCCESS) {
        if (prev_status_ == Denoiser::CALI_2) {
            vector<MatrixX<complex<double>>> denoise_signals = denoiser_->GetOfflineDenoiseSignals();
            for (const MatrixX<complex<double>> &denoise_signal : denoise_signals) {
                // update distance in calibration stage -> frame by frame
                // different approaches should be applied here
                postprocessor_->ProcessCIRSignal(denoise_signal, is_moving);
            }
        } else {
            // compute online distance data
            // different approaches should be applied here
            MatrixX<complex<double>> denoise_signal = denoiser_->GetOnlineDenoiseSignal();
            postprocessor_->ProcessCIRSignal(denoise_signal, is_moving);
        }
    } else if (cur_status_ == Denoiser::CALI_1) {
        postprocessor_->PaddingZero();
    }

    prev_status_ = cur_status_;

}
