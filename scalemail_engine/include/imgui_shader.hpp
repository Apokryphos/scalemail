#pragma once

#include "gl_headers.hpp"

namespace ScaleMail
{
struct ImGuiShader {
	GLuint id;
	GLuint colorLocation;
	GLuint positionLocation;
	GLuint projectionLocation;
	GLuint textureLocation;
	GLuint uvLocation;
};
}