#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class World;

class SuccessAiNode : public AiNode
{
public:
	SuccessAiNode(Entity& entity, AiTree* parentTree = nullptr);
	AiNodeStatus execute(World& world, float elapsedSeconds);
};
}