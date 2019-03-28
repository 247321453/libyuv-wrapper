package org.google.chromium;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.ImageFormat;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;

public class Yuv {
    public enum Type {
        I420,
        NV21
    }

    private int width;
    private int height;
    private byte[] data;
    private Type type;

    public Yuv(int width, int height, Type type) {
        this.width = width;
        this.height = height;
        this.data = newYuvBytes(width, height);
        this.type = type;
    }

    public Yuv.Type getType() {
        return type;
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

    public static Yuv toYuv(Bitmap bitmap) {
//        Bitmap bitmap = Bitmap.createBitmap(bmp.getWidth(), bmp.getHeight(), Bitmap.Config.RGB_565);
//        Canvas canvas = new Canvas(bitmap);
//        Paint paint = new Paint();
//        canvas.drawBitmap(bmp, 0, 0, paint);

        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        boolean is565 = bitmap.getConfig() == Bitmap.Config.RGB_565;
        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
        Yuv yuv = new Yuv(width, height, Yuv.Type.I420);
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
        int err = YuvJni.convertToi420(RGBType.ARGB, src, width, height, yuv.data);
        if (err == 0) {
            return yuv;
        }
        return null;
    }

    public static byte[] newYuvBytes(int width, int height) {
        return new byte[width * height * 3 / 2];
    }

    public void convertType(Type type) {
        if (this.type != type) {
            if (type == Type.NV21) {
                byte[] src = newYuvBytes(width, height);
                YuvJni.i420ToNv21(this.data, width, height, src);
                this.data = src;
            } else {
                byte[] src = newYuvBytes(width, height);
                YuvJni.nv21ToI420(this.data, width, height, src);
                this.data = src;
            }
            this.type = type;
        }
    }

    public Bitmap toBitmap(int q) {
        byte[] src;
        if (type == Yuv.Type.NV21) {
            src = this.data;
        } else {
            //转为nv21
            src = newYuvBytes(width, height);
            YuvJni.i420ToNv21(this.data, width, height, src);
        }
        YuvImage yuvImage = new YuvImage(src, ImageFormat.NV21, width, height, null);
        ByteArrayOutputStream fOut = new ByteArrayOutputStream();
        yuvImage.compressToJpeg(new Rect(0, 0, width, height), q, fOut);
        byte[] bitData = fOut.toByteArray();
        return BitmapFactory.decodeByteArray(bitData, 0, bitData.length);
    }
}
