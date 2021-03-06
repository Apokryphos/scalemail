#pragma once

#include "collision_group.hpp"
#include "entity.hpp"
#include <glm/vec2.hpp>

namespace ScaleMail
{
struct StaticCollision
{
	bool ignore;
	CollisionGroup sourceGroup;
	Entity sourceEntity;
	glm::vec2 velocity;
};
}