#pragma once

struct GLFWwindow;

namespace ScaleMail
{
struct GameWindow {
    int width;
    int height;
    GLFWwindow* window;
};
}