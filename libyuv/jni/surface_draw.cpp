//
// Created by user on 2019/4/1.
//


#include "surface_draw.h"
#include "debug.h"
#include <string>

using namespace drawer;

SurfaceDrawer::SurfaceDrawer(ANativeWindow *_nativeWindow) {
    nativeWindow = _nativeWindow;
}

void SurfaceDrawer::Release() {
    if (nativeWindow != NULL) {
        ANativeWindow_release(nativeWindow);
        nativeWindow = NULL;
    }
}

int SurfaceDrawer::DrawSurface(jbyte *rgba, int width, int height) {
    if (nativeWindow == NULL) {
        return -1;
    }
    if (lastWidth != width && lastHeight != height && format != WINDOW_FORMAT_RGBA_8888) {
        lastWidth = width;
        lastHeight = height;
        format = WINDOW_FORMAT_RGBA_8888;
        ANativeWindow_setBuffersGeometry(nativeWindow, width, height, WINDOW_FORMAT_RGBA_8888);
    }

    ANativeWindow_lock(nativeWindow, &windowBuffer, 0);
    uint8_t *dst = (uint8_t *) windowBuffer.bits;
    uint8_t *src = (uint8_t *) rgba;
    size_t len = static_cast<size_t>(4 * width * height);
    memcpy(dst, src, len);
    ANativeWindow_unlockAndPost(nativeWindow);
    return 0;
}