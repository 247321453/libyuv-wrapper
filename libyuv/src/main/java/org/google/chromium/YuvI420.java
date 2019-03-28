package org.google.chromium;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.util.Log;

import java.io.ByteArrayOutputStream;

public class YuvI420 extends Yuv {
    public YuvI420(int width, int height) {
        super(width, height);
    }

    public YuvI420(int width, int height, byte[] data) {
        super(width, height, data);
    }

    @Override
    public Type getType() {
        return Type.I420;
    }

    @Override
    public YuvNv21 toYuvNv21() {
        byte[] src = newYuvBytes(width, height);
        YuvJni.i420ToNv21(this.data, width, height, src);
        return new YuvNv21(width, height, src);
    }

    @Override
    public YuvI420 toYuvI420() {
        return this;
    }

    @Override
    public Bitmap toBitmap(int q) {
        byte[] src = newYuvBytes(width, height);
        YuvJni.i420ToNv21(this.data, width, height, src);
        YuvImage yuvImage = new YuvImage(src, ImageFormat.NV21, width, height, null);
        ByteArrayOutputStream fOut = new ByteArrayOutputStream();
        yuvImage.compressToJpeg(new Rect(0, 0, width, height), q, fOut);
        byte[] bitData = fOut.toByteArray();
        return BitmapFactory.decodeByteArray(bitData, 0, bitData.length);
    }

    public boolean crop(int left, int top, int width, int height) {
        if (width == this.width && height == this.height) {
            return true;
        }
        byte[] dst = newYuvBytes(width, height);
        int ret = YuvJni.i420Crop(this.data, this.width, this.height, dst, left, top, width, height);
        if (ret == 0) {
            this.data = dst;
            this.width = width;
            this.height = height;
            return true;
        }
        return false;
    }

    /**
     * @param width
     * @param height
     * @param mode   0-5
     */
    public boolean scale(int width, int height, int mode) {
        byte[] dst = newYuvBytes(width, height);
        Log.d(TAG, "scale:" + this.width + "x" + this.height + "->" + width + "x" + height);
        int ret = YuvJni.i420Scale(this.data, this.width, this.height, dst, width, height, mode);
        if (ret == 0) {
            this.data = dst;
            this.width = width;
            this.height = height;
            return true;
        }
        Log.e(TAG, "scale error:"+ret);
        return false;
    }

    /**
     * @param degree 90,180,270
     */
    public boolean rotate(int degree) {
        if (degree == 0) {
            return true;
        }
        if ((degree != 90) && (degree != 180) && (degree != 270)) {
            return false;
        }
        Log.d(TAG, "rotate:" + degree);
        byte[] dst = newYuvBytes(width, height);
        int ret = YuvJni.i420Rotate(this.data, this.width, this.height, dst, degree);
        if (ret == 0) {
            this.data = dst;
            if (degree == 90 || degree == 270) {
                //noinspection SuspiciousNameCombination
                int tmp = width;
                this.width = height;
                this.height = tmp;
            }
            return true;
        }
        Log.w(TAG, "rotate error" + ret);
        return false;
    }

    public boolean mirror() {
        byte[] dst = newYuvBytes(this.width, this.height);
        int ret = YuvJni.i420Mirror(this.data, this.width, this.height, dst);
        if (ret == 0) {
            this.data = dst;
            return true;
        }
        return false;
    }
}
