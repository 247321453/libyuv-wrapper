package org.google.chromium;

import android.graphics.Bitmap;

public abstract class Yuv {
    public enum Type {
        I420,
        Nv21
    }

    public static final String TAG = "libyuv";
    protected int width;
    protected int height;
    protected byte[] data;

    public Yuv(int width, int height) {
        this(width, height, newYuvBytes(width, height));
    }

    public Yuv(int width, int height, byte[] data) {
        this.width = width;
        this.height = height;
        this.data = data;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    public byte[] getData() {
        return data;
    }

    public abstract Yuv.Type getType();

    public abstract YuvNv21 toYuvNv21();

    public abstract YuvI420 toYuvI420();

    public abstract Bitmap toBitmap(int q);

    public static byte[] newYuvBytes(int width, int height) {
        return new byte[width * height * 3 / 2];
    }

    /***
     * 相机是nv21
     */
    public static YuvI420 toYuvI420(byte[] nv21Data, int width, int height){
        byte[] src = newYuvBytes(width, height);
        YuvJni.nv21ToI420(nv21Data, width, height, src);
        return new YuvI420(width, height, src);
    }

    public static YuvI420 toYuvI420(Bitmap bitmap) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        boolean is565 = bitmap.getConfig() == Bitmap.Config.RGB_565;
        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
        YuvI420 yuvI420 = new YuvI420(width, height);
        int arr_len = pixels.length;
        int tmp, a, R, G, B, num = 4;
        byte[] src = new byte[width * height * num];
        for (int i = 0; i < arr_len; i++) {
            tmp = pixels[i];
            a = (tmp & 0xff000000) >> 24; // a is not used obviously
            R = (tmp & 0x00ff0000) >> 16;
            G = (tmp & 0x0000ff00) >> 8;
            B = (tmp & 0x000000ff);
            src[i * num] = (byte) (is565 ? 0x255 : a);
            src[i * num + 1] = (byte) R;
            src[i * num + 2] = (byte) G;
            src[i * num + 3] = (byte) B;
        }
        int err = YuvJni.argbToi420(src, width, height, yuvI420.data);
        if (err == 0) {
            return yuvI420;
        }
        return null;
    }
}
