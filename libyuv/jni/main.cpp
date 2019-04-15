//
// Created by user on 2019/4/4.
//
#include <jni.h>
#include "yuv_jni.h"
#include "yuv_draw.h"
#define JNI_CLASS_NAME "org/google/chromium/YuvJni"


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
            {"i420DrawSurface",    "(Landroid/view/Surface;[BII)I", (void *) jni_i420_draw_surface},
            {"nv21DrawSurface",    "(Landroid/view/Surface;[BII)I", (void *) jni_nv21_draw_surface},
            {"rgbaDrawSurface",    "(Landroid/view/Surface;[BII)I", (void *) jni_rgba_draw_surface},

            {"i420ToArgb",         "([BII[B)I",                     (void *) jni_i420_to_argb},
            {"nv21ToArgb",         "([BII[B)I",                     (void *) jni_nv21_to_argb},
            {"argbToI420",         "([BII[B)I",                     (void *) jni_argb_to_i420},

            {"argbToNv21",         "([BII[B)I",                     (void *) jni_argb_to_nv21},
            {"i420ToNv21",         "([BII[B)I",                     (void *) jni_i420_to_nv21},
            {"i420Mirror",         "([BII[B)I",                     (void *) jni_i420_mirror},

            {"i420RotateWithCrop", "([BIII[BIIII)I",                (void *) jni_i420_rotate_crop},
            {"nv21ToI420",         "([BII[B)I",                     (void *) jni_nv21_to_i420},
            {"i420Scale",          "([BII[BIII)I",                  (void *) jni_i420_scale},

            {"i420RotateWithCropEx", "([BIII[BIIZI)I",                (void *) jni_i420_rotate_crop_ex},
    };

    if (env->RegisterNatives(nativeEngineClass, methods, 13) < 0) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}

//JNIEXPORT void JNI_OnUnload(JavaVM *jvm, void *reserved) {
//
//}