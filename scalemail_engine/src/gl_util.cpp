#include "gl_headers.hpp"
#include <iostream>

namespace ScaleMail
{
//	============================================================================
bool glErrorExists()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "An error occured during an OpenGL operation." << std::endl;

		switch (error)
		{
			case GL_NO_ERROR:
				std::cerr << "NO_ERROR" << std::endl;
				break;
			case GL_INVALID_ENUM:
				std::cerr << "INVALID_ENUM" << std::endl;
				break;
			case GL_INVALID_VALUE:
				std::cerr << "INVALID_VALUE" << std::endl;
				break;
			case GL_INVALID_OPERATION:
				std::cerr << "INVALID_OPERATION" << std::endl;
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				std::cerr << "INVALID_FRAMEBUFFER_OPERATION" << std::endl;
				break;
			case GL_OUT_OF_MEMORY:
				std::cerr << "OUT_OF_MEMORY" << std::endl;
				break;
			case GL_CONTEXT_LOST:
				std::cerr << "CONTEXT_LOST" << std::endl;
				break;
			default:
				std::cerr << "Unknown" << std::endl;
				break;
		}

		return true;
	}

	return false;
}
}