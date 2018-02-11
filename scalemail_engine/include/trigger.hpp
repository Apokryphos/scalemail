#pragma once

#include "entity.hpp"
#include <glm/vec4.hpp>

namespace ScaleMail
{
struct Trigger
{
	Entity entity;
	glm::vec4 rect;
};
}