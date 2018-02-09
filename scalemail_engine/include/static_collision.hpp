#pragma once

#include "collision_group.hpp"
#include "entity.hpp"

namespace ScaleMail
{
struct StaticCollision
{
	Entity sourceEntity;
	CollisionGroup sourceGroup;
};
}