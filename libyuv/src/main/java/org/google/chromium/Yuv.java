package org.google.chromium;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.YuvImage;

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
        int type;
        if (bitmap.getConfig() == Bitmap.Config.ARGB_4444) {
            type = RGBType.ARGB;
        } else if (bitmap.getConfig() == Bitmap.Config.ARGB_8888) {
            type = RGBType.ARGB;
        } else if (bitmap.getConfig() == Bitmap.Config.RGB_565) {
            type = RGBType.RGB565;
        } else {
            type = RGBType.RGB565;
        }
        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
        Yuv yuv = new Yuv(width, height, Yuv.Type.I420);
        int err = YuvJni.convertToi420I(type, pixels, width, height, yuv.data);
        if (err == 0) {
            return yuv;
        }
        return null;
    }

    public static byte[] newYuvBytes(int width, int height) {
        return new byte[width * height * 3 / 2];
    }

    public void setType(Type type) {
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
        }
    }

    public Bitmap toBitmap(int q) {
        byte[] src;
        if (type != Yuv.Type.NV21) {
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
