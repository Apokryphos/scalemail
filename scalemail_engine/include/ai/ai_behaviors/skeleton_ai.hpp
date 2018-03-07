#pragma once

#include "ai_behavior.hpp"
#include <glm/vec2.hpp>

namespace ScaleMail
{
class SkeletonAi : public AiBehavior
{
	Entity mTargetEntity;
	glm::vec2 mMoveDirection;

public:
	SkeletonAi(Entity entity);
	virtual void think(World& world, float elapsedSeconds) override;
};
}