#include "gl_headers.hpp"
#include "lodepng.h"
#include <vector>
#include <iostream>

namespace ScaleMail
{
//  ============================================================================
bool loadCursor(GLFWwindow* window) {
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, "assets/textures/cursor.png");
	if (error != 0)	{
		std::cerr << "LodePNG Error " << error << ": "
				  << lodepng_error_text(error) << std::endl;
		return false;
	}

	GLFWimage cursorImage;
	cursorImage.width = 32;
	cursorImage.height = 32;
	cursorImage.pixels = &image[0];

	GLFWcursor* cursor = glfwCreateCursor(&cursorImage, 15, 15);

	if (cursor == NULL) {
		std::cerr << "Failed to create cursor." << std::endl;
		return false;
	}

	glfwSetCursor(window, cursor);

	return true;
}
}