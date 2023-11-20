#ifdef __linux__

#include <core/linux/Screenshot.h>

namespace silence::impl {
    Screenshot::Screenshot() {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);
    }

    Screenshot::~Screenshot() {
        if (img != nullptr)
            XDestroyImage(img);
        XCloseDisplay(display);
    }

    cv::Mat Screenshot::take() {
        if (img != nullptr)
            XDestroyImage(img);
        img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
        return {height, width, CV_8UC4, img->data};
    }
}

#endif