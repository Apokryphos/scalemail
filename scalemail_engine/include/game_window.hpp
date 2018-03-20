#pragma once

struct GLFWwindow;

namespace ScaleMail
{
class GameWindow
{
	int mWidth;
	int mHeight;
	GLFWwindow* mWindow;

public:
	void center();
	bool getFullscreen() const;
	int getHeight() const;
	int getWidth() const;
	GLFWwindow* getGlfwWindow();
	void resize();
	void setCursorVisible(bool visible);
	void setFullscreen(bool fullscreen);
	void setGlfwWindow(GLFWwindow* window);
};
}