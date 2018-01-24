#include "mesh.hpp"

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
}
