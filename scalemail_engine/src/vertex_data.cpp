#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
//	============================================================================
void addQuadLineVertexData(std::vector<float>& vertexData,
						   const glm::vec2 position, const glm::vec2 size,
						   const glm::vec4 color) {
	vertexData.emplace_back(position.x);
	vertexData.emplace_back(position.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	vertexData.emplace_back(position.x + size.x);
	vertexData.emplace_back(position.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	vertexData.emplace_back(position.x + size.x);
	vertexData.emplace_back(position.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	vertexData.emplace_back(position.x + size.x);
	vertexData.emplace_back(position.y + size.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	vertexData.emplace_back(position.x + size.x);
	vertexData.emplace_back(position.y + size.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	vertexData.emplace_back(position.x);
	vertexData.emplace_back(position.y + size.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	vertexData.emplace_back(position.x);
	vertexData.emplace_back(position.y + size.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	vertexData.emplace_back(position.x);
	vertexData.emplace_back(position.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);
}

//	============================================================================
void addQuadVertexData(std::vector<float>& vertexData, const glm::vec2 position,
					   const glm::vec2 size, const glm::vec4 color) {
	vertexData.emplace_back(position.x);
	vertexData.emplace_back(position.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	vertexData.emplace_back(position.x);
	vertexData.emplace_back(position.y + size.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	vertexData.emplace_back(position.x + size.x);
	vertexData.emplace_back(position.y + size.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	vertexData.emplace_back(position.x);
	vertexData.emplace_back(position.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	vertexData.emplace_back(position.x + size.x);
	vertexData.emplace_back(position.y + size.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	vertexData.emplace_back(position.x + size.x);
	vertexData.emplace_back(position.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);
}
}