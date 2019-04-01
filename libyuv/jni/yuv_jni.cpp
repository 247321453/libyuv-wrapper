//
// Created by user on 2019/3/28.
//

#include <libyuv.h>
#include "yuv_jni.h"
#include "surface_draw.h"
#include <android/native_window_jni.h>
#include <string>

int i420_rotate(jbyte *src_i420_data, int width, int height, jbyte *dst_i420_data, int degree) {
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + src_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + src_i420_y_size + src_i420_u_size;

    int ret;
    //要注意这里的width和height在旋转之后是相反的
    if (degree == libyuv::kRotate90 || degree == libyuv::kRotate270) {
        ret = libyuv::I420Rotate((const uint8_t *) src_i420_y_data, width,
                                 (const uint8_t *) src_i420_u_data, width >> 1,
                                 (const uint8_t *) src_i420_v_data, width >> 1,
                                 (uint8_t *) dst_i420_y_data, height,
                                 (uint8_t *) dst_i420_u_data, height >> 1,
                                 (uint8_t *) dst_i420_v_data, height >> 1,
                                 width, height,
                                 (libyuv::RotationMode) degree);
    } else {
        ret = 0;
    }
    return ret;
}

jint jni_i420_rotate(JNIEnv *env, jclass, jbyteArray src_i420_data, jint width, jint height,
                     jbyteArray dst_i420_data, jint degree) {
    jbyte *src = env->GetByteArrayElements(src_i420_data, NULL);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, NULL);

    int ret = i420_rotate(src, width, height, dst, degree);

    env->ReleaseByteArrayElements(src_i420_data, src, 0);
    env->ReleaseByteArrayElements(dst_i420_data, dst, 0);
    return ret;
}

int i420_mirror(jbyte *src_i420_data, int width, int height, jbyte *dst_i420_data) {
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);

    uint8_t *src_i420_y_data = (uint8_t *) src_i420_data;
    uint8_t *src_i420_u_data = (uint8_t *) src_i420_data + src_i420_y_size;
    uint8_t *src_i420_v_data = (uint8_t *) src_i420_data + src_i420_y_size + src_i420_u_size;

    uint8_t *dst_i420_y_data = (uint8_t *) dst_i420_data;
    uint8_t *dst_i420_u_data = (uint8_t *) dst_i420_data + src_i420_y_size;
    uint8_t *dst_i420_v_data = (uint8_t *) dst_i420_data + src_i420_y_size + src_i420_u_size;

    int ret = libyuv::I420Mirror(src_i420_y_data, width,
                                 src_i420_u_data, width >> 1,
                                 src_i420_v_data, width >> 1,
                                 dst_i420_y_data, width,
                                 dst_i420_u_data, width >> 1,
                                 dst_i420_v_data, width >> 1,
                                 width, height);
    return ret;
}

jint jni_i420_mirror(JNIEnv *env, jclass, jbyteArray src_i420_data, jint width, jint height,
                     jbyteArray dst_i420_data) {
    jbyte *src = env->GetByteArrayElements(src_i420_data, NULL);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, NULL);

    int ret = i420_mirror(src, width, height, dst);

    env->ReleaseByteArrayElements(src_i420_data, src, 0);
    env->ReleaseByteArrayElements(dst_i420_data, dst, 0);
    return ret;
}

int nv21_to_i420(jbyte *src_nv21_data, int width, int height, jbyte *src_i420_data) {
    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    uint8_t *src_nv21_y_data = (uint8_t *) src_nv21_data;
    uint8_t *src_nv21_vu_data = (uint8_t *) src_nv21_data + src_y_size;

    uint8_t *src_i420_y_data = (uint8_t *) src_i420_data;
    uint8_t *src_i420_u_data = (uint8_t *) src_i420_data + src_y_size;
    uint8_t *src_i420_v_data = (uint8_t *) src_i420_data + src_y_size + src_u_size;

    int ret = libyuv::NV21ToI420(
            src_nv21_y_data, width,
            src_nv21_vu_data, width,
            src_i420_y_data, width,
            src_i420_u_data, width >> 1,
            src_i420_v_data, width >> 1,
            width, height);
    return ret;
}

