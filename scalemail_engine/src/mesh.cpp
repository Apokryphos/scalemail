#include "mesh.hpp"
#include "render_options.hpp"
#include "vertex_attrib.hpp"
#include "vertex_definition.hpp"
#include <cassert>
#include <iostream>
#include <vector>

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

namespace ScaleMail
{
//  ============================================================================
void disableMeshVertexAttribPointers(const Mesh& mesh) {
	if (mesh.vao != 0) {
		glBindVertexArray(0);
	} else {
		disableVertexAttributes(mesh.vertexDefinition);
	}
}

//  ============================================================================
void enableMeshVertexAttribPointers(const Mesh& mesh) {
	if (mesh.vao != 0) {
		glBindVertexArray(mesh.vao);
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
		enableVertexAttributes(mesh.vertexDefinition);
		setVertexAttributePointers(mesh.vertexDefinition);
	}
}

//  ============================================================================
void drawMesh(const Mesh& mesh) {
	enableMeshVertexAttribPointers(mesh);
	glDrawArrays(mesh.primitive, 0, mesh.vertexCount);
	disableMeshVertexAttribPointers(mesh);
	glBindVertexArray(0);
}

//  ============================================================================
bool initMesh(Mesh& mesh, const VertexDefinition vertexDefinition,
			  const RenderOptions& renderOptions,
			  size_t vertexCapacity) {
	if (renderOptions.vaoSupported) {
		glGenVertexArrays(1, &mesh.vao);
	} else {
		mesh.vao = 0;
	}

	glGenBuffers(1, &mesh.vbo);

	const int elementCount = getVertexDefinitionElementCount(vertexDefinition);

	const size_t bufferSize = vertexCapacity * elementCount * sizeof(float);

	mesh.elementCount = elementCount;
	mesh.primitive = GL_TRIANGLES;
	mesh.vertexCount = 0;
	mesh.vertexBufferSize = bufferSize;
	mesh.vertexDefinition = vertexDefinition;

	if (renderOptions.vaoSupported) {
		glBindVertexArray(mesh.vao);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);

	if (renderOptions.vaoSupported) {
		enableVertexAttributes(vertexDefinition);
		setVertexAttributePointers(vertexDefinition);
		glBindVertexArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

//  ============================================================================
// bool initPositionColorMesh(Mesh& mesh, size_t vertexCapacity) {
// 	glGenVertexArrays(1, &mesh.vao);
// 	glGenBuffers(1, &mesh.vbo);

// 	const unsigned int ELEMENT_COUNT = 7;

// 	mesh.elementCount = ELEMENT_COUNT;
// 	mesh.primitive = GL_TRIANGLES;
// 	mesh.vertexDefinition = VertexDefinition::POSITION3_COLOR4;
// 	mesh.vertexCount = 0;
// 	mesh.vertexBufferSize = vertexCapacity * ELEMENT_COUNT;

// 	glBindVertexArray(mesh.vao);

// 	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
// 	glBufferData(GL_ARRAY_BUFFER, mesh.vertexBufferSize, NULL, GL_STATIC_DRAW);

// 	glEnableVertexAttribArray(0);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
// 						  sizeof(float) * ELEMENT_COUNT, (void*) 0);

// 	glEnableVertexAttribArray(1);
// 	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
// 						  sizeof(float) * ELEMENT_COUNT,
// 						  (void*) (sizeof(float) * 3));

// 	glBindVertexArray(0);
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);

// 	return true;
// }

//  ============================================================================
bool initQuadMesh(Mesh& mesh) {
	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(1, &mesh.vbo);

	mesh.elementCount = QUAD_MESH_ELEMENT_COUNT;
	mesh.primitive = GL_TRIANGLES;
	mesh.vertexCount = 6;
	mesh.vertexBufferSize = sizeof(quadVertices);
	mesh.vertexDefinition = VertexDefinition::POSITION2_COLOR4_TEXTURE2;

	glBindVertexArray(mesh.vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
				 GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
						  sizeof(float) * QUAD_MESH_ELEMENT_COUNT, (void*) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
						  sizeof(float) * QUAD_MESH_ELEMENT_COUNT,
						  (void*) (sizeof(float) * 2));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
						  sizeof(float) * QUAD_MESH_ELEMENT_COUNT,
						  (void*) (sizeof(float) * 6));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

//  ============================================================================
bool initLineMesh(Mesh& mesh, const std::vector<float>& vertexData) {
	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(1, &mesh.vbo);

	mesh.elementCount = LINE_MESH_ELEMENT_COUNT;
	mesh.primitive = GL_TRIANGLES;
	mesh.vertexCount = 0;
	mesh.vertexBufferSize = 0;
	mesh.vertexDefinition = VertexDefinition::POSITION2_COLOR4;

	glBindVertexArray(mesh.vao);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	if (vertexData.size() > 0) {
		mesh.vertexCount = vertexData.size() / LINE_MESH_ELEMENT_COUNT;
		mesh.vertexBufferSize = vertexData.size();

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
void setMeshVertexData(Mesh& mesh, const std::vector<float>& vertexData) {
	if (vertexData.size() == 0) {
		mesh.vertexCount = 0;
		return;
	}

	assert(vertexData.size() > 0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	const size_t bufferSize = vertexData.size() * sizeof(float);

	if (bufferSize > mesh.vertexBufferSize) {
		glBufferData(GL_ARRAY_BUFFER, bufferSize,
					 &vertexData[0], GL_STATIC_DRAW);

		mesh.vertexBufferSize = bufferSize;

		std::cout << "Mesh buffer reallocated." << std::endl;
	} else {
		glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			bufferSize,
			&vertexData[0]);
	}

	mesh.vertexCount = vertexData.size() / mesh.elementCount;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
}
