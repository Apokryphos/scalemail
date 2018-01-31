#pragma once

#include "gl_headers.hpp"
#include <string>

namespace ScaleMail
{
bool initShaderProgram(
	const std::string& vertexShaderFilename,
	const std::string& fragShaderFilename,
	GLuint& program);
}