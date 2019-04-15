package org.google.chromium;

import android.view.Gravity;
import android.view.Surface;

class YuvJni {
    private static boolean sInit;

    static {
        try {
            System.loadLibrary("yuv-util");
            sInit = true;
        } catch (Throwable e) {
            e.printStackTrace();
            sInit = false;
        }
    }

    static native int i420DrawSurface(Surface surface, byte[] i420, int width, int height);

    static native int nv21DrawSurface(Surface surface, byte[] nv21, int width, int height);

    static native int rgbaDrawSurface(Surface surface, byte[] rgba, int width, int height);

    /**
     * @param width  图片的宽
     * @param height 图片的高
     */
    static native int i420ToArgb(byte[] i420Data, int width, int height, byte[] argbDst);

    /**
     * @param width  图片的宽
     * @param height 图片的高
     */
    static native int nv21ToArgb(byte[] nv21Data, int width, int height, byte[] argbData);

    /**
     * @param width  图片的宽
     * @param height 图片的高
     */
    static native int argbToI420(byte[] rgbSrc, int width, int height, byte[] i420Dst);

    /**
     * @param width  图片的宽
     * @param height 图片的高
     */
    static native int argbToNv21(byte[] rgbSrc, int width, int height, byte[] nv21Data);

    /**
     * 将I420转化为NV21
     *
     * @param width   原始的宽
     * @param height  原始的高
     * @param i420Src 原始I420数据
     * @param nv21Src 转化后的NV21数据
     **/
    static native int i420ToNv21(byte[] i420Src, int width, int height, byte[] nv21Src);

    /**
     * YUV数据的镜像
     *
     * @param src    原始数据
     * @param width  原始的宽
     * @param height 原始的高
     * @param dst    输出数据
     **/
    static native int i420Mirror(byte[] src, int width, int height, byte[] dst);


    /**
     * YUV数据的旋转，裁剪
     *
     * @param src      原始数据
     * @param width    原始的宽
     * @param height   原始的高
     * @param rotation 0-3
     * @see Surface#ROTATION_0, Surface#ROTATION_90, Surface#ROTATION_180, Surface#ROTATION_270
     **/
    static native int i420RotateWithCrop(byte[] src, int width, int height, int rotation,
                                         byte[] dst, int crop_x, int crop_y, int dst_width, int dst_height);


    /**
     * 可以用|组合
     */
    public static final int CROP_MODE_TOP = 1;
    public static final int CROP_MODE_BOTTOM = 2;
    public static final int CROP_MODE_LEFT = 4;
    public static final int CROP_MODE_RIGHT = 8;
    public static final int CROP_MODE_CENTER_HORIZONTAL = 10;
    public static final int CROP_MODE_CENTER_VERTICAL = 20;

    public static final int CROP_MODE_CENTER = CROP_MODE_CENTER_HORIZONTAL|CROP_MODE_CENTER_VERTICAL;

    /**
     * YUV数据的旋转，裁剪
     *
     * @param src      原始数据
     * @param width    原始的宽
     * @param height   原始的高
     * @param rotation 0-3 旋转再处理
     * @param width    目标的宽
     * @param height   目标的高
     * @param mode     默认居中裁剪#CROP_MODE_CENTER
     * @see #CROP_MODE_CENTER
     * @see Surface#ROTATION_0, Surface#ROTATION_90, Surface#ROTATION_180, Surface#ROTATION_270
     **/
    static native int i420RotateWithCropEx(byte[] src, int width, int height, int rotation,
                                           byte[] dst, int dst_width, int dst_height, boolean stretch, int mode);

    /**
     * 将NV21转化为I420
     *
     * @param width   原始的宽
     * @param height  原始的高
     * @param i420Src 原始NV21数据
     * @param nv21Src 转化后的I420数据
     **/
    static native int nv21ToI420(byte[] i420Src, int width, int height, byte[] nv21Src);

    /**
     * i420数据的缩放
     *
     * @param src        原始数据
     * @param width      原始的宽
     * @param height     原始的高
     * @param dst        输出数据
     * @param dst_width  输出的宽
     * @param dst_height 输出的高
     * @param mode       压缩模式。这里为0，1，2，3 速度由快到慢，质量由低到高，一般用0就好了，因为0的速度最快
     **/
    static native int i420Scale(byte[] src, int width, int height, byte[] dst, int dst_width, int dst_height, int mode);


}
