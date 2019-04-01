//
// Created by user on 2019/4/1.
//

#ifndef LIBYUV_WRAPPER_SURFACE_DRAW_H
#define LIBYUV_WRAPPER_SURFACE_DRAW_H


#include <android/native_window.h>
#include <jni.h>
#include "surface_draw.h"

namespace drawer {
    class SurfaceDrawer {
    public:
        SurfaceDrawer(ANativeWindow *);

        int RGBADrawSurface(jbyte *rgba, int width, int height);

        void Release();

        ~SurfaceDrawer() {
            Release();
        }

    private:
        ANativeWindow_Buffer windowBuffer;
        ANativeWindow *nativeWindow;
    };
}

#endif //LIBYUV_WRAPPER_SURFACE_DRAW_H
