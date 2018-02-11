#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace ScaleMail
{
struct Rectangle
{
	float x;
	float y;
	float width;
	float height;

	Rectangle(float x = 0, float y = 0, float width = 0, float height = 0) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	Rectangle(glm::vec4 rect) {
		this->x = rect.x;
		this->y = rect.y;
		this->width = rect.z;
		this->height = rect.w;
	}

	bool contains(glm::vec2 point) const {
		return
			point.x >= x &&
			point.x <= x + width &&
			point.y >= y &&
			point.y <= y + height;
	}

	float getBottom() const  {
		return y + height;
	}

	float getCenterX() const  {
		return x + width / 2;
	}

	float getCenterY() const  {
		return y + height / 2;
	}

	float getLeft() const  {
		return x;
	}

	glm::vec2 getPosition() const  {
		return glm::vec2(x, y);
	}

	float getRight() const  {
		return x + width;
	}

	glm::vec2 getSize() const  {
		return glm::vec2(width, height);
	}

	float getTop() const  {
		return y;
	}
};
}