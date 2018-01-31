#include "direction.hpp"
#include "string_util.hpp"
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
}