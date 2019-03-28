//
// Created by user on 2019/3/28.
//

#include "yuv_jni.h"
#include "yuv.h"
#include "rgb_yuv.h"

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
            {"convertToi420",  "(I[BII[B)I",     (void *) jni_convert_i420},
            {"convertToi420I", "(I[III[B)I",     (void *) jni_convert_i420_i},
            {"i420ToNv21",     "([BII[B)I",      (void *) jni_i420_to_nv21},
            {"nv21ToI420",     "([BII[B)I",      (void *) jni_nv21_to_i420},
            {"i420Scale",      "([BII[BIII)I",   (void *) jni_i420_scale},
            {"i420Crop",       "([BII[BIIII)I",  (void *) jni_i420_crop},
            {"i420Mirror",     "([BII[B)I",      (void *) jni_i420_mirror},
            {"i420Rotate",     "([BII[BI)I",     (void *) jni_i420_rotate},
            {"i420Compress",   "([BII[BIIIIZ)V", (void *) jni_compress_i420},
    };
    int len = 9;
    if (env->RegisterNatives(nativeEngineClass, methods, len) < 0) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM *jvm, void *reserved) {

}