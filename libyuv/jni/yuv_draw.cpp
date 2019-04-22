//
// Created by user on 2019/4/8.
//

#include "yuv_draw.h"
#include <android/native_window_jni.h>
#include <string>
#include "debug.h"
#include "yuv_util.h"

int rgba_draw_surface(JNIEnv *env, jobject surface, jbyte *rgba, int width, int height) {
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (nativeWindow == NULL) {
        ALOGD("surface is null or invisible");
        return -1;
    }
    ANativeWindow_setBuffersGeometry(nativeWindow, width, height, WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer windowBuffer;
    if (ANativeWindow_lock(nativeWindow, &windowBuffer, 0) == 0) {
        uint8_t *dst = (uint8_t *) windowBuffer.bits;
        int dstStride;
        if (windowBuffer.format == WINDOW_FORMAT_RGBA_8888) {
            dstStride = windowBuffer.stride * 4;
        } else {
            dstStride = windowBuffer.stride * 2;
        }
        uint8_t *src = (uint8_t *) rgba;
        size_t src_stride = (size_t) width * 4;
        for (int h = 0; h < height; h++) {
            memcpy(dst + h * dstStride, src + h * src_stride, src_stride);
        }
        ANativeWindow_unlockAndPost(nativeWindow);
    }
    ANativeWindow_release(nativeWindow);
    return 0;
}
jint jni_rgba_draw_surface(JNIEnv *env, jclass, jobject surface, jbyteArray rgba, jint width,
                           jint height) {
    jbyte *srcData = env->GetByteArrayElements(rgba, NULL);
    int ret = rgba_draw_surface(env, surface, srcData, width, height);
    env->ReleaseByteArrayElements(rgba, srcData, 0);
    return ret;
}

jint jni_i420_draw_surface(JNIEnv *env, jclass, jobject surface, jbyteArray i420, jint width,
                           jint height) {
    jbyte *i420Data = env->GetByteArrayElements(i420, NULL);
    jbyte *rgbaData = (jbyte *) malloc(sizeof(jbyte) * width * height * 4);
    int ret = i420_to_rgba((uint8_t *) i420Data, width, height, (uint8_t *) rgbaData);
    env->ReleaseByteArrayElements(i420, i420Data, 0);
    if (ret == 0) {
        ret = rgba_draw_surface(env, surface, rgbaData, width, height);
    }
    return ret;
}

jint jni_nv21_draw_surface(JNIEnv *env, jclass, jobject surface, jbyteArray nv21, jint width,
                           jint height) {
    jbyte *src = env->GetByteArrayElements(nv21, NULL);
    jbyte *rgbaData = (jbyte *) malloc(sizeof(jbyte) * width * height * 4);
    int ret = nv21_to_rgba((uint8_t *) src, width, height, (uint8_t *) rgbaData);
    env->ReleaseByteArrayElements(nv21, src, 0);
    if (ret == 0) {
        ret = rgba_draw_surface(env, surface, rgbaData, width, height);
        free(rgbaData);
    }
    return ret;
}
