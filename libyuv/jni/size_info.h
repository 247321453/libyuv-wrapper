//
// Created by kk on 2019/4/11.
//
#ifndef LIBYUV_SIZE_UTIL_H
#define LIBYUV_SIZE_UTIL_H

#define ROTATION_0 0
#define ROTATION_90 1
#define ROTATION_180 2
#define ROTATION_270 3

#define CROP_MODE_TOP 1
#define CROP_MODE_BOTTOM 2
#define CROP_MODE_LEFT 4
#define CROP_MODE_RIGHT 8
#define CROP_MODE_CENTER_HORIZONTAL 10
#define CROP_MODE_CENTER_VERTICAL 20

struct SizeInfo {
    //原始尺寸
    int src_width;
    int src_height;
    //角度
    int src_rotation = ROTATION_0;
    //当前角度
    int display_rotation;
    //当前播放尺寸
    int display_width;
    int display_height;
    bool need_crop;
    bool need_scale;

    int crop_x;
    int crop_y;
    int crop_width;
    int crop_height;

    int rotate_width;
    int rotate_height;
    //缩放大小
    int scale_width;
    int scale_height;
    //旋转后要交换宽高
    bool need_swap_size;
};

void ResetSizeInfo(SizeInfo *info);

void InitSizeInfo(SizeInfo *size, int dst_width, int dst_height, int dst_rotation, bool stretch, int mode);

#endif //AGGREGATE_SIZE_UTIL_H
