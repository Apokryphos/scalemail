#include "polygon.hpp"
#include <mapbox/earcut.hpp>

namespace mapbox
{
namespace util
{
template <>
struct nth<0, glm::vec2> {
    inline static auto get(const glm::vec2 &p) {
        return p.x;
    };
};

template <>
struct nth<1, glm::vec2> {
    inline static auto get(const glm::vec2 &p) {
        return p.y;
    };
};
}
}

//	============================================================================
namespace ScaleMail
{
using N = uint16_t;

//	============================================================================
Polygon::Polygon(const std::vector<glm::vec2>& points) {
	this->initialize(points);
}


//	============================================================================
Polygon::Polygon(const glm::vec4& rect) {
	const float x = rect.x;
	const float y = rect.y;
	const float width = rect.z;
	const float height = rect.w;

	std::vector<glm::vec2> points = {
		{ x,         y },
		{ x,         y + height},
		{ x + width, y},
		{ x,         y + height},
		{ x + width, y + height},
		{ x + width, y},
	};

	this->initialize(points);
}

//	============================================================================
const std::vector<uint16_t>& Polygon::getIndices() const {
	return mIndices;
}

//	============================================================================
const std::vector<glm::vec2>& Polygon::getPoints() const {
	return mPoints;
}

//	============================================================================
void Polygon::initialize(const std::vector<glm::vec2>& points) {
	//	First set of points defines the main polygon. Subsequent sets would
	//	define holes.
	std::vector<std::vector<glm::vec2>> polygon;

	polygon.resize(1);
	polygon[0] = points;

	mIndices = mapbox::earcut<N>(polygon);
	mPoints = polygon[0];
}
}