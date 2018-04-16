#pragma once

struct GLFWwindow;

namespace ScaleMail
{
class InputDevice;

InputDevice& getKeyboardInputDevice();
void initializeInput(GLFWwindow* window);
void inputKeyCallback(GLFWwindow* window, int key, int scancode, int action,
					  int mods);
void inputMouseButtonCallback(GLFWwindow* window, int button, int action,
							  int mods);
}