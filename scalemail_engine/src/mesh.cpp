#include "mesh.hpp"
#include <cassert>
#include <vector>

static const struct
{
	float x, y;
	float r, g, b, a;
} colorQuadVertices[6] = {
	{  1.0f, -1.0f, 1.f, 1.f, 1.f, 1.f },
	{  1.0f,  1.0f, 1.f, 1.f, 1.f, 1.f },
	{ -1.0f, -1.0f, 1.f, 1.f, 1.f, 1.f },
	{  1.0f,  1.0f, 1.f, 1.f, 1.f, 1.f },
	{ -1.0f,  1.0f, 1.f, 1.f, 1.f, 1.f },
	{ -1.0f, -1.0f, 1.f, 1.f, 1.f, 1.f }
};

static const struct
{
	float x, y;
	float r, g, b, a;
	float u, v;
} quadVertices[6] = {
	{  1.0f, -1.0f, 1.f, 1.f, 1.f, 1.f, 1.0f, 0.0f },
	{  1.0f,  1.0f, 1.f, 1.f, 1.f, 1.f, 1.0f, 1.0f },
	{ -1.0f, -1.0f, 1.f, 1.f, 1.f, 1.f, 0.0f, 0.0f },
	{  1.0f,  1.0f, 1.f, 1.f, 1.f, 1.f, 1.0f, 1.0f },
	{ -1.0f,  1.0f, 1.f, 1.f, 1.f, 1.f, 0.0f, 1.0f },
	{ -1.0f, -1.0f, 1.f, 1.f, 1.f, 1.f, 0.0f, 0.0f }
};

static const unsigned int LINE_MESH_ELEMENT_COUNT = 6;

namespace ScaleMail
{
//  ============================================================================
bool initColorQuadMesh(Mesh& mesh) {
	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(1, &mesh.vbo);

	mesh.vertexCount = 6;

	glBindVertexArray(mesh.vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorQuadVertices), colorQuadVertices,
				 GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
						  sizeof(float) * COLOR_QUAD_ELEMENT_COUNT, (void*) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
						  sizeof(float) * COLOR_QUAD_ELEMENT_COUNT,
						  (void*) (sizeof(float) * 2));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

//  ============================================================================
bool initQuadMesh(Mesh& mesh) {
	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(1, &mesh.vbo);

	mesh.vertexCount = 6;

	glBindVertexArray(mesh.vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
				 GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
						  sizeof(float) * 8, (void*) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
						  sizeof(float) * 8, (void*) (sizeof(float) * 2));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
						  sizeof(float) * 8, (void*) (sizeof(float) * 6));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

//  ============================================================================
bool initLineMesh(Mesh& mesh, const std::vector<float>& vertexData) {
	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(1, &mesh.vbo);

	mesh.vertexCount = 0;

	glBindVertexArray(mesh.vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	if (vertexData.size() > 0) {
		mesh.vertexCount = vertexData.size() / LINE_MESH_ELEMENT_COUNT;

		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
					&vertexData[0], GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
						  sizeof(float) * LINE_MESH_ELEMENT_COUNT, (void*) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
						  sizeof(float) * LINE_MESH_ELEMENT_COUNT,
						  (void*) (sizeof(float) * 2));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

//  ============================================================================
bool updateLineMesh(Mesh& mesh, const std::vector<float>& vertexData) {
	assert(vertexData.size() > 0);

	mesh.vertexCount = vertexData.size() / LINE_MESH_ELEMENT_COUNT;

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
}
