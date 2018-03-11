#pragma once

#include "gl_headers.hpp"
#include "vertex_definition.hpp"
#include <vector>

namespace ScaleMail
{
struct RenderOptions;

struct Mesh {
	VertexDefinition vertexDefinition;
	GLenum primitive;
	GLuint vao;
	GLuint vbo;
	unsigned int elementCount;
	unsigned int vertexCount;
	size_t vertexBufferSize;
};

static const unsigned int COLOR_QUAD_ELEMENT_COUNT = 6;
static const unsigned int LINE_MESH_ELEMENT_COUNT = 6;
static const unsigned int QUAD_MESH_ELEMENT_COUNT = 8;

void drawMesh(const Mesh& mesh);
bool initMesh(Mesh& mesh, const VertexDefinition vertexDefinition,
			  const RenderOptions& renderOptions,
			  size_t vertexCapacity = 6);
bool initQuadMesh(Mesh& mesh);
bool initLineMesh(Mesh& mesh, const std::vector<float>& vertexData);
void setMeshVertexData(Mesh& mesh, const std::vector<float>& vertexData);
}