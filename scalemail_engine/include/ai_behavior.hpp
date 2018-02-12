#pragma once

#include "entity.hpp"

namespace ScaleMail
{
class World;

class AiBehavior
{
	Entity mEntity;

public:
	Entity getEntity() {
		return mEntity;
	}

	void setEntity(Entity entity) {
		mEntity = entity;
	}

	virtual void think(World& world, float elapsedSeconds) = 0;
};
}