#pragma once

#include "polygon.hpp"
#include "rectangle.hpp"

namespace ScaleMail
{
class Bounds
{
	Rectangle mRectangle;
	Polygon mPolygon;

public:
	Bounds();
	Bounds(float x, float y, float width, float height);
	Bounds(const Polygon& polygon);
	bool contains(const glm::vec2& point) const;
	const Polygon& getPolygon() const;
	const Rectangle& getRectangle() const;
	bool intersects(const Rectangle& rect) const;
};
}