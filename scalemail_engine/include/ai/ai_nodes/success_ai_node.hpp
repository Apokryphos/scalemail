#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class World;

class SuccessAiNode : public AiNode
{
public:
	SuccessAiNode(Entity& entity, AiTree& parentTree);
	AiNodeStatus execute(World& world);
};
}