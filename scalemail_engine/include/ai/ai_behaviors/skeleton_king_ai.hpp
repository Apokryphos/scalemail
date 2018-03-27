#pragma once

#include "ai/ai_tree.hpp"
#include "ai/ai_behavior.hpp"
#include <glm/vec2.hpp>

namespace ScaleMail
{
class SkeletonKingAi : public AiBehavior
{
	AiTree mAiTree;

public:
	SkeletonKingAi(Entity entity);
	virtual void think(World& world, double totalElapsedSeconds) override;
};
}