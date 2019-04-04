//
// Created by user on 2019/3/28.
//
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
        int dstStride = windowBuffer.stride * 4;
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

jint jni_nv21_to_argb(JNIEnv *env, jclass, jbyteArray nv21, jint width, jint height,
                      jbyteArray rgba) {
    jbyte *src = env->GetByteArrayElements(nv21, NULL);
    jbyte *dst = env->GetByteArrayElements(rgba, NULL);
    jbyte *i420 = (jbyte *) malloc(sizeof(jbyte) * width * height * 3 / 2);
    int ret = nv21_to_i420((uint8_t *) src, width, height, (uint8_t *) i420);
    if (ret == 0) {
        ret = i420_to_argb((uint8_t *) i420, width, height, (uint8_t *)dst);
    }
    env->ReleaseByteArrayElements(nv21, src, 0);
    env->ReleaseByteArrayElements(rgba, dst, 0);
    free(i420);
    return ret;
}

jint jni_i420_to_argb(JNIEnv *env, jclass, jbyteArray i420, jint width, jint height,
                      jbyteArray rgba) {
    jbyte *src = env->GetByteArrayElements(i420, NULL);
    jbyte *dst = env->GetByteArrayElements(rgba, NULL);

    int ret = i420_to_argb((uint8_t *) src, width, height, (uint8_t *) dst);

    env->ReleaseByteArrayElements(i420, src, 0);
    env->ReleaseByteArrayElements(rgba, dst, 0);
    return ret;
}

jint jni_argb_to_i420(JNIEnv *env, jclass, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv) {
    jbyte *rgbaData = env->GetByteArrayElements(rgba, NULL);
    jbyte *yuvData = env->GetByteArrayElements(yuv, NULL);
    int ret = argb_to_i420((uint8_t*)rgbaData, width, height, (uint8_t*)yuvData);
    env->ReleaseByteArrayElements(rgba, rgbaData, 0);
    env->ReleaseByteArrayElements(yuv, yuvData, 0);
    return ret;
}

jint jni_argb_to_nv21(JNIEnv *env, jclass, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv) {
    jbyte *rgbaData = env->GetByteArrayElements(rgba, NULL);
    jbyte *nv21Data = env->GetByteArrayElements(yuv, NULL);

    jbyte *i420_data = (jbyte *) malloc(sizeof(jbyte) * width * height * 3 / 2);
    int ret = argb_to_i420((uint8_t*)rgbaData, width, height, (uint8_t*)i420_data);
    if(ret == 0){
        ret = i420_to_nv21((uint8_t*)i420_data, width, height, (uint8_t*)nv21Data);
    }
    env->ReleaseByteArrayElements(rgba, rgbaData, 0);
    env->ReleaseByteArrayElements(yuv, nv21Data, 0);
    free(i420_data);
    return ret;
}

jint jni_i420_to_nv21(JNIEnv *env, jclass, jbyteArray i420_data, jint width, jint height,
                      jbyteArray nv21_data) {
    jbyte *src = env->GetByteArrayElements(i420_data, NULL);
    jbyte *dst = env->GetByteArrayElements(nv21_data, NULL);

    int ret = i420_to_nv21((uint8_t *) src, width, height, (uint8_t *) dst);

    env->ReleaseByteArrayElements(i420_data, src, 0);
    env->ReleaseByteArrayElements(nv21_data, dst, 0);
    return ret;
}


jint jni_i420_mirror(JNIEnv *env, jclass, jbyteArray src_arr, jint width, jint height,
                     jbyteArray dst_arr) {
    jbyte *src = env->GetByteArrayElements(src_arr, NULL);
    jbyte *dst = env->GetByteArrayElements(dst_arr, NULL);

    int ret = i420_mirror((uint8_t *) src, width, height, (uint8_t *) dst);

    env->ReleaseByteArrayElements(src_arr, src, 0);
    env->ReleaseByteArrayElements(dst_arr, dst, 0);
    return ret;
}

jint jni_i420_rotate_crop(JNIEnv *env, jclass,
                          jbyteArray src_i420_data,
                          jint src_width, jint src_height, jint degree,
                          jbyteArray dst_i420_data,
                          jint crop_x, jint crop_y, jint dst_width, jint dst_height) {
    jbyte *src = env->GetByteArrayElements(src_i420_data, NULL);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, NULL);

    if (degree > 4) {
        degree = degree / 90;
    }
    int ret = i420_rotate_crop((uint8_t *) src, src_width, src_height, degree,
                               crop_x, crop_y, dst_width, dst_height, (uint8_t *) dst);
    env->ReleaseByteArrayElements(src_i420_data, src, 0);
    env->ReleaseByteArrayElements(dst_i420_data, dst, 0);
    return ret;
}

jint jni_nv21_to_i420(JNIEnv *env, jclass, jbyteArray src_nv21_data, int width, int height,
                      jbyteArray dst_i420_data) {
    jbyte *src = env->GetByteArrayElements(src_nv21_data, NULL);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, NULL);

    int ret = nv21_to_i420((uint8_t *) src, width, height, (uint8_t *) dst);

    env->ReleaseByteArrayElements(src_nv21_data, src, 0);
    env->ReleaseByteArrayElements(dst_i420_data, dst, 0);
    return ret;
}

jint jni_i420_scale(JNIEnv *env, jclass, jbyteArray src_i420_data, jint width, jint height,
                    jbyteArray dst_i420_data, jint dst_width,
                    jint dst_height, jint mode) {
    jbyte *src = env->GetByteArrayElements(src_i420_data, NULL);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, NULL);

    int ret = i420_scale((uint8_t *) src, width, height, (uint8_t *) dst, dst_width, dst_height,
                         mode % 4);

    env->ReleaseByteArrayElements(src_i420_data, src, 0);
    env->ReleaseByteArrayElements(dst_i420_data, dst, 0);
    return ret;
}
