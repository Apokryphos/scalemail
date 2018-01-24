#pragma once

#include <glm/vec4.hpp>

typedef struct GLFWwindow;

namespace ScaleMail
{
class Camera;

void render(GLFWwindow* window, Camera& camera, glm::vec4 ambientColor);
}