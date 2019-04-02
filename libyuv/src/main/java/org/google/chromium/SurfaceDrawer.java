package org.google.chromium;

import android.graphics.Bitmap;
import android.view.Surface;

/**
 * 封装了也没快多少
 * @deprecated
 */
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
        return drawArgb(BitmapUtils.toRgbABytes(bitmap), bitmap.getWidth(), bitmap.getHeight());
    }

    public int drawArgb(byte[] rgba, int width, int height) {
        return YuvJni.doDrawer(nativePtr, rgba, width, height);
    }

    public void Release() {
        if (nativePtr != 0) {
            YuvJni.releaseDrawer(nativePtr);
        }
        mSurface = null;
        nativePtr = 0;
    }
}
