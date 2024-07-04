//
// Created by elevoc on 2018/12/20.
//

#ifndef ELEVOCNSFORROM_ELEVOCTESTCENTER_H
#define ELEVOCNSFORROM_ELEVOCTESTCENTER_H

#include <string>
#include <vector>
#include <jni.h>

class ElevocTestCenter {

public:
    FILE *pcm_write;
    FILE *kws_pcm_write;
    std::string appid;
    std::string appkey;

    JavaVM *jvm;

    ElevocTestCenter(const char *appid, const char *appkey, const char *dirPath, JavaVM *jvm);
    ~ElevocTestCenter();

    void search_and_sort_file(std::string path, std::vector<std::string> &fileNames);
    void start_process();

};

#endif //ELEVOCNSFORROM_ELEVOCTESTCENTER_H
