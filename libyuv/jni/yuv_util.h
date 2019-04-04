//
// Created by kk on 2019/4/4.
//

#ifndef LIBYUV_WRAPPER_YUV_UTIL_H
#define LIBYUV_WRAPPER_YUV_UTIL_H

#include <libyuv.h>

int nv21_to_rgba(uint8_t *src, int width, int height, uint8_t *dst);

int nv21_to_i420(uint8_t *src, int width, int height, uint8_t *dst);

int i420_scale(uint8_t *src, int width, int height,
               uint8_t *dst, int dst_width, int dst_height, int mode);

int i420_to_nv21(uint8_t *src, int width, int height, uint8_t *dst);

int i420_to_argb(uint8_t *src, int width, int height, uint8_t *dst);

int i420_to_rgba(uint8_t *src, int width, int height, uint8_t *dst);

int i420_mirror(uint8_t *src, int width, int height, uint8_t *dst);

int i420_rotate_crop(uint8_t *src,
                     int src_width, int src_height, int degree,
                     int left, int top, int dst_width, int dst_height, uint8_t *dst);
int argb_to_i420(uint8_t *src, int width, int height, uint8_t *dst);
#endif //LIBYUV_WRAPPER_YUV_UTIL_H
