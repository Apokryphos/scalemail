#pragma once

#include <glm/vec4.hpp>

namespace ScaleMail
{
void addAmbientLight(glm::vec4 color, glm::vec4 rect);
void buildAmbientLights();
void drawAmbientLights();
void initializeAmbientLights();
}