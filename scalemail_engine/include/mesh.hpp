#pragma once

#include "gl_headers.hpp"
#include <vector>

namespace ScaleMail
{
struct Mesh {
	GLuint vao;
	GLuint vbo;
	int vertexCount;
};

bool initQuadMesh(Mesh& mesh);
bool initLineMesh(Mesh& mesh, const std::vector<float>& vertexData);
bool updateLineMesh(Mesh& mesh, const std::vector<float>& vertexData);
}