
#include "yuv.h"

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
        ret = libyuv::I420Rotate((const uint8_t *) src_i420_y_data, width,
                                 (const uint8_t *) src_i420_u_data, width >> 1,
                                 (const uint8_t *) src_i420_v_data, width >> 1,
                                 (uint8_t *) dst_i420_y_data, width,
                                 (uint8_t *) dst_i420_u_data, width >> 1,
                                 (uint8_t *) dst_i420_v_data, width >> 1,
                                 width, height,
                                 (libyuv::RotationMode) degree);
    }
    return ret;
}
jint jni_i420_rotate(JNIEnv *env, jclass clazz, jbyteArray src_i420_data, jint width, jint height,
                     jbyteArray dst_i420_data, jint degree) {
    jbyte *src = env->GetByteArrayElements(src_i420_data, JNI_FALSE);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, JNI_FALSE);

    int ret = i420_rotate(src, width, height, dst, degree);

    env->ReleaseByteArrayElements(src_i420_data, src, JNI_OK);
    env->ReleaseByteArrayElements(dst_i420_data, dst, JNI_OK);
    return ret;
}

int i420_mirror(jbyte *src_i420_data, int width, int height, jbyte *dst_i420_data) {
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);

    uint8_t *src_i420_y_data = (uint8_t *) src_i420_data;
    uint8_t *src_i420_u_data = (uint8_t *) (src_i420_data + src_i420_y_size);
    uint8_t *src_i420_v_data = (uint8_t *) (src_i420_data + src_i420_y_size + src_i420_u_size);

    uint8_t *dst_i420_y_data = (uint8_t *) dst_i420_data;
    uint8_t *dst_i420_u_data = (uint8_t *) (dst_i420_data + src_i420_y_size);
    uint8_t *dst_i420_v_data = (uint8_t *) (dst_i420_data + src_i420_y_size + src_i420_u_size);

    int ret = libyuv::I420Mirror(src_i420_y_data, width,
                       src_i420_u_data, width >> 1,
                       src_i420_v_data, width >> 1,
                       dst_i420_y_data, width,
                       dst_i420_u_data, width >> 1,
                       dst_i420_v_data, width >> 1,
                       width, height);
    return ret;
}

jint jni_i420_mirror(JNIEnv *env, jclass clazz, jbyteArray src_i420_data, jint width, jint height,
                     jbyteArray dst_i420_data) {
    jbyte *src = env->GetByteArrayElements(src_i420_data, JNI_FALSE);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, JNI_FALSE);

    int ret = i420_mirror(src, width, height, dst);

    env->ReleaseByteArrayElements(src_i420_data, src, JNI_OK);
    env->ReleaseByteArrayElements(dst_i420_data, dst, JNI_OK);
    return ret;
}

int nv21_to_i420(jbyte *src_nv21_data, int width, int height, jbyte *src_i420_data) {
    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    uint8_t *src_nv21_y_data = (uint8_t *) src_nv21_data;
    uint8_t *src_nv21_vu_data = (uint8_t *) (src_nv21_data + src_y_size);

    uint8_t *src_i420_y_data = (uint8_t *) src_i420_data;
    uint8_t *src_i420_u_data = (uint8_t *) (src_i420_data + src_y_size);
    uint8_t *src_i420_v_data = (uint8_t *) (src_i420_data + src_y_size + src_u_size);


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
    jbyte *src = env->GetByteArrayElements(src_nv21_data, JNI_FALSE);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, JNI_FALSE);

    int ret = nv21_to_i420(src, width, height, dst);

    env->ReleaseByteArrayElements(src_nv21_data, src, JNI_OK);
    env->ReleaseByteArrayElements(dst_i420_data, dst, JNI_OK);
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
    uint8_t *dst_i420_u_data = (uint8_t *) (dst_i420_data + dst_i420_y_size);
    uint8_t *dst_i420_v_data = (uint8_t *) (dst_i420_data + dst_i420_y_size + dst_i420_u_size);

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
    uint8_t *src_i420_u_data = (uint8_t *) (src_i420_data + src_i420_y_size);
    uint8_t *src_i420_v_data = (uint8_t *) (src_i420_data + src_i420_y_size + src_i420_u_size);

    jint dst_i420_y_size = dst_width * dst_height;
    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);

    uint8_t *dst_i420_y_data = (uint8_t *) dst_i420_data;
    uint8_t *dst_i420_u_data = (uint8_t *) (dst_i420_data + dst_i420_y_size);
    uint8_t *dst_i420_v_data = (uint8_t *) (dst_i420_data + dst_i420_y_size + dst_i420_u_size);

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
    jbyte *src = env->GetByteArrayElements(src_i420_data, JNI_FALSE);
    jbyte *dst = env->GetByteArrayElements(dst_i420_data, JNI_FALSE);

    int ret = i420_scale(src, width, height, dst, dst_width, dst_height, mode);

    env->ReleaseByteArrayElements(src_i420_data, src, JNI_OK);
    env->ReleaseByteArrayElements(dst_i420_data, dst, JNI_OK);
    return ret;
}

jint jni_i420_to_nv21(JNIEnv *env, jclass clazz, jbyteArray i420_data, jint width, jint height,
                      jbyteArray nv21_data) {

    int dst_stride_y = width, src_stride_y = width;
    int dst_stride_vu = width;
    int src_stride_u = width >> 1;
    int src_stride_v = src_stride_u;

    size_t y_size = (size_t) (src_stride_y * height);
    size_t u_size = (size_t) (src_stride_u * (height >> 1));

    jbyte *src = env->GetByteArrayElements(i420_data, JNI_FALSE);
    jbyte *dst = env->GetByteArrayElements(nv21_data, JNI_FALSE);

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

    env->ReleaseByteArrayElements(i420_data, src, JNI_OK);
    env->ReleaseByteArrayElements(nv21_data, dst, JNI_OK);

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

    i420_scale(Src_i420_data, width, height, Src_i420_data_scale, dst_width, dst_height, mode);
    if (isMirror) {
        i420_rotate(Src_i420_data_scale, dst_width, dst_height, Src_i420_data_rotate, degree);
        i420_mirror(Src_i420_data_rotate, dst_height, dst_width, Dst_data);
    } else {
        i420_rotate(Src_i420_data_scale, dst_width, dst_height, Dst_data, degree);
    }
    env->ReleaseByteArrayElements(dst_, Dst_data, 0);
}
