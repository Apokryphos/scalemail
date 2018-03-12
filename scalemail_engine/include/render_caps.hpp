#pragma once

#include "shader_version.hpp"

namespace ScaleMail
{
struct RenderCaps
{
	ShaderVersion shaderVersion;
	bool fboSupported;
	bool vaoSupported;
};
}