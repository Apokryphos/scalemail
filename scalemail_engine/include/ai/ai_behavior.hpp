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
	AiBehavior(const AiBehavior&) = delete;
	AiBehavior& operator=(const AiBehavior&) = delete;

	virtual ~AiBehavior() {}

	Entity getEntity() {
		return mEntity;
	}

	virtual void think(World& world, double totalElapsedSeconds) = 0;
};
}