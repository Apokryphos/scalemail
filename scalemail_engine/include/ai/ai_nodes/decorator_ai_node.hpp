#pragma once

#include "ai/ai_node.hpp"
#include <memory>

namespace ScaleMail
{
class DecoratorAiNode : public AiNode
{
	std::shared_ptr<AiNode> mChildNode;

public:
	DecoratorAiNode(Entity& entity, AiTree& parentTree);
	std::shared_ptr<AiNode> getChildNode();
	bool hasChildNode() const;
	void setChildNode(std::shared_ptr<AiNode> node);
};
}