#pragma once

#include <glm/vec3.hpp>

namespace ScaleMail
{
void getTilesetUv(const int index, const int width, const int height,
                  const int tileWidth, const int tileHeight,
                  glm::vec2& uv1, glm::vec2& uv2);
}