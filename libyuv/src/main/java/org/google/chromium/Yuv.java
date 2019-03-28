package org.google.chromium;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
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
        int bytes = bitmap.getByteCount();

        ByteBuffer buf = ByteBuffer.allocate(bytes);
        bitmap.copyPixelsToBuffer(buf);

        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        int type, num;
        if (bitmap.getConfig() == Bitmap.Config.ARGB_4444) {
            num = 4;
            type = RGBType.ARGB;
        } else if (bitmap.getConfig() == Bitmap.Config.ARGB_8888) {
            num = 4;
            type = RGBType.ARGB;
        } else if (bitmap.getConfig() == Bitmap.Config.RGB_565) {
            num = 3;
            type = RGBType.RGB565;
        } else {
            num = 3;
            type = RGBType.RGB565;
        }
        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
        Yuv yuv = new Yuv(width, height, Yuv.Type.I420);
        byte[] src = new byte[width * height * num];
        int arr_len = pixels.length;
        int tmp, a, R, G, B;
        for (int i = 0; i < arr_len; i++) {
            tmp = pixels[i];
            R = (tmp & 0xff0000) >> 16;
            G = (tmp & 0xff00) >> 8;
            B = (tmp & 0xff);
            if (num == 4) {
                a = (tmp & 0xff000000) >> 24; // a is not used obviously
                src[i * num] = (byte) a;
                src[i * num + 1] = (byte) R;
                src[i * num + 2] = (byte) G;
                src[i * num + 3] = (byte) B;
            } else {
                src[i * num] = (byte) R;
                src[i * num + 1] = (byte) G;
                src[i * num + 2] = (byte) B;
            }
        }
        Log.d("yuv_util", String.format("toYuv type=%x", type));
        int err = YuvJni.convertToi420(type, src, width, height, yuv.data);
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
