#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class RandomMoveDirectionAiNode : public AiNode
{
public:
	RandomMoveDirectionAiNode(Entity& entity, AiTree& parentTree);
	virtual AiNodeStatus execute(World& world) override;
};
}