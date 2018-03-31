#pragma once

struct GLFWwindow;

namespace ScaleMail
{
class InputDevice;

InputDevice& getKeyboardInputDevice();
void initializeInput(GLFWwindow* window);
}