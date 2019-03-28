#include "rgb_yuv.h"
/**
 * rgb转i420
 */
int convert_i420(uint8_t *rgbaData, jint rgba_stride,
                 uint8_t *yuvData, jint y_stride, jint u_stride, jint v_stride,
               jint width, jint height,
               int (*func)(const uint8_t *, int, uint8_t *, int, uint8_t *, int, uint8_t *, int, int,
                           int)) {
    size_t ySize = (size_t) (y_stride * height);
    size_t uSize = (size_t) (u_stride * height >> 1);
    ALOGD("wait convert_i420");
    int ret = func(rgbaData, rgba_stride,
                   yuvData, y_stride,
                   (yuvData) + ySize, u_stride,
                   yuvData + ySize + uSize, v_stride,
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
    int y_stride = width;
    int u_stride = width >> 1;
    int v_stride = u_stride;
    jbyte *rgbaData = env->GetByteArrayElements(rgba, NULL);
    jbyte *yuvData = env->GetByteArrayElements(yuv, NULL);
    int ret =  convert_i420((uint8_t*)rgbaData, rgba_stride, (uint8_t*)yuvData, y_stride, u_stride, v_stride, width,
                      height, i420Func[convert_type]);
    env->ReleaseByteArrayElements(rgba, rgbaData, 0);
    env->ReleaseByteArrayElements(yuv, yuvData, 0);

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
    int y_stride = width;
    int u_stride = width >> 1;
    int v_stride = u_stride;

    //int[] byte[]
    int num = (c_size == 4 ? 4 : 3);
    int len = width * height * num;
    uint8_t rgbaData[len];
    int a, R, G, B, tmp;
    jint *pixels;
    pixels = env->GetIntArrayElements(_rgba, NULL);
    int arr_len = env->GetArrayLength(_rgba);
    ALOGD("jint[] -> jbyte[], len=%d, size=%d, num=%d", arr_len, (width*height), num);
    while (arr_len>0){

        arr_len--;
    }
    for (int i = 0; i < arr_len; i++) {
        tmp = (int)pixels[i];
        R = (tmp & 0xff0000) >> 16;
        G = (tmp & 0xff00) >> 8;
        B = (tmp & 0xff);
        if (num == 4) {
            a = (tmp & 0xff000000) >> 24; // a is not used obviously
            rgbaData[i * num] = (uint8_t) a;
            rgbaData[i * num + 1] = (uint8_t) R;
            rgbaData[i * num + 2] = (uint8_t) G;
            rgbaData[i * num + 3] = (uint8_t) B;
        } else {
            rgbaData[i * num] = (uint8_t) R;
            rgbaData[i * num + 1] = (uint8_t) G;
            rgbaData[i * num + 2] = (uint8_t) B;
        }
    }
    ///
    ALOGD("start convert_i420");
    jbyte *yuvData = env->GetByteArrayElements(yuv, NULL);
    int ret= convert_i420(rgbaData, rgba_stride, (uint8_t*)yuvData, y_stride, u_stride, v_stride, width,
                          height, i420Func[convert_type]);
    env->ReleaseIntArrayElements(_rgba, pixels, 0);
    env->ReleaseByteArrayElements(yuv, yuvData, 0);
    return ret;
}