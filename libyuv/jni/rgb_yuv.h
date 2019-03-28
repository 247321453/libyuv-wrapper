//
// Created by user on 2019/3/28.
//

#ifndef LIBYUV_WRAPPER_RGB_YUV_H
#define LIBYUV_WRAPPER_RGB_YUV_H

#include "debug.h"
#include <jni.h>
#include "libyuv.h"

static int (*i420Func[])(const uint8_t *,int,uint8_t *,int,uint8_t *,int ,uint8_t *,int,int,int)={
        //4
        libyuv::ABGRToI420,libyuv::RGBAToI420,libyuv::ARGBToI420,libyuv::BGRAToI420,
        //3
        libyuv::RGB24ToI420,libyuv::RGB565ToI420
};

/**
    int RGBA = 0x01001040;
    int ABGR = 0x01001041;
    int BGRA = 0x01001042;
    int ARGB = 0x01001043;

    int RGB24 = 0x01001034;
    int RGB565 = 0x01001025;
 */
jint jni_convert_i420(JNIEnv *env, jclass clazz, jint type, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv);
jint jni_convert_i420_i(JNIEnv *env, jclass clazz, jint type, jintArray rgba, jint width, jint height,
                        jbyteArray yuv);

#endif //LIBYUV_WRAPPER_RGB_YUV_H
