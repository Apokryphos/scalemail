#include <glm/gtx/transform.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <cmath>
#include <vector>

namespace ScaleMail
{
static const glm::vec3 gQuadVertices[] = {
	glm::vec3(-0.5f, -0.5f, 1.0f),
	glm::vec3(-0.5f,  0.5f, 0.0f),
	glm::vec3( 0.5f,  0.5f, 0.0f),
	glm::vec3( 0.5f, -0.5f, 1.0f),
};

//  ============================================================================
void addTileVertexData(std::vector<float>& meshVertexData,
					   const glm::vec3 position, const glm::vec3 size,
					   const glm::vec2 uv1, const glm::vec2 uv2,
					   float rotate) {
	float u1 = uv1.x;
	float v1 = uv1.y;
	float u2 = uv2.x;
	float v2 = uv2.y;

	glm::mat3 rotation =
		glm::rotate(rotate, glm::vec3(0.0f, 0.0f, 1.0f));

	// float sinAngle = std::sin(rotate);
	// float cosAngle = std::cos(rotate);
	// glm::mat2 rotation = glm::mat2(
	// 	cosAngle, -sinAngle,
	// 	sinAngle,  cosAngle);

	glm::vec3 offset = glm::vec3(8.f, 8.0f, 0.0f);

	glm::vec3 quadA = position + offset + rotation * gQuadVertices[0] * size;
	glm::vec3 quadB = position + offset + rotation * gQuadVertices[1] * size;
	glm::vec3 quadC = position + offset + rotation * gQuadVertices[2] * size;
	glm::vec3 quadD = position + offset + rotation * gQuadVertices[3] * size;

	meshVertexData.push_back(quadA.x);
	meshVertexData.push_back(quadA.y);
	meshVertexData.push_back(quadA.z);
	meshVertexData.push_back(u1);
	meshVertexData.push_back(v1);

	meshVertexData.push_back(quadB.x);
	meshVertexData.push_back(quadB.y);
	meshVertexData.push_back(quadB.z);
	meshVertexData.push_back(u1);
	meshVertexData.push_back(v2);

	meshVertexData.push_back(quadC.x);
	meshVertexData.push_back(quadC.y);
	meshVertexData.push_back(quadC.z);
	meshVertexData.push_back(u2);
	meshVertexData.push_back(v2);

	meshVertexData.push_back(quadA.x);
	meshVertexData.push_back(quadA.y);
	meshVertexData.push_back(quadA.z);
	meshVertexData.push_back(u1);
	meshVertexData.push_back(v1);

	meshVertexData.push_back(quadC.x);
	meshVertexData.push_back(quadC.y);
	meshVertexData.push_back(quadC.z);
	meshVertexData.push_back(u2);
	meshVertexData.push_back(v2);

	meshVertexData.push_back(quadD.x);
	meshVertexData.push_back(quadD.y);
	meshVertexData.push_back(quadD.z);
	meshVertexData.push_back(u2);
	meshVertexData.push_back(v1);
}
}