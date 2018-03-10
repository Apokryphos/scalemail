#include "gl_headers.hpp"
#include "lodepng.h"
#include "texture.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace ScaleMail
{
//  ===========================================================================
void flipTexture(unsigned char* image, const int width, const int height)
{
	unsigned char *imagePtr = &image[0];
	int halfHeight = height / 2;

	// RGBA is 4 components per pixel
	int numColorComponents = 4;

	int widthInBytes = width * numColorComponents;

	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	for (int h = 0; h < halfHeight; ++h)
	{
		top = imagePtr + h * widthInBytes;
		bottom = imagePtr + (height - h - 1) * widthInBytes;
		for (int w = 0; w < widthInBytes; ++w)
		{
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			++top;
			++bottom;
		}
	}
}

//  ============================================================================
bool loadPngTexture(const std::string& filename, Texture& texture) {
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);
	if (error != 0)	{
		std::cerr << "LodePNG Error " << error << ": "
				  << lodepng_error_text(error) << std::endl;
		return false;
	}

	// flipTexture(&image[0], width, height);

	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &texture.id);
	texture.width = width;
	texture.height = height;

	glBindTexture(GL_TEXTURE_2D, texture.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_SRGB_ALPHA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		&image[0]);

	//	OpenGL Version 3.0 onwards
	// glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
}