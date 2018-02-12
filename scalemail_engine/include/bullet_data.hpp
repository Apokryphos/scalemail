#pragma once

#include <glm/vec2.hpp>

namespace ScaleMail
{
struct BulletData
{
	int impactTilesetId;
	int tilesetId;
	float life;
	float speed;
	glm::vec2 direction;
	glm::vec2 target;
};
}