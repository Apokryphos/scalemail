#pragma once

#include "ai/ai_nodes/decorator_ai_node.hpp"

namespace ScaleMail
{
class World;

class InverterAiNode : public DecoratorAiNode
{
public:
	InverterAiNode(Entity& entity, AiTree* parentTree = nullptr);
	virtual AiNodeStatus execute(World& world) override;
};
}