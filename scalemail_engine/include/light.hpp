#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct GLFWwindow;

namespace ScaleMail
{
class Camera;
struct GameWindow;

struct Light {
    glm::vec2 position;
    glm::vec4 color;
    int textureId;
    float size;
    float pulse;
    float pulseSize;
};

void addLight(glm::vec2 position, glm::vec4 color, float size, float pulse,
              float pulseSize);
void initializeLight();
void renderLight(GameWindow& gameWindow, Camera& camera, glm::vec4 ambientColor);
void simulateLights(float elapsedSeconds);
}