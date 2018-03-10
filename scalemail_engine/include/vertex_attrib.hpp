#pragma once

#include "gl_headers.hpp"
#include "vertex_definition.hpp"

namespace ScaleMail
{
void disableVertexAttributes(const VertexDefinition def);
void enableVertexAttributes(const VertexDefinition def);
void setVertexAttributePointers(const VertexDefinition def);

//	============================================================================
inline void floatVertexAttribPointer(GLuint index, GLint size,
									 int elementCount, size_t offset) {
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE,
						  sizeof(GLfloat) * elementCount,
						  (void*)(sizeof(float) * offset));
}
}