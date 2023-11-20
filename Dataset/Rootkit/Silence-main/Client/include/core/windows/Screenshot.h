#pragma once
#ifdef _WIN32

#include <opencv2/opencv.hpp>
#include <Windows.h>

namespace silence::impl
{
    class Screenshot
    {
        HWND hwndDesktop;

    public:
        Screenshot();
        cv::Mat take();
    };
}

#endif