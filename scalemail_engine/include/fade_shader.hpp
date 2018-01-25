#pragma once

#include "gl_headers.hpp"

namespace ScaleMail
{
struct FadeShader {
    GLuint id;
    GLuint fadeColorLocation;
    GLuint fadeProgressLocation;
};
}