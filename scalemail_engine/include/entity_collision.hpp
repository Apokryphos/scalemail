#pragma once

#include "collision_group.hpp"
#include "entity.hpp"
#include <glm/vec2.hpp>

namespace ScaleMail
{
struct EntityCollision
{
	bool ignore;
	float distance;
	CollisionGroup sourceGroup;
	CollisionGroup targetGroup;
	Entity sourceEntity;
	Entity targetEntity;
	glm::vec2 velocity;
};
}