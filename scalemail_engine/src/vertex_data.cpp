#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
//	============================================================================
void addQuadVertexData(std::vector<float>& vertexData, const glm::vec2 position,
					   const glm::vec2 size, const glm::vec4 color) {
	vertexData.push_back(position.x);
	vertexData.push_back(position.y);
	vertexData.push_back(color.r);
	vertexData.push_back(color.g);
	vertexData.push_back(color.b);
	vertexData.push_back(color.a);

	vertexData.push_back(position.x + size.x);
	vertexData.push_back(position.y);
	vertexData.push_back(color.r);
	vertexData.push_back(color.g);
	vertexData.push_back(color.b);
	vertexData.push_back(color.a);

	vertexData.push_back(position.x + size.x);
	vertexData.push_back(position.y);
	vertexData.push_back(color.r);
	vertexData.push_back(color.g);
	vertexData.push_back(color.b);
	vertexData.push_back(color.a);

	vertexData.push_back(position.x + size.x);
	vertexData.push_back(position.y + size.y);
	vertexData.push_back(color.r);
	vertexData.push_back(color.g);
	vertexData.push_back(color.b);
	vertexData.push_back(color.a);

	vertexData.push_back(position.x + size.x);
	vertexData.push_back(position.y + size.y);
	vertexData.push_back(color.r);
	vertexData.push_back(color.g);
	vertexData.push_back(color.b);
	vertexData.push_back(color.a);

	vertexData.push_back(position.x);
	vertexData.push_back(position.y + size.y);
	vertexData.push_back(color.r);
	vertexData.push_back(color.g);
	vertexData.push_back(color.b);
	vertexData.push_back(color.a);

	vertexData.push_back(position.x);
	vertexData.push_back(position.y + size.y);
	vertexData.push_back(color.r);
	vertexData.push_back(color.g);
	vertexData.push_back(color.b);
	vertexData.push_back(color.a);

	vertexData.push_back(position.x);
	vertexData.push_back(position.y);
	vertexData.push_back(color.r);
	vertexData.push_back(color.g);
	vertexData.push_back(color.b);
	vertexData.push_back(color.a);
}
}