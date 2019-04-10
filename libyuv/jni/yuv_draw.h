//
// Created by user on 2019/4/8.
//

#ifndef LIBYUV_WRAPPER_DRAW_SURFACE_H
#define LIBYUV_WRAPPER_DRAW_SURFACE_H

#include <jni.h>

int rgba_draw_surface(JNIEnv *env, jobject surface, jbyte *rgba, int width, int height);

jint jni_rgba_draw_surface(JNIEnv *env, jclass, jobject surface, jbyteArray rgba, jint width,
                           jint height);

jint jni_i420_draw_surface(JNIEnv *env, jclass, jobject surface, jbyteArray i420, jint width,
                           jint height);

jint jni_nv21_draw_surface(JNIEnv *env, jclass, jobject surface, jbyteArray nv21, jint width,
                           jint height);


#endif //LIBYUV_WRAPPER_DRAW_SURFACE_H
