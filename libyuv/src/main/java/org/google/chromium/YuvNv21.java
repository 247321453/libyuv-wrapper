package org.google.chromium;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.view.Surface;

import java.io.ByteArrayOutputStream;

public class YuvNv21 extends Yuv {
    public YuvNv21(int width, int height) {
        super(width, height);
    }

    public YuvNv21(int width, int height, byte[] data) {
        super(width, height, data);
    }

    @Override
    public Type getType() {
        return Type.Nv21;
    }

    @Override
    public YuvNv21 toYuvNv21() {
        return this;
    }

    @Override
    public YuvI420 toYuvI420() {
        byte[] src = newYuvBytes(width, height);
        YuvJni.nv21ToI420(this.data, width, height, src);
        return new YuvI420(width, height, src);
    }

    @Override
    public Bitmap toBitmap(int q) {
        YuvImage yuvImage = new YuvImage(this.data, ImageFormat.NV21, width, height, null);
        ByteArrayOutputStream fOut = new ByteArrayOutputStream();
        yuvImage.compressToJpeg(new Rect(0, 0, width, height), q, fOut);
        byte[] bitData = fOut.toByteArray();
        return BitmapFactory.decodeByteArray(bitData, 0, bitData.length);
    }

    @Override
    public int draw(Surface surface) {
        return YuvJni.nv21DrawSurface(surface, this.data, width, height);
    }
}
