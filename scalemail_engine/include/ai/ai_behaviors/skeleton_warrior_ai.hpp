#pragma once

#include "ai_behavior.hpp"
#include "ai/ai_tree.hpp"
#include <glm/vec2.hpp>

namespace ScaleMail
{
class SkeletonWarriorAi : public AiBehavior
{
	Entity mTargetEntity;
	glm::vec2 mMoveDirection;
	AiTree mAiTree;

public:
	SkeletonWarriorAi(Entity entity);
	virtual void think(World& world, float elapsedSeconds) override;
};
}