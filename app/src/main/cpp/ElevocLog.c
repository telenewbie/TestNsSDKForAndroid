
#include "ElevocLog.h"
#include <stdarg.h>

static int LogEnableValue = 1;
static const char *evtag = "ELEVOC_NS_FRONT";

int EVLogEnabled(void) {
    return LogEnableValue;
}

void EVLogSetEnabled(int enable) {
    LogEnableValue = enable;
}

void EVLogSetTag(const char *tag) {
    evtag = tag;
}

void LOGV(const char *fmt, ...) {
    if (LogEnableValue <= 0) {
        return;
    }
    va_list ap;
    va_start(ap, fmt);
#ifdef PLATFORM_ANDROID
    __android_log_vprint(ANDROID_LOG_VERBOSE, evtag, fmt, ap);
#else
    printf("%s: ", evtag);
    vprintf(fmt, ap);
    printf("\n");
#endif
    va_end(ap);
}
void LOGI(const char *fmt, ...) {
    if (LogEnableValue <= 0) {
        return;
    }
    va_list ap;
    va_start(ap, fmt);
#ifdef PLATFORM_ANDROID
    __android_log_vprint(ANDROID_LOG_INFO, evtag, fmt, ap);
#else
    printf("%s: ", evtag);
    vprintf(fmt, ap);
    printf("\n");
#endif
    va_end(ap);
}
void LOGD(const char *fmt, ...) {
    if (LogEnableValue <= 0) {
        return;
    }
    va_list ap;
    va_start(ap, fmt);
#ifdef PLATFORM_ANDROID
    __android_log_vprint(ANDROID_LOG_DEBUG, evtag, fmt, ap);
#else
    printf("%s: ", evtag);
    vprintf(fmt, ap);
    printf("\n");
#endif
    va_end(ap);
}
void LOGE(const char *fmt, ...) {
    if (LogEnableValue <= 0) {
        return;
    }
    va_list ap;
    va_start(ap, fmt);
#ifdef PLATFORM_ANDROID
    __android_log_vprint(ANDROID_LOG_ERROR, evtag, fmt, ap);
#else
    printf("%s: ", evtag);
    vprintf(fmt, ap);
    printf("\n");
#endif
    va_end(ap);
}
