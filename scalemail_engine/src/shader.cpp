#include "gl_headers.hpp"
#include "gl_util.hpp"
#include <iostream>
#include <fstream>
#include <string>

namespace ScaleMail
{
//	============================================================================
bool loadShaderSource(const std::string& filename, std::string& source) {
	std::ifstream file(filename);

	if (file.is_open() == false) {
		std::cerr << "Could not open shader source file " << filename << std::endl;
		return false;
	}

	std::string str;
	while (std::getline(file, str)) {
		source += str;
		source.push_back('\n');
	}

	return true;
}

//	============================================================================
bool loadAndCompileShader(
	const GLuint shader, const std::string& sourceFilename) {
	std::string sourceString;
	if (loadShaderSource(sourceFilename, sourceString) == false) {
		return false;
	}

	const char* source = sourceString.c_str();

	glShaderSource(shader, 1, &source, NULL);
	if (glErrorExists()) {
		std::cerr << "Could not load shader " << sourceFilename << std::endl;
		return false;
	}

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		std::cerr << "Error compiling shader program " << sourceFilename << std::endl;
		std::cerr << buffer << std::endl;
		return false;
	}

	return true;
}

//	============================================================================
bool initShaderProgram(
	const std::string& vertexShaderFilename,
	const std::string& fragmentShaderFilename,
	GLuint& program) {
	program = glCreateProgram();

	if (program == 0 || glErrorExists()) {
		std::cerr << "Error creating shader program." << std::endl;
		return false;
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	if (vertexShader == 0) {
		std::cerr << "Error creating vertex shader." << std::endl;
		return false;
	}

	if (loadAndCompileShader(vertexShader, vertexShaderFilename) == false) {
		return false;
	}

	glAttachShader(program, vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	if (fragmentShader == 0) {
		std::cerr << "Error creating fragment shader." << std::endl;
		return false;
	}

	if (loadAndCompileShader(fragmentShader, fragmentShaderFilename) == false) {
		return false;
	}

	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetProgramInfoLog(program, 512, NULL, buffer);
		std::cerr << "Error linking shader program." << std::endl;
		std::cerr << buffer << std::endl;
		return false;
	}

	glDetachShader(program, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(fragmentShader);

	return true;
}
}