#ifndef ACOUSTICTRACKER_ENGINE_H
#define ACOUSTICTRACKER_ENGINE_H

#include "denoiser.h"
#include "postprocessor.h"
#include "preprocessor.h"
#include "filter_util.hpp"
#include "matrix_util.hpp"

class Engine {
public:
    static Engine* GetInstance(int id, int N);

    static Engine* GetInstance(int id);

    static void ProcessFrame(int id, const double *data, int n, int N);

    static void GetCIR(int id, double *cir_abs, int n);

    static void getMovingStatus(int id, bool *status);

    static void GetBeta(int id, double* beta_real, double* beta_imag);

    static void Reset(int id, int N);

    static void GetHistoryData(int id, double *history, int n, int history_id, int history_type);

    void setup(int N);

    static void getTime(int id, double* time);

    double time_count;

private:
    Engine();

    ~Engine();

    int N_ZC_UP_;

    void ProcessFrameCore(const MatrixX<double> &rx_signal);

    static Engine *instance1, *instance2;

    Denoiser::status_type prev_status_;
    Denoiser::status_type cur_status_;

    Preprocessor *preprocessor_;
    Denoiser *denoiser_;
    Postprocessor *postprocessor_;
};

#endif
