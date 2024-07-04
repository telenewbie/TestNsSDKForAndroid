//
// Created by elevoc on 2018/12/20.
//

#include "ElevocTestCenter.h"
#include "IEVProcessTask.h"
#include "ElevocLog.h"
#include <vector>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <time.h>

using namespace std;

#define ON_LINE

std::string dirPath;

void GetALLFormatFiles(std::string path, std::vector<std::string>&fileNames, std::string format);
bool comp(std::string &a, std::string &b);
//void handleErrorCallback(ProcessErrorType errorType, const char* errorDesc);
//void oauthSuccessCallback(const char *);
//void detectHowlingCallBack(bool isHowling);
void startDenoise(IEVProcessTask *evtask, std::string originDir,  std::string filename, int samplerate, int channels);
//void startDenoiseWithAEC(IEVProcessTask *evtask, std::string originDir,  std::string aec_near_name, std::string
//aec_far_name, std::string aec_out_name, int samplerate, int channels);

static void response(const char* msg){
    LOGD("response >>>> [%s]",msg);
}

ElevocTestCenter::ElevocTestCenter(const char *appid, const char *appkey, const char *path, JavaVM *jvm){

    this->appid = appid;
    this->appkey = appkey;
    dirPath = path;
    this->jvm = jvm;
}

ElevocTestCenter::~ElevocTestCenter() {

}

void ElevocTestCenter::start_process() {

 //   vector<string> fileName;
 //   search_and_sort_file(dirPath, fileName);
    srand((unsigned int)time(NULL));

    for (size_t i = 0; i < 1; i++) {
        LOGD("start ... %d", i);
        IEVProcessTask * evtask = evNewTaskWithJavaVM(this->appid.c_str() , this->appkey.c_str(),dirPath.data(), this->jvm);
        evtask->setAuthCallback(response);
        evtask->setNetworkLicenseSaveDir(dirPath.data());
//        evtask->setSampleRate(16000);
        const char *ver = evtask->libVer();
        LOGD("ver: %s", ver);
//        evtask->setSceneMode(VOIP);
        sleep(5);
        LOGI("ver: %s, %p", evtask->libVer(), evtask);
//        startDenoise(evtask, dirPath, "recording_8000.pcm", 8000, 1);
//        startDenoiseWithAEC(evtask, dirPath, "aecNear.pcm", "aecFar.pcm", "aecOut_8k5.pcm", 8000, 1);
        int sample_rate = 48000;
        int channels = 2;
        evtask->setSampleRate(sample_rate);
        evtask->setChannels(channels,0);
        startDenoise(evtask, dirPath, "48k_c2_10s.pcm", sample_rate, channels);
//        startDenoiseWithAEC(evtask, dirPath, "elevoc_audio_input_2023-12-29-10-35-51_8000_1.pcm", "elevoc_far_input.pcm", "aecOut.pcm", 16000, 1);



        evDeleteTask(evtask);
        LOGD("done ... %d!", i);
    }
}

void startDenoise(IEVProcessTask *const evtask, std::string originDir,  std::string filename, int samplerate, int channels)
{

    static int cnt = 0;
    string resultDir = originDir;

    stringstream sstr;
    ++cnt;
    sstr << cnt;
    string str = sstr.str();

    int wavLen = 0;

    string path = originDir + filename;
    string currentHandleFileName = resultDir + "/" + str + "ns_" + filename;

    FILE *rfp = fopen(path.c_str(), "rb");
    if (rfp != NULL) {
        if (!fseek(rfp, 0, SEEK_END)) {
            wavLen = ftell(rfp);
            fseek(rfp, 0, SEEK_SET);
        }
    }else {
        LOGE("%s , %s, open failed.", filename.c_str(), path.c_str());
        return;
    }

    if (filename.find(".wav") != std::string::npos) {
        wavLen -= 44;
        fseek(rfp, 44, SEEK_SET);
    }

    LOGD("filesize : %d", wavLen);

    FILE *wfp = fopen(currentHandleFileName.c_str(), "wb");
    if (wfp == NULL) {
        LOGE("%s open failed.", currentHandleFileName.c_str());
    }

    LOGD("--------------------------------------");
    LOGD("begin process %s", filename.c_str());

    int onceReadLen = (samplerate / 100) * channels * sizeof(short);
    int readLen = 0;
    bool isLast = false;
    char wavData[8192 * 2];
    size_t freadLen;
    size_t len;
    evtask->prepared();
    evtask->setParam(0x100,1.5);
    evtask->startTask();
    LOGD("jni process start!");

    while((readLen = fread(wavData, 1, onceReadLen, rfp))>0){
        evtask->writeAudio(wavData, readLen);
        usleep(10000);
        len = evtask->readBuffer(wavData, readLen, &isLast, false);
        if (len > 0) {
            fwrite(wavData, 1, len, wfp);
        }else{
            LOGE("readBuffer error ! %d,readLen = %d",len,readLen);
        }
    }

    LOGD("jni process over!");

#ifdef ON_LINE
    evtask->stopTask(true);
#else
    evtask->stopTask(false);
#endif

#ifndef ON_LINE
    // waiting for handle finish
    while (!isLast) {
//        int t = rand() % 8192 + 100;
//        t = ((t % 2) == 0) ? t : t + 1;
//        onceReadLen = t;
        len = evtask->readBuffer(wavData, onceReadLen, &isLast, false);
        if (len > 0) {
            fwrite(wavData, 1, len, wfp);
        }

        if (isLast) {
            LOGD("isLast %d", isLast);
            break;
        }
//        usleep(10000);
    }
#endif
    fclose(rfp);
    fclose(wfp);

    //int howlingTime = 0, unHowlingTime = 0;

    //evtask->getHowlingState(&howlingTime, &unHowlingTime);

    //LOGD("howlingTime : %d, unHowlingTime : %d", howlingTime, unHowlingTime);

    LOGD("%s process done!", filename.c_str());
    LOGD("--------------------------------------");
}

bool comp(std::string &a, std::string &b) {
    return a.length() < b.length();
}

void ElevocTestCenter::search_and_sort_file(std::string path, std::vector<std::string> &fileNames) {
    string format = ".wav";
    GetALLFormatFiles(path, fileNames, format);
    format = ".pcm";
    GetALLFormatFiles(path, fileNames, format);

    std::sort(fileNames.begin(), fileNames.end(), comp);

    vector<string>::iterator it = fileNames.begin();

    LOGD("***************** wav/pcm file list **************");
    for (; it != fileNames.end(); it++)
    {
        std::cout << *it << std::endl;
    }

    int size = fileNames.size();

    LOGD("total: %d", size);
    LOGD("***********************************************");
}

void GetALLFormatFiles(std::string path, std::vector<std::string>&fileNames, std::string format)
{
    //目录
    DIR *dp;
    //获取dir目录具体的文件信息，名字，长度，指针地址等

    struct dirent *dirp;

    if ((dp = opendir(path.c_str())) == NULL)
    {
        perror("opendir");
    }
    int strL = 0;
    std::string p;
    while ((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0 || strncmp(dirp->d_name, ".", 1) == 0)
            continue;

        int size = strlen(dirp->d_name);
        int formatSize = strlen(format.c_str());
        if(strcmp(( dirp->d_name + (size - formatSize)), format.c_str()) != 0)
            continue;

        //fileVec.
        fileNames.push_back(p.assign(dirp->d_name));
    }
    closedir(dp);
}
