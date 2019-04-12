//
// Created by user on 2019/4/4.
//

#ifndef LIBYUV_WRAPPER_YUV_JNI_H
#define LIBYUV_WRAPPER_YUV_JNI_H

#include <jni.h>

jint jni_nv21_to_argb(JNIEnv *env, jclass, jbyteArray nv21, jint width, jint height,
                      jbyteArray rgba);

jint jni_i420_to_argb(JNIEnv *env, jclass, jbyteArray i420, jint width, jint height,
                      jbyteArray rgba);

jint jni_argb_to_i420(JNIEnv *env, jclass, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv);

jint jni_argb_to_nv21(JNIEnv *env, jclass, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv);

jint jni_rgba_to_i420(JNIEnv *env, jclass, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv);

jint jni_rgba_to_nv21(JNIEnv *env, jclass, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv);

jint jni_i420_mirror(JNIEnv *env, jclass, jbyteArray src_arr, jint width, jint height,
                     jbyteArray dst_arr);

jint jni_i420_rotate_crop(JNIEnv *env, jclass,
                          jbyteArray src_i420_data,
                          jint src_width, jint src_height, jint degree,
                          jbyteArray dst_i420_data,
                          jint crop_x, jint crop_y, jint dst_width, jint dst_height);

jint jni_nv21_to_i420(JNIEnv *env, jclass, jbyteArray src_nv21_data, int width, int height,
                      jbyteArray dst_i420_data);

jint jni_i420_scale(JNIEnv *env, jclass, jbyteArray src_i420_data, jint width, jint height,
                    jbyteArray dst_i420_data, jint dst_width,
                    jint dst_height, jint mode);

jint jni_i420_to_nv21(JNIEnv *env, jclass, jbyteArray i420_data, jint width, jint height,
                      jbyteArray nv21_data);

jint jni_i420_rotate_crop_ex(JNIEnv *env, jclass,
                             jbyteArray src_i420_data,
                             jint src_width, jint src_height, jint rotation,
                             jbyteArray dst_i420_data,
                             jint dst_width, jint dst_height,
                             jboolean stretch, jint mode);
#endif //LIBYUV_WRAPPER_YUV_JNI_H
