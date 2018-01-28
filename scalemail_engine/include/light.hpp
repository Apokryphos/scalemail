#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct GLFWwindow;

namespace ScaleMail
{
class AssetManager;
class Camera;
struct GameWindow;
class LightSystem;

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
void initializeLight(AssetManager& assetManager);
void renderLight(GameWindow& gameWindow, Camera& camera, LightSystem& lightSystem);
void simulateLights(float elapsedSeconds);
}