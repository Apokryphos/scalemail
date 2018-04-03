#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class StopFiringAiNode : public AiNode
{
public:
	StopFiringAiNode(Entity& entity, AiTree& parentTree);
	virtual AiNodeStatus execute(World& world) override;
};
}