#include <jni.h>
#include "engine.h"
#include "logger_util.hpp"

extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_processFrame(
        JNIEnv *env, jobject thiz, jint id, jdoubleArray data, jint n
) {
    // from java type to c type
    jdouble *data_ = (env)->GetDoubleArrayElements(data, nullptr);

    if (Engine::GetInstance(id) != nullptr) {
        Engine::ProcessFrame(id, data_, n);
    }
    // from c type to java type
    env->ReleaseDoubleArrayElements(data, data_, JNI_ABORT);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_getVelocityHistory(
        JNIEnv *env, jobject thiz, jint id, jdoubleArray history, jint n
) {
    jdouble *history_ = (env)->GetDoubleArrayElements(history, nullptr);

    if (Engine::GetInstance(id) != nullptr) {
        Engine::GetVelocityHistory(id, history_, n);
    }

    env->ReleaseDoubleArrayElements(history, history_, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_getDistHistory(
        JNIEnv *env, jobject thiz, jint id, jdoubleArray history, jint n
) {
    jdouble *history_ = (env)->GetDoubleArrayElements(history, nullptr);

    if (Engine::GetInstance(id) != nullptr) {
        Engine::GetDistHistory(id, history_, n);
    }

    env->ReleaseDoubleArrayElements(history, history_, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_getDistHistory2(
        JNIEnv *env, jobject thiz, jint id, jdoubleArray history, jint n, jint history_id
) {
    jdouble *history_ = (env)->GetDoubleArrayElements(history, nullptr);

    if (Engine::GetInstance(id) != nullptr) {
        Engine::GetDistHistory(id, history_, n, history_id);
    }

    env->ReleaseDoubleArrayElements(history, history_, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_reset(
        JNIEnv *env, jobject thiz, jint id
) {
    Engine::Reset(id);
}