jint jni_nv21_to_i420(JNIEnv *env, jclass clazz, jbyteArray src_nv21_data, int width, int height,
                      jbyteArray dst_i420_data) {
    jbyte *src = env->GetByteArrayElements(src_nv21_data, NULL);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, NULL);

    int ret = nv21_to_i420(src, width, height, dst);

    env->ReleaseByteArrayElements(src_nv21_data, src, 0);
    env->ReleaseByteArrayElements(dst_i420_data, dst, 0);
    return ret;
}

jint jni_i420_crop(JNIEnv *env, jclass clazz, jbyteArray src_, jint width,
                   jint height, jbyteArray dst_, jint left, jint top, jint dst_width,
                   jint dst_height) {
    //裁剪的区域大小不对
    if (left + dst_width > width || top + dst_height > height) {
        return -2;
    }

    //left和top必须为偶数，否则显示会有问题
    if (left % 2 != 0 || top % 2 != 0) {
        return -1;
    }

    size_t src_length = (size_t) env->GetArrayLength(src_);
    jbyte *src_i420_data = env->GetByteArrayElements(src_, NULL);
    jbyte *dst_i420_data = env->GetByteArrayElements(dst_, NULL);

    jint dst_i420_y_size = dst_width * dst_height;
    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);

    uint8_t *src = (uint8_t *) src_i420_data;
    uint8_t *dst_i420_y_data = (uint8_t *) dst_i420_data;
    uint8_t *dst_i420_u_data = (uint8_t *) dst_i420_data + dst_i420_y_size;
    uint8_t *dst_i420_v_data = (uint8_t *) dst_i420_data + dst_i420_y_size + dst_i420_u_size;

    int ret = libyuv::ConvertToI420(
            src, src_length,
            dst_i420_y_data, dst_width,
            dst_i420_u_data, dst_width >> 1,
            dst_i420_v_data, dst_width >> 1,
            left, top,
            width, height,
            dst_width, dst_height,
            libyuv::kRotate0, libyuv::FOURCC_I420);

    env->ReleaseByteArrayElements(dst_, dst_i420_data, 0);
    return ret;
}

int i420_scale(jbyte *src_i420_data, int width, int height,
               jbyte *dst_i420_data, int dst_width, int dst_height, int mode) {

    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);

    uint8_t *src_i420_y_data = (uint8_t *) src_i420_data;
    uint8_t *src_i420_u_data = (uint8_t *) src_i420_data + src_i420_y_size;
    uint8_t *src_i420_v_data = (uint8_t *) src_i420_data + src_i420_y_size + src_i420_u_size;

    jint dst_i420_y_size = dst_width * dst_height;
    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);

    uint8_t *dst_i420_y_data = (uint8_t *) dst_i420_data;
    uint8_t *dst_i420_u_data = (uint8_t *) dst_i420_data + dst_i420_y_size;
    uint8_t *dst_i420_v_data = (uint8_t *) dst_i420_data + dst_i420_y_size + dst_i420_u_size;

    return libyuv::I420Scale(
            src_i420_y_data, width,
            src_i420_u_data, width >> 1,
            src_i420_v_data, width >> 1,
            width, height,
            dst_i420_y_data, dst_width,
            dst_i420_u_data, dst_width >> 1,
            dst_i420_v_data, dst_width >> 1,
            dst_width, dst_height,
            (libyuv::FilterMode) mode);
}

jint jni_i420_scale(JNIEnv *env, jclass clazz, jbyteArray src_i420_data, jint width, jint height,
                    jbyteArray dst_i420_data, jint dst_width,
                    jint dst_height, jint mode) {
    jbyte *src = env->GetByteArrayElements(src_i420_data, NULL);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, NULL);

    int ret = i420_scale(src, width, height, dst, dst_width, dst_height, mode);

    env->ReleaseByteArrayElements(src_i420_data, src, 0);
    env->ReleaseByteArrayElements(dst_i420_data, dst, 0);
    return ret;
}

