#pragma once

#include "direction.hpp"
#include <glm/vec2.hpp>
#include <string>

namespace ScaleMail
{
Direction stringToDirection(std::string value);
Direction vec2ToDirection(const glm::vec2& direction);
}