#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <cmath>

namespace ScaleMail
{
//  ============================================================================
bool collision(glm::vec2 position1, float radius1, glm::vec2 position2,
			   float radius2) {
	return
		(((position2.x - position1.x) * (position2.x - position1.x)) +
		 ((position2.y - position1.y) * (position2.y - position1.y))) <=
		 ((radius1 + radius2) * (radius1 + radius2));
}

//  ============================================================================
bool circleIntersectsRectangle(glm::vec2 center, float radius, glm::vec4 rect) {
	float w = rect.z / 2;
	float h = rect.w / 2;

	float x = abs(center.x - (rect.x + w));
	float y = abs(center.y - (rect.y + h));

	if (x > w + radius) {
		return false;
	}

	if (y > h + radius) {
		return false;
	}

	if (x <= w) {
		return true;
	}

	if (y <= h) {
		return true;
	}

	float dx = x - w;
	float dy = y - h;
	float cornerDistance = (dx * dx) + (dy * dy);

	return cornerDistance <= radius * radius;
}

//  ============================================================================
glm::vec2 collisionPoint(glm::vec2 position1, float radius1, glm::vec2 position2,
						 float radius2) {
	float x =
		(position1.x * radius2 + position2.x * radius1) / (radius1 + radius2);

	float y =
		(position1.y * radius2 + position2.y * radius1) / (radius1 + radius2);

	return glm::vec2(x, y);
}
}