int i420_to_nv21(jbyte *src, int width, int height, jbyte *dst) {

    int dst_stride_y = width, src_stride_y = width;
    int dst_stride_vu = width;
    int src_stride_u = width >> 1;
    int src_stride_v = src_stride_u;

    size_t y_size = static_cast<size_t>(src_stride_y * height);
    size_t u_size = static_cast<size_t>(src_stride_u * (height >> 1));

    uint8_t *src_y = (uint8_t *) src;
    uint8_t *src_u = (uint8_t *) src + y_size;
    uint8_t *src_v = (uint8_t *) src + y_size + u_size;

    uint8_t *dst_y = (uint8_t *) dst;
    uint8_t *dst_vu = (uint8_t *) dst + y_size;

    int ret = libyuv::I420ToNV21(
            src_y, src_stride_y,
            src_u, src_stride_u,
            src_v, src_stride_v,
            dst_y, dst_stride_y,
            dst_vu, dst_stride_vu,
            width, height);

    return ret;
}

jint jni_i420_to_nv21(JNIEnv *env, jclass clazz, jbyteArray i420_data, jint width, jint height,
                      jbyteArray nv21_data) {
    jbyte *src = env->GetByteArrayElements(i420_data, NULL);
    jbyte *dst = env->GetByteArrayElements(nv21_data, NULL);

    int ret = i420_to_nv21(src, width, height, dst);

    env->ReleaseByteArrayElements(i420_data, src, 0);
    env->ReleaseByteArrayElements(nv21_data, dst, 0);
    return ret;
}


void jni_compress_i420(JNIEnv *env, jclass clazz,
                       jbyteArray src_, jint width,
                       jint height, jbyteArray dst_,
                       jint dst_width, jint dst_height,
                       jint mode, jint degree,
                       jboolean isMirror) {
    //为中间操作需要的分配空间
    jbyte *Src_i420_data = (jbyte *) malloc(sizeof(jbyte) * width * height * 3 / 2);
    jbyte *Src_i420_data_scale = (jbyte *) malloc(sizeof(jbyte) * dst_width * dst_height * 3 / 2);
    jbyte *Src_i420_data_rotate = (jbyte *) malloc(sizeof(jbyte) * dst_width * dst_height * 3 / 2);

    jbyte *Src_data = env->GetByteArrayElements(src_, NULL);
    jbyte *Dst_data = env->GetByteArrayElements(dst_, NULL);

    nv21_to_i420(Src_data, width, height, Src_i420_data);

    if (width == dst_width && height == dst_height) {
        //
    } else {
        i420_scale(Src_i420_data, width, height, Src_i420_data_scale, dst_width, dst_height, mode);
    }
    if (isMirror) {
        i420_rotate(Src_i420_data_scale, dst_width, dst_height, Src_i420_data_rotate, degree);
        i420_mirror(Src_i420_data_rotate, dst_height, dst_width, Dst_data);
    } else {
        i420_rotate(Src_i420_data_scale, dst_width, dst_height, Dst_data, degree);
    }
    env->ReleaseByteArrayElements(dst_, Dst_data, 0);
    free(Src_i420_data);
    free(Src_i420_data_scale);
    free(Src_i420_data_rotate);
}

/***
 * byte数组转i420
 */
jint jni_argb_to_i420(JNIEnv *env, jclass clazz, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv) {
    int rgba_stride = 4 * width;
    int y_stride = width;
    int u_stride = width >> 1;
    int v_stride = u_stride;
    size_t ySize = (y_stride * height);
    size_t uSize = (u_stride * height >> 1);

    jbyte *rgbaData = env->GetByteArrayElements(rgba, NULL);
    jbyte *yuvData = env->GetByteArrayElements(yuv, NULL);
    int ret = libyuv::BGRAToI420((uint8_t *) rgbaData, rgba_stride,
                                 (uint8_t *) yuvData, y_stride,
                                 (uint8_t *) yuvData + ySize, u_stride,
                                 (uint8_t *) yuvData + ySize + uSize, v_stride,
                                 width, height);

    env->ReleaseByteArrayElements(rgba, rgbaData, 0);
    env->ReleaseByteArrayElements(yuv, yuvData, 0);

    return ret;
}

