//
// Created by kk on 2019/4/4.
//
#include "yuv_util.h"
#include <libyuv.h>

int i420_rotate_crop(uint8_t *src,
                     int src_width, int src_height, int degree,
                     int left, int top, int dst_width, int dst_height, uint8_t *dst) {
    libyuv::RotationMode rotation;
    switch (degree) {
        case 3:
            rotation = libyuv::kRotate270;
            break;
        case 2:
            rotation = libyuv::kRotate180;
            break;
        case 1:
            rotation = libyuv::kRotate90;
            break;
        default:
            rotation = libyuv::kRotate0;
            break;
    }
    int half_width = src_width >> 1;

    int src_y_size = src_width * src_height;
    int src_u_size = (src_width >> 1) * (src_height >> 1);

    int crop_y_start = src_width * top + left;
    int crop_u_start = half_width * (top >> 1) + (left >> 1);
    int crop_v_start = crop_u_start;

    int dst_y_stride, dst_u_stride, dst_v_stride;
    int dst_y_size = dst_width * dst_height;
    int dst_u_size = (dst_width >> 1) * (dst_height >> 1);

    if (rotation == libyuv::kRotate90 || rotation == libyuv::kRotate270) {
        dst_y_stride = dst_height;
        dst_u_stride = dst_height >> 1;
        dst_v_stride = dst_u_stride;
    } else {
        dst_y_stride = dst_width;
        dst_u_stride = dst_width >> 1;
        dst_v_stride = dst_u_stride;
    }

    int ret = libyuv::I420Rotate(
            src + crop_y_start, src_width,
            src + src_y_size + crop_u_start, half_width,
            src + src_y_size + src_u_size + crop_v_start, half_width,
            dst, dst_y_stride,
            dst + dst_y_size, dst_u_stride,
            dst + dst_y_size + dst_u_size, dst_v_stride,
            dst_width, dst_height, rotation);
    return ret;
}

int i420_mirror(uint8_t *src, int width, int height, uint8_t *dst) {
    int y_size = width * height;
    int u_size = (width >> 1) * (height >> 1);

    int half_width = width >> 1;
    int ret = libyuv::I420Mirror(src, width,
                                 src + y_size, half_width,
                                 src + y_size + u_size, half_width,
                                 dst, width,
                                 dst + y_size, half_width,
                                 dst + y_size + u_size, half_width,
                                 width, height);
    return ret;
}


int nv21_to_i420(uint8_t *src, int width, int height, uint8_t *dst) {

    int y_size = width * height;
    int u_size = (width >> 1) * (height >> 1);
    int half_width = width >> 1;

    int ret = libyuv::NV21ToI420(
            src, width,
            src + y_size, width,
            dst, width,
            dst + y_size, half_width,
            dst + y_size + u_size, half_width,
            width, height);
    return ret;
}

int i420_scale(uint8_t *src, int width, int height,
               uint8_t *dst, int dst_width, int dst_height, int mode) {
    mode = mode % 4;
    int src_y_size = width * height;
    int src_u_size = (width >> 1) * (height >> 1);
    int dst_y_size = dst_width * dst_height;
    int dst_u_size = (dst_width >> 1) * (dst_height >> 1);
    int half_width = width >> 1;
    int dst_half_width = dst_width >> 1;

    return libyuv::I420Scale(
            src, width,
            src + src_y_size, half_width,
            src + src_y_size + src_u_size, half_width,
            width, height,
            dst, dst_width,
            dst + dst_y_size, dst_half_width,
            dst + dst_y_size + dst_u_size, dst_half_width,
            dst_width, dst_height,
            (libyuv::FilterMode) mode);
}

int i420_to_nv21(uint8_t *src, int width, int height, uint8_t *dst) {
    int y_size = width * height;
    int u_size = (width >> 1) * (height >> 1);
    int half_width = width >> 1;
    int ret = libyuv::I420ToNV21(
            src, width,
            src + y_size, half_width,
            src + y_size + u_size, half_width,
            dst, width,
            dst + y_size, width,
            width, height);
    return ret;
}

int nv21_to_rgba(uint8_t *src, int width, int height, uint8_t *dst) {
    int y_size = width * height;
    int ret = libyuv::NV21ToABGR(
            src, width,
            src + y_size, width,
            dst,
            width * 4,
            width, height);
    return ret;
}

int i420_to_argb(uint8_t *src, int width, int height, uint8_t *dst) {
    int y_size = width * height;
    int u_size = (width >> 1) * (height >> 1);
    int half_width = width >> 1;
    int ret = libyuv::I420ToBGRA(
            src, width,
            src + y_size, half_width,
            src + y_size + u_size, half_width,
            dst,
            width * 4,
            width, height);
    return ret;
}

int i420_to_rgba(uint8_t *src, int width, int height, uint8_t *dst) {
    int y_size = width * height;
    int u_size = (width >> 1) * (height >> 1);
    int half_width = width >> 1;
    int ret = libyuv::I420ToABGR(
            src, width,
            src + y_size, half_width,
            src + y_size + u_size, half_width,
            dst,
            width * 4,
            width, height);
    return ret;
}

int argb_to_i420(uint8_t *src, int width, int height, uint8_t *dst) {
    int y_size = width * height;
    int u_size = (width >> 1) * (height >> 1);
    int half_width = width >> 1;
    int ret = libyuv::BGRAToI420(src, width * 4,
                                 dst, width,
                                 dst + y_size, half_width,
                                 dst + y_size + u_size, half_width,
                                 width, height);
    return ret;
}
int rgba_to_i420(uint8_t *src, int width, int height, uint8_t *dst) {
    int y_size = width * height;
    int u_size = (width >> 1) * (height >> 1);
    int half_width = width >> 1;
    int ret = libyuv::ABGRToI420(src, width * 4,
                                 dst, width,
                                 dst + y_size, half_width,
                                 dst + y_size + u_size, half_width,
                                 width, height);
    return ret;
}