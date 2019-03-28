//
// Created by user on 2019/3/28.
//

#ifndef LIBYUV_WRAPPER_YUV_H
#define LIBYUV_WRAPPER_YUV_H

#include "debug.h"
#include <jni.h>
#include "libyuv.h"

void jni_compress_i420(JNIEnv *env, jclass type,
                     jbyteArray src_, jint width,
                     jint height, jbyteArray dst_,
                     jint dst_width, jint dst_height,
                     jint mode, jint degree,
                     jboolean isMirror);

jint jni_i420_rotate(JNIEnv *env, jclass clazz, jbyteArray src_i420_data, jint width, jint height,
                     jbyteArray dst_i420_data, jint degree);

jint jni_i420_mirror(JNIEnv *env, jclass clazz, jbyteArray src_i420_data, jint width, jint height,
                     jbyteArray dst_i420_data);

jint jni_nv21_to_i420(JNIEnv *env, jclass clazz, jbyteArray src_nv21_data, int width, int height,
                      jbyteArray dst_i420_data);

jint jni_i420_crop(JNIEnv *env, jclass clazz, jbyteArray src_, jint width,
                   jint height, jbyteArray dst_, jint dst_width, jint dst_height,
                   jint left, jint top);

jint jni_i420_scale(JNIEnv *env, jclass clazz, jbyteArray src_i420_data, jint width, jint height,
                    jbyteArray dst_i420_data, jint dst_width,
                    jint dst_height, jint mode);

jint
jni_i420_to_nv21(JNIEnv *env, jclass clazz, jbyteArray i420_data, jint width, jint height, jbyteArray nv21_data);


#endif //LIBYUV_WRAPPER_YUV_H
