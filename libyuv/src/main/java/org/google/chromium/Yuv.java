package org.google.chromium;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.YuvImage;

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;

public class Yuv {
    private int width;
    private int height;
    private byte[] data;
    private YuvType type;

    public Yuv(int width, int height, YuvType type) {
        this.width = width;
        this.height = height;
        this.data = new byte[width * height * 3 / 2];
        this.type = type;
    }

    public YuvType getType() {
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
        Yuv yuv = new Yuv(width, height, YuvType.I420);
        int err = YuvJni.convertToi420I(type, pixels, width, height, yuv.data);
        if (err == 0) {
            return yuv;
        }
        return null;
    }

    public Bitmap toBitmap() {
        byte[] src;
        if (type != YuvType.NV21) {
            src = this.data;
        } else {
            //转为nv21
            src = new byte[width * height * 3 / 2];
            YuvJni.i420ToNv21(this.data, width, height, src);
        }
        YuvImage yuvImage = new YuvImage(src, ImageFormat.NV21, width, height, null);
        ByteArrayOutputStream fOut = new ByteArrayOutputStream();
        yuvImage.compressToJpeg(new Rect(0, 0, width, height), 100, fOut);
        byte[] bitData = fOut.toByteArray();
        return BitmapFactory.decodeByteArray(bitData, 0, bitData.length);
    }
}
