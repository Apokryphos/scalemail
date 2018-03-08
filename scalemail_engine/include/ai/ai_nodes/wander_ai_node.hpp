#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class WanderAiNode : public AiNode
{
public:
	WanderAiNode(Entity& entity, AiTree* parentTree = nullptr);
	virtual AiNodeStatus execute(World& world, float elapsedSeconds) override;
};
}