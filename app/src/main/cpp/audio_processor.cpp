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

//extern "C"
//JNIEXPORT void JNICALL
//Java_com_example_swifttrack_AudioProcessor_processFrame03(
//        JNIEnv *env, jobject thiz, jint id, jdoubleArray data, jint n, jint N_ZC_UP, jint FC, jint BW, jbooleanArray is_moving
//) {
//    // from java type to c type
//    jdouble *data_ = (env)->GetDoubleArrayElements(data, nullptr);
//    jboolean *is_moving_ = (env)->GetBooleanArrayElements(is_moving, nullptr);
//    bool is_moving__ = false;
//
//    if (Engine::GetInstance(id, N_ZC_UP, FC, BW) != nullptr) {
////        LoggerUtil::Log("in_c_test", "Ready to process data");
//        Engine::ProcessFrame(id, data_, n, N_ZC_UP, FC, BW, &is_moving__);
//    }
//    *is_moving_ = is_moving__;
//    // from c type to java type
//    env->ReleaseDoubleArrayElements(data, data_, JNI_ABORT);
//    env->ReleaseBooleanArrayElements(is_moving, is_moving_, 0);
//}

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



// GetHistoryData(int id, double *history, double *next_waveform, double *resp_wave, bool *is_body_moving_, bool *is_new_waveform, int n, int history_id, int history_type);
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
Java_com_example_swifttrack_AudioProcessor_recalibrate(
        JNIEnv *env, jobject thiz, jint id, jdoubleArray history, jdoubleArray next_waveform, jdoubleArray resp_wave, jbooleanArray is_new_waveform, jbooleanArray is_body_moving, jdoubleArray resp_freq, jint n, jboolean need_bp
) {
    jdouble *history_ = (env)->GetDoubleArrayElements(history, nullptr);
    jdouble *next_waveform_ = (env)->GetDoubleArrayElements(next_waveform, nullptr);
    jdouble *resp_wave_ = (env)->GetDoubleArrayElements(resp_wave, nullptr);
    jboolean *is_body_moving_ = (env)->GetBooleanArrayElements(is_body_moving, nullptr);
    jdouble *resp_freq_ = (env)->GetDoubleArrayElements(resp_freq, nullptr);
    jboolean *is_new_waveform_ = (env)->GetBooleanArrayElements(is_new_waveform, nullptr);

    bool moving_flag[n];
    bool is_new_waveform__ = false;

    if (Engine::GetInstance(id) != nullptr) {
        Engine::reCalibrate(id, history_, n, next_waveform_,resp_wave_, moving_flag, &is_new_waveform__, resp_freq_, need_bp);
    }

    for(int ti = 0; ti < n; ti ++){
        is_body_moving_[ti] = moving_flag[ti];
    }
    *is_new_waveform_ = is_new_waveform__;
//    resp_freq = 10;

    env->ReleaseDoubleArrayElements(history, history_, 0);
    env->ReleaseDoubleArrayElements(next_waveform, next_waveform_, 0);
    env->ReleaseDoubleArrayElements(resp_wave, resp_wave_, 0);
    env->ReleaseBooleanArrayElements(is_body_moving, is_body_moving_, 0);
    env->ReleaseDoubleArrayElements(resp_freq, resp_freq_, 0);
    env->ReleaseBooleanArrayElements(is_new_waveform, is_new_waveform_, 0);
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
Java_com_example_swifttrack_AudioProcessor_getHistoryLength(
        JNIEnv *env, jobject thiz, jint id, jint history_id, jintArray length
) {
    jint *length_ = (env)->GetIntArrayElements(length, nullptr);
//    Engine::detrend_frames(history_);
    if (Engine::GetInstance(id) != nullptr) {
        *length_ = Engine::GetHistoryLength(id, history_id);
    }
    env->ReleaseIntArrayElements(length, length_, 0);
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

