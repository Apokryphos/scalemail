#pragma once

#include "gl_headers.hpp"

namespace ScaleMail
{
struct Mesh {
    GLuint vao;
    GLuint vbo;
    int vertexCount;
};

bool initQuadMesh(Mesh& mesh);
}