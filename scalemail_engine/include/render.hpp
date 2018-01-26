#pragma once

#include <glm/vec4.hpp>

struct GLFWwindow;

namespace ScaleMail
{
class Camera;
class World;

void render(GLFWwindow* window, World& world, Camera& camera,
            glm::vec4 ambientColor);
}