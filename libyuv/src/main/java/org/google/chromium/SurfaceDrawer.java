package org.google.chromium;

import android.graphics.Bitmap;
import android.view.Surface;

public class SurfaceDrawer {
    private Surface mSurface;
    private long nativePtr;

    public static SurfaceDrawer create(Surface surface) {
        long ptr = YuvJni.createDrawer(surface);
        if (ptr != 0) {
            return new SurfaceDrawer(surface, ptr);
        }
        return null;
    }

    private SurfaceDrawer(Surface surface, long ptr) {
        mSurface = surface;
        nativePtr = ptr;
    }

    public int draw(Bitmap bitmap) {
        return draw(BitmapUtils.toRgbABytes(bitmap), bitmap.getWidth(), bitmap.getHeight());
    }

    public int draw(byte[] rgba, int width, int height) {
        return YuvJni.rgbaDraw(nativePtr, rgba, width, height);
    }

    public void Release() {
        if (nativePtr != 0) {
            YuvJni.releaseDrawer(nativePtr);
        }
        mSurface = null;
        nativePtr = 0;
    }
}
