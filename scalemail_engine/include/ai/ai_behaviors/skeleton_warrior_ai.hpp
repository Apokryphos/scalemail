#pragma once

#include "ai/ai_behavior.hpp"
#include "ai/ai_tree.hpp"
#include <glm/vec2.hpp>

namespace ScaleMail
{
class SkeletonWarriorAi : public AiBehavior
{
	AiTree mAiTree;

public:
	SkeletonWarriorAi(Entity entity);
	virtual void think(World& world, double totalElapsedSeconds) override;
};
}