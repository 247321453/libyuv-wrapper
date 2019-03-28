#include "rgb_yuv.h"
/**
 * rgb转i420
 */
int convert_i420(jbyte *src, jint width, jint height,
                 jint src_stride, jbyte *dst,
                 int (*func)(const uint8_t *, int, uint8_t *, int, uint8_t *, int, uint8_t *, int,
                             int, int)) {
    int dst_stride_y = width;
    int dst_stride_u = width >> 1;
    int dst_stride_v = dst_stride_u;

    size_t y_size = (size_t) (dst_stride_y * height);
    size_t u_size = (size_t) (dst_stride_u * (height >> 1));

    uint8_t *src_rgb = (uint8_t *) src;
    uint8_t *dst_y = (uint8_t *) dst;
    uint8_t *dst_u = (uint8_t *) (dst + +y_size);
    uint8_t *dst_v = (uint8_t *) (dst + y_size + u_size);

    int ret = func(
            src_rgb, src_stride,
            dst_y, dst_stride_y,
            dst_u, dst_stride_u,
            dst_v, dst_stride_v,
            width, height);

    return ret;
}

/***
 * byte数组转i420
 */
jint jni_convert_i420(JNIEnv *env, jclass clazz, jint type, jbyteArray rgba, jint width, jint height,
                      jbyteArray yuv) {
    uint8_t convert_type = (uint8_t) (type & 0x0F);
    int rgba_stride = ((type & 0xF0) >> 4) * width;
    jbyte *src = env->GetByteArrayElements(rgba, NULL);
    jbyte *dst = env->GetByteArrayElements(yuv, NULL);
    int ret = convert_i420(src, rgba_stride, width, height, dst, i420Func[convert_type]);
    env->ReleaseByteArrayElements(rgba, src, 0);
    env->ReleaseByteArrayElements(yuv, dst, 0);
    return ret;
}

/***
 * int数组转i420
 */
jint jni_convert_i420_i(JNIEnv *env, jclass clazz, jint type, jintArray _rgba, jint width, jint height,
                        jbyteArray yuv) {
    uint8_t convert_type = (uint8_t) (type & 0x0F);
    int c_size = ((type & 0xF0) >> 4);
    int rgba_stride = c_size * width;

    //int[] byte[]
    int num = (c_size == 4 ? 4 : 3);
    int len = width * height * num;
    jbyte src[len];
    int a, R, G, B, tmp;
    jint *intArray;
    intArray = env->GetIntArrayElements(_rgba, NULL);
    int arr_len = env->GetArrayLength(_rgba);
    for (int i = 0; i < arr_len; i++) {
        tmp = intArray[i];
        R = (tmp & 0xff0000) >> 16;
        G = (tmp & 0xff00) >> 8;
        B = (tmp & 0xff);
        if (num == 4) {
            a = (tmp & 0xff000000) >> 24; // a is not used obviously
            src[i * num] = (jbyte) a;
            src[i * num + 1] = (jbyte) R;
            src[i * num + 2] = (jbyte) G;
            src[i * num + 3] = (jbyte) B;
        } else {
            src[i * num] = (jbyte) R;
            src[i * num + 1] = (jbyte) G;
            src[i * num + 2] = (jbyte) B;
        }
    }
    env->ReleaseIntArrayElements(_rgba, intArray, 0);

    ///
    jbyte *dst = env->GetByteArrayElements(yuv, NULL);
    int ret= convert_i420(src, rgba_stride, width, height, dst, i420Func[convert_type]);

    env->ReleaseByteArrayElements(yuv, dst, 0);
    return ret;
}