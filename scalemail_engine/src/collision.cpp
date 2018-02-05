#include <glm/vec2.hpp>

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
glm::vec2 collisionPoint(glm::vec2 position1, float radius1, glm::vec2 position2,
			   		float radius2) {
	float x =
		(position1.x * radius2 + position2.x * radius1) / (radius1 + radius2);

	float y =
		(position1.y * radius2 + position2.y * radius1) / (radius1 + radius2);

	return glm::vec2(x, y);
}
}