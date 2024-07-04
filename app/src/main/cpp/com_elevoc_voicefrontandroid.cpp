//
// Created by elevoc on 2018/12/20.
//

#include <unistd.h>
#include "com_elevoc_voicefrontandroid_MainActivity.h"
#include "ElevocLog.h"
#include "ElevocTestCenter.h"

JNIEXPORT void JNICALL Java_com_elevoc_voicefrontandroid_MainActivity_start_1process
        (JNIEnv *env, jobject, jstring appid_, jstring appkey_, jstring pcmpath_){

    const char *appid = env->GetStringUTFChars(appid_, 0);
    const char *appkey = env->GetStringUTFChars(appkey_, 0);
    const char *pcm_path = env->GetStringUTFChars(pcmpath_, 0);


    JavaVM *gs_jvm;
    env->GetJavaVM(&gs_jvm);

    ElevocTestCenter *testCenter = new ElevocTestCenter(appid, appkey, pcm_path, gs_jvm);
    testCenter->start_process();
    delete testCenter;

    env->ReleaseStringUTFChars(appid_, appid);
    env->ReleaseStringUTFChars(appkey_, appkey);
    env->ReleaseStringUTFChars(pcmpath_, pcm_path);
 }