#include "camera.hpp"
#include "mesh.hpp"
#include "vertex_data.hpp"
#include "world.hpp"
#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
static const struct
{
	float x, y;
	float r, g, b, a;
} quadVertices[6] = {
	{  1.0f, -1.0f, 1.f, 1.f, 1.f, 1.f },
	{  1.0f,  1.0f, 1.f, 1.f, 1.f, 1.f },
	{ -1.0f, -1.0f, 1.f, 1.f, 1.f, 1.f },
	{  1.0f,  1.0f, 1.f, 1.f, 1.f, 1.f },
	{ -1.0f,  1.0f, 1.f, 1.f, 1.f, 1.f },
	{ -1.0f, -1.0f, 1.f, 1.f, 1.f, 1.f }
};

struct AmbientLight
{
	glm::vec4 color;
	glm::vec4 rect;
};

static const unsigned int LIGHT_MESH_ELEMENT_COUNT = 6;

static Mesh ambientLightMesh;
static std::vector<float> ambientLightVertexData;

static std::vector<AmbientLight> ambientLights;

//  ============================================================================
static bool initializeMesh(Mesh& mesh) {
	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(1, &mesh.vbo);

	mesh.vertexCount = 0;

	glBindVertexArray(mesh.vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
						  sizeof(float) * LIGHT_MESH_ELEMENT_COUNT, (void*) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
						  sizeof(float) * LIGHT_MESH_ELEMENT_COUNT,
						  (void*) (sizeof(float) * 2));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

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
	initializeMesh(ambientLightMesh);
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