#pragma once

#include <glm/vec4.hpp>

namespace ScaleMail
{
class Camera;
class World;

void addAmbientLight(glm::vec4 color, glm::vec4 rect);
void initializeAmbientLight(World& world, Camera& camera);
void updateAmbientLight(World& world, Camera& camera, float elapsedSeconds);
}