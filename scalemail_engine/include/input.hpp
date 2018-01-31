#pragma once

#include "input_state.hpp"

struct GLFWwindow;

namespace ScaleMail
{
InputState getKeyboardInputState();
void initializeInput(GLFWwindow* window);
}