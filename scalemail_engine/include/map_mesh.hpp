#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace ScaleMail
{
void addTileVertexData(std::vector<float>& meshVertexData,
					   const glm::vec3 position, const glm::vec3 size,
					   const glm::vec2 uv1, const glm::vec2 uv2,
					   float rotate);
}