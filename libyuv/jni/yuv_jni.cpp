//
// Created by user on 2019/3/28.
//
#include <string>
#include "debug.h"
#include "yuv_jni.h"
#include "yuv_util.h"
#include "size_info.h"

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

jint jni_rgba_to_i420(JNIEnv *env, jclass, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv){
    jbyte *rgbaData = env->GetByteArrayElements(rgba, NULL);
    jbyte *yuvData = env->GetByteArrayElements(yuv, NULL);
    int ret = rgba_to_i420((uint8_t*)rgbaData, width, height, (uint8_t*)yuvData);
    env->ReleaseByteArrayElements(rgba, rgbaData, 0);
    env->ReleaseByteArrayElements(yuv, yuvData, 0);
    return ret;
}

jint jni_rgba_to_nv21(JNIEnv *env, jclass, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv){
    jbyte *rgbaData = env->GetByteArrayElements(rgba, NULL);
    jbyte *nv21Data = env->GetByteArrayElements(yuv, NULL);

    jbyte *i420_data = (jbyte *) malloc(sizeof(jbyte) * width * height * 3 / 2);
    int ret = rgba_to_i420((uint8_t*)rgbaData, width, height, (uint8_t*)i420_data);
    if(ret == 0){
        ret = i420_to_nv21((uint8_t*)i420_data, width, height, (uint8_t*)nv21Data);
    }
    env->ReleaseByteArrayElements(rgba, rgbaData, 0);
    env->ReleaseByteArrayElements(yuv, nv21Data, 0);
    free(i420_data);
    return ret;
}

jint jni_argb_to_i420(JNIEnv *env, jclass, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv) {
    jbyte *rgbaData = env->GetByteArrayElements(rgba, NULL);
    jbyte *yuvData = env->GetByteArrayElements(yuv, NULL);
    int ret = rgba_to_i420((uint8_t*)rgbaData, width, height, (uint8_t*)yuvData);
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
                          jint src_width, jint src_height, jint rotation,
                          jbyteArray dst_i420_data,
                          jint crop_x, jint crop_y, jint dst_width, jint dst_height) {
    jbyte *src = env->GetByteArrayElements(src_i420_data, NULL);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, NULL);
    int ret = i420_rotate_crop((uint8_t *) src, src_width, src_height, rotation,
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

jint jni_i420_rotate_crop_ex(JNIEnv *env, jclass,
                             jbyteArray src_i420_data,
                             jint src_width, jint src_height, jint rotation,
                             jbyteArray dst_i420_data,
                             jint dst_width, jint dst_height,
                             jboolean stretch, jint mode) {
    jbyte *src = env->GetByteArrayElements(src_i420_data, NULL);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, NULL);
    SizeInfo info;
    info.src_width = src_width;
    info.src_height = src_height;
    info.src_rotation = 0;
    InitSizeInfo(&info, dst_width, dst_height, rotation, stretch, mode);

    int ret;
    int crop_x = info.crop_x, crop_y = info.crop_y, crop_w = info.crop_width, crop_h = info.crop_height;
    if (info.need_scale) {
        uint8_t *r_data = new uint8_t[crop_w * crop_h * 3 / 2];
        ret = i420_rotate_crop((uint8_t *) src, src_width, src_height, info.display_rotation,
                               crop_x, crop_y, crop_w, crop_h, r_data);
        if (ret == 0) {
            if (info.display_rotation == ROTATION_90 || info.display_rotation == ROTATION_270) {
                ret = i420_scale(r_data, crop_h, crop_w, (uint8_t *) dst, info.display_width,
                                 info.display_height, 3);
            } else {
                ret = i420_scale(r_data, crop_x, crop_h, (uint8_t *) dst, info.display_width,
                                 info.display_height, 3);
            }
        }
        free(r_data);
    } else {
        ret = i420_rotate_crop((uint8_t *) src, src_width, src_height, info.display_rotation,
                               crop_x, crop_y, crop_w, crop_h, (uint8_t *) dst);
    }
    env->ReleaseByteArrayElements(src_i420_data, src, 0);
    env->ReleaseByteArrayElements(dst_i420_data, dst, 0);
    return ret;
}
