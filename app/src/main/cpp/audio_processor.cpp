#include <jni.h>
#include "engine.h"
#include "logger_util.hpp"

extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_processFrame(
        JNIEnv *env, jobject thiz, jint id, jdoubleArray data, jint n, jint N_ZC_UP
) {
    // from java type to c type
    jdouble *data_ = (env)->GetDoubleArrayElements(data, nullptr);

    if (Engine::GetInstance(id, N_ZC_UP) != nullptr) {
//        LoggerUtil::Log("in_c_test", "Ready to process data");
        Engine::ProcessFrame(id, data_, n, N_ZC_UP);
    }
    // from c type to java type
    env->ReleaseDoubleArrayElements(data, data_, JNI_ABORT);
}



extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_getHistoryData(
        JNIEnv *env, jobject thiz, jint id, jdoubleArray history, jint n, jint history_id, jint history_type
) {
    jdouble *history_ = (env)->GetDoubleArrayElements(history, nullptr);

    if (Engine::GetInstance(id) != nullptr) {
        Engine::GetHistoryData(id, history_, n, history_id, history_type);
    }

    env->ReleaseDoubleArrayElements(history, history_, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_getCIR(
        JNIEnv *env, jobject thiz, jint id, jdoubleArray cir_abs, jint n
) {
    jdouble *history_ = (env)->GetDoubleArrayElements(cir_abs, nullptr);

    if (Engine::GetInstance(id) != nullptr) {
        Engine::GetCIR(id, history_, n);
    }

    env->ReleaseDoubleArrayElements(cir_abs, history_, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_reset(
        JNIEnv *env, jobject thiz, jint id, jint N
) {
    Engine::Reset(id, N);
}
