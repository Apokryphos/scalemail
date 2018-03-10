#pragma once

#include "shader_version.hpp"

namespace ScaleMail
{
struct RenderOptions
{
	ShaderVersion shaderVersion;
	bool fboSupported;
	bool vaoSupported;
	bool lightsEnabled;
};
}