#pragma once

#include "gl_headers.hpp"
#include <vector>

namespace ScaleMail
{
struct Mesh {
	GLuint vao;
	GLuint vbo;
	unsigned int elementCount;
	unsigned int vertexCount;
	size_t vertexBufferSize;
};

static const unsigned int COLOR_QUAD_ELEMENT_COUNT = 6;
static const unsigned int LINE_MESH_ELEMENT_COUNT = 6;
static const unsigned int QUAD_MESH_ELEMENT_COUNT = 8;

bool initColorQuadMesh(Mesh& mesh);
bool initPositionColorMesh(Mesh& mesh, size_t vertexCapacity);
bool initQuadMesh(Mesh& mesh);
bool initLineMesh(Mesh& mesh, const std::vector<float>& vertexData);
void updateMesh(Mesh& mesh, const std::vector<float>& vertexData);
}