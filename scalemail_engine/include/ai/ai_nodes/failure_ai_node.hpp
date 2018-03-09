#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class World;

class FailureAiNode : public AiNode
{
public:
	FailureAiNode(Entity& entity, AiTree& parentTree);
	virtual AiNodeStatus execute(World& world) override;
};
}