#include <jni.h>
#include "include/engine.h"
//#include "logger_util.hpp"
#include <math.h>


extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_processFrame(
        JNIEnv *env, jobject thiz, jint id, jdoubleArray data, jint n, jint N_ZC_UP, jint FC, jint BW
) {
    // from java type to c type
    jdouble *data_ = (env)->GetDoubleArrayElements(data, nullptr);

    if (Engine::GetInstance(id, N_ZC_UP, FC, BW) != nullptr) {
//        LoggerUtil::Log("in_c_test", "Ready to process data");
        Engine::ProcessFrame(id, data_, n, N_ZC_UP, FC, BW);
    }
    // from c type to java type
    env->ReleaseDoubleArrayElements(data, data_, JNI_ABORT);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_getTime(
        JNIEnv *env, jobject thiz, jint id, jdoubleArray time_count
) {
    // from java type to c type
    jdouble *time_count_ = (env)->GetDoubleArrayElements(time_count, nullptr);
    if (Engine::GetInstance(id) != nullptr) {
//        LoggerUtil::Log("in_c_test", "Ready to process data");
        Engine::getTime(id, time_count_);
    }
    env->ReleaseDoubleArrayElements(time_count, time_count_, JNI_ABORT);
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
        JNIEnv *env, jobject thiz, jint id, jint N, jint FC, jint BW
) {
    Engine::Reset(id, N, FC, BW);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_getThre(
        JNIEnv *env, jobject thiz, jint id, jdoubleArray thre
) {
    jdouble *history_ = (env)->GetDoubleArrayElements(thre, nullptr);


    if (Engine::GetInstance(id) != nullptr) {
        Engine::GetThreshold(id, history_);
    }

    env->ReleaseDoubleArrayElements(thre, history_, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_detrend(
        JNIEnv *env, jobject thiz, jdoubleArray data
) {
    jdouble *history_ = (env)->GetDoubleArrayElements(data, nullptr);
//    Engine::detrend_frames(history_);
    env->ReleaseDoubleArrayElements(data, history_, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_swifttrack_AudioProcessor_resetResults(
        JNIEnv *env, jobject thiz
) {
    Engine::reset_results();
}
extern "C"
JNIEXPORT void JNICALL Java_com_example_swifttrack_AudioProcessor_genZCSeq(JNIEnv* env, jclass thisObj,
                                                                        jint N_ZC, jint U, jboolean USE_WINDOW,
                                                                        jint N_ZC_UP, jint FC, jint SAMPLE_RATE,
                                                                        jbooleanArray SPEAKER_CHANNEL_MASK,
                                                                        jdouble SCALE, jobjectArray TX_SEQ){
    // Scaling
    jboolean * SPEAKER_CHANNEL =  (env)->GetBooleanArrayElements(SPEAKER_CHANNEL_MASK, nullptr);

    vector<vector<double>> TX_SEQ_IN(N_ZC_UP,vector<double>(2,0));
    bool SPEAKER_CHANNEL_[2];
    SPEAKER_CHANNEL_[0] = SPEAKER_CHANNEL[0];
    SPEAKER_CHANNEL_[1] = SPEAKER_CHANNEL[1];
    Engine::genZC(N_ZC, N_ZC_UP, U, FC, SAMPLE_RATE, SPEAKER_CHANNEL_, USE_WINDOW,SCALE ,TX_SEQ_IN);
    for (int i = 0; i < N_ZC_UP; i++) {
        jdoubleArray TX_SEQ_ = (jdoubleArray)env->GetObjectArrayElement(TX_SEQ, i);
        jboolean jb = JNI_FALSE;
        jdouble * TX_SEQ_i =  (env)->GetDoubleArrayElements(TX_SEQ_,&jb);
        TX_SEQ_i[0] = TX_SEQ_IN[i][0];
        TX_SEQ_i[1] = TX_SEQ_IN[i][1];
        env->ReleaseDoubleArrayElements(TX_SEQ_, TX_SEQ_i, 0);
    }
}

