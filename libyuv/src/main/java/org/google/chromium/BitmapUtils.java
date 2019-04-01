package org.google.chromium;

import android.graphics.Bitmap;

class BitmapUtils {
    static byte[] toRgbBytes(Bitmap bitmap) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
        int arr_len = pixels.length;
        int tmp, a, R, G, B, num = 3;
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
        }
        return src;
    }

    static byte[] toRgbABytes(Bitmap bitmap) {
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

    static byte[] toArgbBytes(Bitmap bitmap) {
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
