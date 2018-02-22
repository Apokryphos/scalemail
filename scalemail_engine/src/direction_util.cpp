#include "direction.hpp"
#include "math_util.hpp"
#include "string_util.hpp"
#include <glm/vec2.hpp>
#include <string>

namespace ScaleMail
{
Direction stringToDirection(std::string value) {
	value = toLowercase(value);

	if (value == "south") {
		return Direction::SOUTH;
	} else if (value == "north") {
		return Direction::NORTH;
	} else if (value == "east") {
		return Direction::EAST;
	} else if (value == "west") {
		return Direction::WEST;
	}

	return Direction::SOUTH;
}

Direction vec2ToDirection(const glm::vec2& direction) {
	const int quadrant = toQuadrant(direction);

	switch (quadrant) {
		case 0:
			return Direction::EAST;
		case 1:
			return Direction::SOUTH;
		case 2:
			return Direction::WEST;
		case 3:
			return Direction::NORTH;
		default:
			return Direction::SOUTH;
	}
}
}