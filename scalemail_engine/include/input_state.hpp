#pragma once

#include <glm/vec2.hpp>

namespace ScaleMail
{
struct InputState
{
	bool fire;
	glm::vec2 aimPosition;
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;
	bool capture;
	bool down;
	bool left;
	bool right;
	bool up;
};
}