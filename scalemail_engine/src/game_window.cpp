#include "game_window.hpp"
#include "gl_headers.hpp"

namespace ScaleMail
{
//	============================================================================
void GameWindow::center() {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	int monitorX;
	int monitorY;
	glfwGetMonitorPos(monitor, &monitorX, &monitorY);

	const int centerX = monitorX + mode->width / 2;
	const int centerY = monitorY + mode->height / 2;

	int windowWidth;
	int windowHeight;
	glfwGetWindowSize(mWindow, &windowWidth, &windowHeight);

	const int positionX = centerX - windowWidth / 2;
	const int positionY = centerY - windowHeight / 2;

	glfwSetWindowPos(mWindow, positionX, positionY);
}

//	============================================================================
bool GameWindow::getFullscreen() const {
	return glfwGetWindowMonitor(mWindow) != nullptr;
}

//	============================================================================
GLFWwindow* GameWindow::getGlfwWindow() {
	return mWindow;
}


//	============================================================================
int GameWindow::getHeight() const {
	return mHeight;
}

//	============================================================================
int GameWindow::getWidth() const {
	return mWidth;
}

//	============================================================================
void GameWindow::resize() {
	glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
}

//	============================================================================
void GameWindow::setCursorVisible(bool visible) {
	if (visible) {
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	} else {
		glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
}

//	============================================================================
void GameWindow::setFullscreen(bool fullscreen) {
}

//	============================================================================
void GameWindow::setGlfwWindow(GLFWwindow* window) {
	mWindow = window;
	glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
}
}