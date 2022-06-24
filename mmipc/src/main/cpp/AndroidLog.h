//
// Created by didi on 2022/6/23.
//
#include <android/log.h>

#ifndef MMIPC_ANDROIDLOG_H
#define MMIPC_ANDROIDLOG_H

#define LOG_TAG "MMIPC->"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)

#endif //MMIPC_ANDROIDLOG_H
