#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
void addQuadLineVertexData(std::vector<float>& vertexData,
						   const glm::vec2 position, const glm::vec2 size,
						   const glm::vec4 color);
void addQuadVertexData(std::vector<float>& vertexData, const glm::vec2 position,
					   const glm::vec2 size, const glm::vec4 color);
}