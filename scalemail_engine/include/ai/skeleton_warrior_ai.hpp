#pragma once

#include "ai_behavior.hpp"
#include <glm/vec2.hpp>

namespace ScaleMail
{
class SkeletonWarriorAi : public AiBehavior
{
	Entity mTargetEntity;
	glm::vec2 mMoveDirection;

public:
	SkeletonWarriorAi();
	virtual void think(World& world, float elapsedSeconds) override;
};
}