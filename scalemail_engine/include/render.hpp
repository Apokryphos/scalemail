#pragma once

#include <glm/vec4.hpp>

struct GLFWwindow;

namespace ScaleMail
{
class Camera;
class GameState;
class World;

void render(GLFWwindow* window, World& world, Camera& camera, GameState& gameState,
            float totalElapsedSeconds);
}