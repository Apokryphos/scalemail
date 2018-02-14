#include "camera.hpp"
#include "mesh.hpp"
#include "vertex_data.hpp"
#include "world.hpp"
#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
struct AmbientLight
{
	glm::vec4 color;
	glm::vec4 rect;
};

static const unsigned int LIGHT_MESH_ELEMENT_COUNT = COLOR_QUAD_ELEMENT_COUNT;

static Mesh ambientLightMesh;
static std::vector<float> ambientLightVertexData;

static std::vector<AmbientLight> ambientLights;

//  ============================================================================
static bool updateMesh(Mesh& mesh, const std::vector<float>& vertexData) {
	assert(vertexData.size() > 0);

	mesh.vertexCount = vertexData.size() / LIGHT_MESH_ELEMENT_COUNT;

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	if (vertexData.size() > mesh.vertexCount) {
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
					 &vertexData[0], GL_STATIC_DRAW);
	} else {
		glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			vertexData.size() * sizeof(float),
			&vertexData[0]);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

//  ============================================================================
void addAmbientLight(glm::vec4 color, glm::vec4 rect) {
	ambientLights.push_back({ color, rect });
}

//  ============================================================================
void initializeAmbientLights() {
	initColorQuadMesh(ambientLightMesh);
}

//  ============================================================================
void buildAmbientLights() {
	ambientLightVertexData.clear();
	for (const auto& light : ambientLights) {
		glm::vec2 position = glm::vec2(light.rect.x, light.rect.y);
		glm::vec2 size = glm::vec2(light.rect.z, light.rect.w);

		addQuadVertexData(ambientLightVertexData, position, size, light.color);
	}
	updateMesh(ambientLightMesh, ambientLightVertexData);
}

//  ============================================================================
void drawAmbientLights() {
	glBindVertexArray(ambientLightMesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, ambientLightMesh.vertexCount);
}
}