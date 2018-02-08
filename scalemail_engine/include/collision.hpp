#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace ScaleMail
{
bool collision(glm::vec2 position1, float radius1, glm::vec2 position2,
			   float radius2);
glm::vec2 collisionPoint(glm::vec2 position1, float radius1, glm::vec2 position2,
	float radius2);
bool circleIntersectsRectangle(glm::vec2 center, float radius, glm::vec4 rect);
}