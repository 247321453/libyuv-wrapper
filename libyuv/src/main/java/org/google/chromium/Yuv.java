package org.google.chromium;

import android.graphics.Bitmap;
import android.view.Surface;
import android.view.SurfaceHolder;

/*
 * YUV420p：又叫planer平面模式，Y ，U，V分别再不同平面，也就是有三个平面。
 * YUV420p又分为：他们的区别只是存储UV的顺序不一样而已。
 * I420:又叫YU12，安卓的模式。存储顺序是先存Y，再存U，最后存V。YYYYUUUVVV
 * YV12:存储顺序是先存Y，再存V，最后存U。YYYVVVUUU
 *
 * YUV420sp：又叫bi-planer或two-planer双平面，Y一个平面，UV在同一个平面交叉存储。
 * YUV420sp又分为：他们的区别只是存储UV的顺序不一样而已。
 * NV12:IOS只有这一种模式。存储顺序是先存Y，再UV交替存储。YYYYUVUVUV
 * NV21:安卓的模式。存储顺序是先存Y，再存U，再VU交替存储。YYYYVUVUVU
 */
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

    public abstract int draw(Surface surface);

    public int draw(SurfaceHolder holder) {
        return draw(holder.getSurface());
    }

    public static int drawSurface(Surface surface, Bitmap bitmap) {
        return YuvJni.rgbaDrawSurface(surface, toRgbABytes(bitmap), bitmap.getWidth(), bitmap.getHeight());
    }

    public static byte[] newYuvBytes(int width, int height) {
        return new byte[width * height * 3 / 2];
    }

    /***
     * 相机是nv21
     */
    public static YuvI420 toYuvI420(byte[] nv21Data, int width, int height) {
        byte[] src = newYuvBytes(width, height);
        YuvJni.nv21ToI420(nv21Data, width, height, src);
        return new YuvI420(width, height, src);
    }

    public static YuvNv21 toYuvNv21(Bitmap bitmap) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        YuvNv21 yuv = new YuvNv21(width, height);
        byte[] src = toArgbBytes(bitmap);
        int err = YuvJni.argbToNv21(src, width, height, yuv.data);
        if (err == 0) {
            return yuv;
        }
        return null;
    }

    public static YuvI420 toYuvI420(Bitmap bitmap) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        YuvI420 yuv = new YuvI420(width, height);
        byte[] src = toArgbBytes(bitmap);
        int err = YuvJni.argbToI420(src, width, height, yuv.data);
        if (err == 0) {
            return yuv;
        }
        return null;
    }

    public static byte[] toRgbABytes(Bitmap bitmap) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        boolean is565 = bitmap.getConfig() == Bitmap.Config.RGB_565;
        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
        int arr_len = pixels.length;
        int tmp, a, R, G, B, num = 4;
        byte[] src = new byte[width * height * num];
        for (int i = 0; i < arr_len; i++) {
            tmp = pixels[i];
            a = (tmp & 0xff000000) >> 24; // a is not used obviously
            R = (tmp & 0x00ff0000) >> 16;
            G = (tmp & 0x0000ff00) >> 8;
            B = (tmp & 0x000000ff);

            src[i * num] = (byte) R;
            src[i * num + 1] = (byte) G;
            src[i * num + 2] = (byte) B;
            src[i * num + 3] = (byte) (is565 ? 0x255 : a);
        }
        return src;
    }

    public static byte[] toArgbBytes(Bitmap bitmap) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        boolean is565 = bitmap.getConfig() == Bitmap.Config.RGB_565;
        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
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
        return src;
    }
}
