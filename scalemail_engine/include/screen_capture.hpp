#pragma once

#include <stdio.h>
#include <sstream>

struct GLFWwindow;

namespace ScaleMail {
class ScreenCapture
{
	bool mIsCapturing;
	GLFWwindow* mWindow;
	FILE* mStream;

public:
	ScreenCapture();
	~ScreenCapture();
	ScreenCapture(const ScreenCapture&) = delete;
	ScreenCapture& operator=(const ScreenCapture&) = delete;
	void captureFrame();
	void initialize(GLFWwindow* window);
	bool isCapturing() const;
	void startCapture();
	void stopCapture();
};
}