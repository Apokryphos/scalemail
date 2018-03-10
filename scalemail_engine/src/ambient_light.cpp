#include "ambient_light.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "vertex_data.hpp"
#include "world.hpp"
#include <glm/vec4.hpp>
#include <iostream>
#include <vector>

namespace ScaleMail
{
static const unsigned int LIGHT_MESH_ELEMENT_COUNT = COLOR_QUAD_ELEMENT_COUNT;

static Mesh ambientLightMesh;
static std::vector<float> ambientLightVertexData;

static std::vector<AmbientLight> ambientLights;

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
	ambientLightVertexData.resize(0);
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