#include "gl_headers.hpp"
#include "screen_capture.hpp"
#include <stdio.h>
#include <sstream>

namespace ScaleMail {

//	===========================================================================
ScreenCapture::ScreenCapture() :
    mIsCapturing(false),
    mWindow(nullptr),
    mStream(nullptr)
{
}

//	===========================================================================
ScreenCapture::~ScreenCapture() {
    this->stopCapture();

}

//	===========================================================================
void ScreenCapture::captureFrame() {
    if (!mIsCapturing) {
        return;
    }

    int width, height;
    glfwGetWindowSize(mWindow, &width, &height);

    int* buffer = new int[width * height];

    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    fwrite(buffer, sizeof(int) * width * height, 1, mStream);
}

//	===========================================================================
void ScreenCapture::initialize(GLFWwindow* window) {
    mWindow = window;
}

//	===========================================================================
void ScreenCapture::startCapture() {
    if (mIsCapturing) {
        return;
    }

    int width, height;
    glfwGetWindowSize(mWindow, &width, &height);

    std::stringstream cmd;
    cmd << "/usr/bin/ffmpeg -f rawvideo -pix_fmt rgba "
    << "-s " << width << "x" << height << " "
    << "-i - -y -threads 0 "
    << "-preset veryfast " // Encoding speed: ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow
    << "-c:v libx264 -crf 18 " // codec & quality. Range is logarithmic 0 (lossless) to 51 (worst quality). Default is 23.
    << "-vf vflip -framerate 30 "
    << "output.mp4";

    //  This generated corrupt video...
    // const char* cmd =
    //     "ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s 1280x720 -i - "
    //     "-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";

#ifdef __linux__
    mStream = popen(cmd.str().c_str(), "w");
#elif _WIN32
    mStream = _popen(cmd.str().c_str(), "w");
#endif

    mIsCapturing = true;
}

//	===========================================================================
void ScreenCapture::stopCapture() {
    if (mIsCapturing) {
        fclose(mStream);
        mStream = nullptr;
        mIsCapturing = false;
    }
}
}