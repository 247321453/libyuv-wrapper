//
// Created by user on 2019/4/11.
//

#include <cmath>
#include "debug.h"
#include "size_info.h"

void ResetSizeInfo(SizeInfo *info) {
    //视频原始尺寸
    info->src_width = 0;
    info->src_height = 0;
    info->src_rotation = ROTATION_0;
    //当前角度
    info->display_rotation = 0;
    //当前播放尺寸
    info->display_width = 0;
    info->display_height = 0;
    info->need_scale = false;

    //原始尺寸的坐标
    info->crop_x = 0;
    info->crop_y = 0;
    info->crop_width = 0;
    info->crop_height = 0;
    info->need_crop = false;

    //
    info->scale_width = 0;
    info->scale_height = 0;
    info->rotate_width = 0;
    info->rotate_height = 0;
}

void
InitSizeInfo(SizeInfo *size, int dst_width, int dst_height, int dst_rotation, bool stretch, int mode) {
    if (size->src_height == 0 || size->src_width == 0) {
        ALOGW("need set crop_width and src_height.");
        return;
    }
    ALOGD("rotation=%d, src=%dx%d, dst=%dx%d@%d ", size->src_rotation, size->src_width,
          size->src_height, dst_width, dst_height, (dst_rotation * 90));
    //按照比例自动旋转
    int rotation = size->src_rotation;
    if (dst_rotation > 0) {
        rotation = (rotation + dst_rotation) % 4; // 0-3
        ALOGD("has pre rotate: pre=%d, now=%d", dst_rotation, rotation);
    }
    size->need_swap_size = (rotation == ROTATION_90 || rotation == ROTATION_270);
    if (size->need_swap_size) {
        size->rotate_width = size->src_height;
        size->rotate_height = size->src_width;
        ALOGD("swap size: pre=%dx%d, now=%dx%d",
              size->src_width, size->src_height,
              size->rotate_width, size->rotate_height);
    } else {
        size->rotate_width = size->src_width;
        size->rotate_height = size->src_height;
    }
    size->display_rotation = rotation;
    //旋转之后的
    size->display_width = size->rotate_width;
    size->display_height = size->rotate_height;
    size->crop_x = 0;
    size->crop_y = 0;
    size->crop_width = size->display_width;
    size->crop_height = size->display_height;
    size->scale_width = dst_width;
    size->scale_height = dst_height;

    int src_width = size->display_width;
    int src_height = size->display_height;
    if (dst_width > 0 && dst_height > 0) {
        //基于旋转后的宽高去计算缩放，裁剪
        if (dst_width == src_width && dst_height == src_height) {
            size->need_scale = false;
            size->need_crop = false;
            //宽高一样，不需要裁剪，不需要缩放
            ALOGD("size is same, don't scale and crop");
        } else {
            //默认
            size->need_crop = true;
            size->need_scale = true;
            if (!stretch) {
                if ((((float) dst_width / (float) dst_height) ==
                     ((float) src_width / (float) src_height))) {
                    //一样的比例，不需要裁剪，直接缩放
                    ALOGD("don't crop");
                } else {
                    //目标的宽和原始比例的宽一样
                    int w1 = src_width;//540/360*640=960
                    int h1 = (int) round((float) w1 / (float) dst_width * (float) dst_height);
                    //目标的高和原始比例的高一样
                    int h2 = src_height;//960/640*360=540
                    int w2 = (int) round((float) h2 / (float) dst_height * (float) dst_width);

                    if (h1 < src_height) {
                        //裁剪的高度小于原始高度，进行裁剪
                        size->crop_x = 0;
                        if ((mode & CROP_MODE_TOP) != 0) {
                            //保留顶部
                            size->crop_y = 0;
                        } else if ((mode & CROP_MODE_BOTTOM) != 0) {
                            //保留底部
                            size->crop_y = (src_height - h1) * 2 / 2;
                        } else {
                            //保留中间
                            size->crop_y = (src_height - h1) / 4 * 2;//不能单数 26/2=13  26/4*2=12
                        }
                        size->crop_width = w1;
                        size->crop_height = h1;
                    } else if (w2 < src_width) {
                        //裁剪的宽度小于原始宽度，进行裁剪
                        //宽度比目标宽
                        if ((mode & CROP_MODE_LEFT) != 0) {
                            //保留顶部
                            size->crop_x = 0;
                        } else if ((mode & CROP_MODE_RIGHT) != 0) {
                            //保留底部
                            size->crop_x = (src_width - w2) * 2 / 2;
                        } else {
                            //保留中间
                            size->crop_x = (src_width - w2) / 4 * 2;
                        }
                        size->crop_y = 0;
                        size->crop_width = w2;
                        size->crop_height = h2;
                    } else {
                        //
                        ALOGW("how to crop?");
                    }
                }
            }
        }
    } else {
        size->need_scale = false;
        size->need_crop = false;
        ALOGD("don't scale and crop");
    }
    if (size->need_scale) {
        size->display_width = size->scale_width;
        size->display_height = size->scale_height;
    }
    int crop_x, crop_y, crop_w, crop_h;
    if (size->need_swap_size) {
        if (size->need_scale) {
            crop_x = size->crop_y;
            crop_y = size->crop_x;
            crop_w = size->crop_height;
            crop_h = size->crop_width;
        } else {
            crop_x = 0;
            crop_y = 0;
            crop_w = size->rotate_height;
            crop_h = size->rotate_width;
        }
    } else {
        if (size->need_scale) {
            crop_x = size->crop_x;
            crop_y = size->crop_y;
            crop_w = size->crop_width;
            crop_h = size->crop_height;
        } else {
            crop_x = 0;
            crop_y = 0;
            crop_w = size->rotate_width;
            crop_h = size->rotate_height;
        }
    }
    size->crop_x = crop_x;
    size->crop_y = crop_y;
    size->crop_width = crop_w;
    size->crop_height = crop_h;
    ALOGD("need crop %d,%d,%d,%d", crop_x, crop_y, crop_w, crop_h);
}
