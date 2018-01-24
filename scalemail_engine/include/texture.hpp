#pragma once

#include "gl_headers.hpp"
#include <string>

namespace ScaleMail
{
struct Texture {
    GLuint id;
    int width;
    int height;
};

bool loadPngTexture(const std::string& filename, Texture& texture);
}