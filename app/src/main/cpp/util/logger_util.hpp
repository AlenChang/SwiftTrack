#ifndef SONAR_LOGGER_UTIL_HPP
#define SONAR_LOGGER_UTIL_HPP

#include <android/log.h>
#include <stdio.h>

class LoggerUtil {
public:
    static void Log(const char *flag, char *s) {
        __android_log_print(ANDROID_LOG_DEBUG, flag, "%s", s);
    }
    static void Log(const char *flag, const char *s) {
        __android_log_print(ANDROID_LOG_DEBUG, flag, "%s", s);
    }
};

#endif
