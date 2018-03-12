#include "mesh.hpp"
#include "render_options.hpp"
#include "vertex_attrib.hpp"
#include "vertex_definition.hpp"
#include <cassert>
#include <iostream>
#include <vector>

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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//  ============================================================================
bool initMesh(Mesh& mesh, const VertexDefinition vertexDefinition,
			  bool vaoSupported,
			  size_t vertexCapacity,
			  GLenum primitive) {
	if (vaoSupported) {
		glGenVertexArrays(1, &mesh.vao);
	} else {
		mesh.vao = 0;
	}

	glGenBuffers(1, &mesh.vbo);

	const int elementCount = getVertexDefinitionElementCount(vertexDefinition);

	const size_t bufferSize = vertexCapacity * elementCount * sizeof(float);

	mesh.elementCount = elementCount;
	mesh.primitive = primitive;
	mesh.vertexCount = 0;
	mesh.vertexBufferSize = bufferSize;
	mesh.vertexDefinition = vertexDefinition;

	if (vaoSupported) {
		glBindVertexArray(mesh.vao);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);

	if (vaoSupported) {
		enableVertexAttributes(vertexDefinition);
		setVertexAttributePointers(vertexDefinition);
		glBindVertexArray(0);
	}

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
