#pragma once

#include "gl_headers.hpp"
#include <vector>

namespace ScaleMail
{
struct Mesh {
	GLuint vao;
	GLuint vbo;
	unsigned int vertexCount;
};

static const unsigned int COLOR_QUAD_ELEMENT_COUNT = 6;

bool initColorQuadMesh(Mesh& mesh);
bool initQuadMesh(Mesh& mesh);
bool initLineMesh(Mesh& mesh, const std::vector<float>& vertexData);
bool updateLineMesh(Mesh& mesh, const std::vector<float>& vertexData);
}