jint jni_argb_to_nv21(JNIEnv *env, jclass, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv) {
    int rgba_stride = 4 * width;
    int y_stride = width;
    int u_stride = width >> 1;
    int v_stride = u_stride;
    size_t ySize = static_cast<size_t>(y_stride * height);
    size_t uSize = static_cast<size_t>(u_stride * (height >> 1));

    jbyte *rgbaData = env->GetByteArrayElements(rgba, NULL);
    jbyte *nv21Data = env->GetByteArrayElements(yuv, NULL);

    jbyte *i420_data = (jbyte *) malloc(sizeof(jbyte) * width * height * 3 / 2);

    int ret = libyuv::BGRAToI420((uint8_t *) rgbaData, rgba_stride,
                                 (uint8_t *) i420_data, y_stride,
                                 (uint8_t *) i420_data + ySize, u_stride,
                                 (uint8_t *) i420_data + ySize + uSize, v_stride,
                                 width, height);
    if (ret != 0) {
        return ret;
    }
    ret = i420_to_nv21(i420_data, width, height, nv21Data);

    env->ReleaseByteArrayElements(rgba, rgbaData, 0);
    env->ReleaseByteArrayElements(yuv, nv21Data, 0);
    free(i420_data);
    return ret;
}

int rgba_draw_surface(JNIEnv *env, jobject surface, jbyte *rgba, int width, int height) {
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    if(nativeWindow == NULL){
        ALOGD("surface is null or invisible");
        return -1;
    }
    ANativeWindow_setBuffersGeometry(nativeWindow, width, height, WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer windowBuffer;
    // lock native window buffer
    ANativeWindow_lock(nativeWindow, &windowBuffer, 0);
    // 获取stride
    uint8_t *dst = (uint8_t *) windowBuffer.bits;
    uint8_t *src = (uint8_t *) rgba;
    size_t len = static_cast<size_t>(4 * width * height);
    memcpy(dst, src, len);
    ANativeWindow_unlockAndPost(nativeWindow);
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

int i420_draw_surface(JNIEnv *env, jobject surface, jbyte *i420Data, int width, int height) {
    jbyte *argbData = (jbyte *) malloc(sizeof(jbyte) * width * height * 4);
    /**
     * int I420ToBGRA(const uint8_t* src_y,
               int src_stride_y,
               const uint8_t* src_u,
               int src_stride_u,
               const uint8_t* src_v,
               int src_stride_v,
               uint8_t* dst_bgra,
               int dst_stride_bgra,
               int width,
               int height)
     */
    uint8_t *dst_y = (uint8_t *) i420Data;
    int dst_y_stride = width;
    int dst_u_stride = width >> 1;
    int dst_v_stride = dst_u_stride;
    size_t ySize = static_cast<size_t>(width * height);
    size_t uSize = static_cast<size_t>(dst_u_stride * (height >> 1));

    uint8_t *dst_u = (uint8_t *) i420Data + ySize;

    uint8_t *dst_v = (uint8_t *) i420Data + ySize + uSize;

    //rgba
    //abgr
    int ret = libyuv::I420ToABGR(
            dst_y, dst_y_stride,
            dst_u, dst_u_stride,
            dst_v, dst_v_stride,
            (uint8_t *) argbData,
            width * 4,
            width, height);
    if (ret != 0) {
        return ret;
    }
    ret = rgba_draw_surface(env, surface, argbData, width, height);
    free(argbData);
    return ret;
}

jint jni_i420_draw_surface(JNIEnv *env, jclass, jobject surface, jbyteArray i420, jint width,
                           jint height) {
    jbyte *i420Data = env->GetByteArrayElements(i420, NULL);
    int ret = i420_draw_surface(env, surface, i420Data, width, height);
    env->ReleaseByteArrayElements(i420, i420Data, 0);
    return ret;
}

jint jni_nv21_draw_surface(JNIEnv *env, jclass, jobject surface, jbyteArray nv21, jint width,
                           jint height) {
    jbyte *nv21Data = env->GetByteArrayElements(nv21, NULL);
    jbyte *i420Data = (jbyte *) malloc(sizeof(jbyte) * width * height * 3 / 2);
    int ret = nv21_to_i420(nv21Data, width, height, i420Data);
    env->ReleaseByteArrayElements(nv21, nv21Data, 0);
    if (ret != 0) {
        return ret;
    }
    ret = i420_draw_surface(env, surface, i420Data, width, height);
    free(i420Data);
    return ret;
}

jlong jni_create_drawer(JNIEnv *env, jclass, jobject surface) {
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (nativeWindow == NULL) {
        return 0;
    }
    drawer::SurfaceDrawer *surfaceDrawer = new drawer::SurfaceDrawer(nativeWindow);
    return (jlong) surfaceDrawer;
}

void jni_release_drawer(JNIEnv *env, jclass, jlong p_drawer){
    if(p_drawer != 0){
        drawer::SurfaceDrawer *drawer = (drawer::SurfaceDrawer *)p_drawer;
        drawer->Release();
        free(drawer);
    }
}
jint jni_drawer_draw_surface(JNIEnv *env, jclass, jlong p_drawer, jbyteArray rgba, jint width,
                                                       jint height) {
    if(p_drawer != 0){
        drawer::SurfaceDrawer *drawer = (drawer::SurfaceDrawer *)p_drawer;
        jbyte *srcData = env->GetByteArrayElements(rgba, NULL);
        int ret = drawer->DrawSurface(srcData, width, height);
        env->ReleaseByteArrayElements(rgba, srcData, 0);
        return ret;
    }
    return -1;
}

/*
boolean	Z
long	J
byte	B
float	F
char	C
double	D
short	S
类	L全限定类名;
int	I
数组	[元素类型签名
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *_vm, void *) {
    JNIEnv *env;
    _vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
    jclass nativeEngineClass = (jclass) env->NewGlobalRef(env->FindClass(JNI_CLASS_NAME));
    static JNINativeMethod methods[] = {
            {"argbToI420",      "([BII[B)I",                     (void *) jni_argb_to_i420},
            {"argbToNv21",      "([BII[B)I",                     (void *) jni_argb_to_nv21},
            {"i420ToNv21",      "([BII[B)I",                     (void *) jni_i420_to_nv21},
            {"nv21ToI420",      "([BII[B)I",                     (void *) jni_nv21_to_i420},
            {"i420Scale",       "([BII[BIII)I",                  (void *) jni_i420_scale},
            {"i420Crop",        "([BII[BIIII)I",                 (void *) jni_i420_crop},
            {"i420Mirror",      "([BII[B)I",                     (void *) jni_i420_mirror},
            {"i420Rotate",      "([BII[BI)I",                    (void *) jni_i420_rotate},
            {"i420Compress",    "([BII[BIIIIZ)V",                (void *) jni_compress_i420},

            {"i420DrawSurface", "(Landroid/view/Surface;[BII)I", (void *) jni_i420_draw_surface},
            {"nv21DrawSurface", "(Landroid/view/Surface;[BII)I", (void *) jni_nv21_draw_surface},
            {"rgbaDrawSurface", "(Landroid/view/Surface;[BII)I", (void *) jni_rgba_draw_surface},

            {"createDrawer",    "(Landroid/view/Surface;)J",     (void *) jni_create_drawer},
            {"releaseDrawer",   "(J)V",                          (void *) jni_release_drawer},
            {"doDrawer",        "(J[BII)I",                      (void *) jni_drawer_draw_surface},
    };

    if (env->RegisterNatives(nativeEngineClass, methods, 15) < 0) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}

//JNIEXPORT void JNI_OnUnload(JavaVM *jvm, void *reserved) {
//
//}