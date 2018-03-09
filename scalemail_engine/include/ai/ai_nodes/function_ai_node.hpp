#pragma once

#include "ai/ai_node.hpp"
#include <functional>

namespace ScaleMail
{
class FunctionAiNode : public AiNode
{
	std::function<AiNodeStatus(AiNode&, World&)> mFunction;

public:
	FunctionAiNode(Entity& entity, AiTree& parentTree);
	virtual AiNodeStatus execute(World& world) override;
	void setFunction(std::function<AiNodeStatus(AiNode&, World&)> func);
};
}