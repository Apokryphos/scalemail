#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
class Polygon
{
	std::vector<uint16_t> mIndices;
	std::vector<glm::vec2> mPoints;

public:
	Polygon(const float x, const float y, const float width,
			const float height);
	Polygon(const std::vector<glm::vec2>& points);
	const std::vector<uint16_t>& getIndices() const;
	const std::vector<glm::vec2>& getPoints() const;
	void initialize(const std::vector<glm::vec2>& points);
};
}