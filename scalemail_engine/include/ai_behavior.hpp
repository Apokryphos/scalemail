#pragma once

#include "entity.hpp"

namespace ScaleMail
{
class World;

class AiBehavior
{
	Entity mEntity;

public:
	AiBehavior(Entity entity) : mEntity(entity) {}

	virtual ~AiBehavior() {}

	Entity getEntity() {
		return mEntity;
	}

	virtual void think(World& world, float elapsedSeconds) = 0;
};
}