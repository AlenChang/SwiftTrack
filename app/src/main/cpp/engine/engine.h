#ifndef ACOUSTICTRACKER_ENGINE_H
#define ACOUSTICTRACKER_ENGINE_H

#include "denoiser.h"
#include "postprocessor.h"
#include "preprocessor.h"
#include "filter_util.hpp"
#include "matrix_util.hpp"

class Engine {
public:
    static Engine* GetInstance(int id);

    static double ProcessFrame(int id, const double *data, int n);

    static void GetVelocityHistory(int id, double *history, int n);

    static void GetDistHistory(int id, double *history, int n);

    static void Reset(int id);

private:
    Engine();

    ~Engine();

    double ProcessFrameCore(const MatrixX<double> &rx_signal);

    static Engine *instance1, *instance2;

    Denoiser::status_type prev_status_;
    Denoiser::status_type cur_status_;

    Preprocessor *preprocessor_;
    Denoiser *denoiser_;
    Postprocessor *postprocessor_;
};

#endif
