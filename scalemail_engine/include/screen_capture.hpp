#pragma once

#include "gl_headers.hpp"
#include <stdio.h>
#include <sstream>

namespace ScaleMail {
class ScreenCapture
{
	bool mIsCapturing;
	GLFWwindow* mWindow;
	FILE* mStream;

public:
	ScreenCapture();
	~ScreenCapture();
	void captureFrame();
	void initialize(GLFWwindow* window);
	bool isCapturing() const;
	void startCapture();
	void stopCapture();
};
}