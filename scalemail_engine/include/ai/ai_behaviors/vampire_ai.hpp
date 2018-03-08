#pragma once

#include "ai/ai_tree.hpp"
#include "ai/ai_behavior.hpp"

namespace ScaleMail
{
class VampireAi : public AiBehavior
{
	AiTree mAiTree;

public:
	VampireAi(Entity entity);
	virtual void think(World& world, double totalElapsedSeconds) override;
};
}