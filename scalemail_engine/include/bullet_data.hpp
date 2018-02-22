#pragma once

#include <glm/vec2.hpp>

namespace ScaleMail
{
struct BulletData
{
	int impactTilesetId;
	int tilesetId;
	float damage;
	float life;
	float rotateOffset;
	float speed;
	glm::vec2 direction;
};
}