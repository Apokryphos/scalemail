#pragma once

#include "math_util.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <cmath>
#include <vector>

namespace ScaleMail
{
//  ============================================================================
inline void addCircleVertexData(std::vector<float>& vertexData,
								const unsigned int lineCount,
								const glm::vec2& position,
								const float radius,
								const glm::vec4& color) {
	for (unsigned int n = 0; n < lineCount; ++n) {
		float x1 = position.x + radius * std::cos(n * TWO_PI / lineCount);
		float x2 = position.x + radius * std::cos((n + 1) * TWO_PI / lineCount);

		float y1 = position.y + radius * std::sin(n * TWO_PI / lineCount);
		float y2 = position.y + radius * std::sin((n + 1) * TWO_PI / lineCount);

		vertexData.emplace_back(x1);
		vertexData.emplace_back(y1);
		vertexData.emplace_back(color.r);
		vertexData.emplace_back(color.g);
		vertexData.emplace_back(color.b);
		vertexData.emplace_back(color.a);

		vertexData.emplace_back(x2);
		vertexData.emplace_back(y2);
		vertexData.emplace_back(color.r);
		vertexData.emplace_back(color.g);
		vertexData.emplace_back(color.b);
		vertexData.emplace_back(color.a);
	}
}

void addQuadLineVertexData(std::vector<float>& vertexData,
						   const glm::vec2 position, const glm::vec2 size,
						   const glm::vec4 color);
void addQuadVertexData(std::vector<float>& vertexData, const glm::vec2 position,
					   const glm::vec2 size, const glm::vec4 color);
}