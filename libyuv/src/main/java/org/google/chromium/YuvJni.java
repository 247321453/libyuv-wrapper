package org.google.chromium;

public class YuvJni {
    private static boolean sInit;
    static {
        try {
            System.loadLibrary("yuv");
            sInit = true;
        }catch (Throwable e){
            e.printStackTrace();
            sInit = false;
        }
    }

    /**
     * YUV数据的缩放
     *
     * @param src        原始数据nv21
     * @param width      原始的宽
     * @param height     原始的高
     * @param dst        输出数据i420
     * @param dst_width  输出的宽
     * @param dst_height 输出的高
     * @param mode       压缩模式。这里为0，1，2，3 速度由快到慢，质量由低到高，一般用0就好了，因为0的速度最快
     * @param degree     旋转的角度，90，180和270三种
     * @param isMirror   是否镜像，一般只有270的时候才需要镜像
     **/
    public static native void i420Compress(byte[] src, int width, int height, byte[] dst, int dst_width, int dst_height, int mode, int degree, boolean isMirror);

    /**
     * yuv数据的裁剪操作
     *
     * @param src        原始数据
     * @param width      原始的宽
     * @param height     原始的高
     * @param dst        输出数据
     * @param left       裁剪的x的开始位置，必须为偶数，否则显示会有问题
     * @param top        裁剪的y的开始位置，必须为偶数，否则显示会有问题
     * @param dst_width  输出的宽
     * @param dst_height 输出的高
     **/
    public static native int i420Crop(byte[] src, int width, int height, byte[] dst, int left, int top, int dst_width, int dst_height);

    /**
     * YUV数据的镜像
     *
     * @param src        原始数据
     * @param width      原始的宽
     * @param height     原始的高
     * @param dst        输出数据
     **/
    public static native int i420Mirror(byte[] src, int width, int height, byte[] dst);

    /**
     * YUV数据的旋转
     *
     * @param src        原始数据
     * @param width      原始的宽
     * @param height     原始的高
     * @param dst        输出数据
     * @param degree     旋转的角度，90，180和270三种
     **/
    public static native int i420Rotate(byte[] src, int width, int height, byte[] dst, int degree);
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
    public static native int i420Scale(byte[] src, int width, int height, byte[] dst, int dst_width, int dst_height, int mode);

    /**
     * 将I420转化为NV21
     *
     * @param width   原始的宽
     * @param height  原始的高
     * @param i420Src 原始I420数据
     * @param nv21Src 转化后的NV21数据
     **/
    public static native int i420ToNv21(byte[] i420Src, int width, int height, byte[] nv21Src);

    /**
     * 将NV21转化为I420
     *
     * @param width   原始的宽
     * @param height  原始的高
     * @param i420Src 原始NV21数据
     * @param nv21Src 转化后的I420数据
     **/
    public static native int nv21ToI420(byte[] i420Src, int width, int height, byte[] nv21Src);


    /**
     * @param type   @see RGBType
     * @param width  图片的宽
     * @param height 图片的高
     */
    public static native int convertToi420(int type, byte[] rgbSrc, int width, int height, byte[] i420Dst);
}
