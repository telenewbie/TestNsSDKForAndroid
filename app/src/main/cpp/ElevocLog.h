
#ifndef Log_h
#define Log_h

#ifdef PLATFORM_ANDROID
#include <android/log.h>
#include <jni.h>
#else
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

int EVLogEnabled(void);
void EVLogSetEnabled(int enable);
void EVLogSetTag(const char *tag);

void LOGV(const char *fmt, ...);
void LOGI(const char *fmt, ...);
void LOGD(const char *fmt, ...);
void LOGE(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* Log_h */
