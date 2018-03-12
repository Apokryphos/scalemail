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

void disableMeshVertexAttribPointers(const Mesh& mesh);
void drawMesh(const Mesh& mesh);
void enableMeshVertexAttribPointers(const Mesh& mesh);
bool initMesh(Mesh& mesh, const VertexDefinition vertexDefinition,
			  bool vaoSupported,
			  size_t vertexCapacity = 6,
			  GLenum primitive = GL_TRIANGLES);
bool initQuadMesh(Mesh& mesh);
void setMeshVertexData(Mesh& mesh, const std::vector<float>& vertexData);
}