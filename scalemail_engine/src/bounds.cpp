#include "bounds.hpp"
#include <limits>

namespace ScaleMail
{
//	============================================================================
float side (const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

//	============================================================================
bool triangleContainsPoint(const glm::vec2& a, const glm::vec2& b,
						   const glm::vec2& c, const glm::vec2& point) {
	bool b1 = side(point, a, b) < 0.0f;
	bool b2 = side(point, b, c) < 0.0f;
	bool b3 = side(point, c, a) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

//	============================================================================
Bounds::Bounds() : mPolygon(0, 0, 0, 0) {
}

//	============================================================================
Bounds::Bounds(float x, float y, float width, float height)
: mRectangle(x, y, width, height), mPolygon(x, y, width, height) {
}

//	============================================================================
Bounds::Bounds(const Polygon& polygon) : mPolygon(polygon) {
	float x1 = std::numeric_limits<float>::max();
	float y1 = std::numeric_limits<float>::max();
	float x2 = std::numeric_limits<float>::min();
	float y2 = std::numeric_limits<float>::min();

	for (const auto& p : polygon.getPoints()) {
		x1 = std::min(x1, p.x);
		x2 = std::max(x2, p.x);

		y1 = std::min(y1, p.y);
		y2 = std::max(y2, p.y);
	}

	const float width = x2 - x1;
	const float height = y2 - y1;

	assert(width >= 0);
	assert(height >= 0);

	mRectangle = Rectangle(x1, y1, width, height);
}

//	============================================================================
bool Bounds::contains(const glm::vec2& point) const {
	if (!mRectangle.contains(point)) {
		return false;
	}

	const auto& points = mPolygon.getPoints();
	const auto& indices = mPolygon.getIndices();

	size_t count = indices.size();
	for (size_t n = 0; n < count; n += 3) {
		const auto a = points[indices[n]];
		const auto b = points[indices[n + 1]];
		const auto c = points[indices[n + 2]];

		if (triangleContainsPoint(a, b, c, point)) {
			return true;
		}
	}

	return false;
}

//	============================================================================
const Polygon& Bounds::getPolygon() const {
	return mPolygon;
}

//	============================================================================
const Rectangle& Bounds::getRectangle() const {
	return mRectangle;
}

//	============================================================================
bool Bounds::intersects(const Rectangle& rect) const {
	return mRectangle.intersects(rect);
}
}