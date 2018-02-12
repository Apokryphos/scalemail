#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace ScaleMail
{
struct LightData
{
	float pulse;
	float pulseSize;
	glm::vec2 glowSize;
	glm::vec2 size;
	glm::vec4 color;
};
}