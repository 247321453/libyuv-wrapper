//
// Created by user on 2019/3/28.
//

#ifndef LIBYUV_WRAPPER_YUV_JNI_H
#define LIBYUV_WRAPPER_YUV_JNI_H

#include <jni.h>
#include <android/log.h>

#define LOG_TAG "libyuv"

#if defined(DISABLE_LOG)
#define ALOGI(...) NULL
#define ALOGD(...) NULL
#define ALOGW(...) NULL
#define ALOGE(...) NULL
#else
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG ,__VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG ,__VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG ,__VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG ,__VA_ARGS__)
#endif

#define JNI_CLASS_NAME "org/google/chromium/YuvJni"

#endif //LIBYUV_WRAPPER_YUV_JNI_H
