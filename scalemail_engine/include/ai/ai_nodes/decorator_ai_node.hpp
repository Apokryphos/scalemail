#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class DecoratorAiNode : public AiNode
{
	AiNode* mChildNode;

public:
	DecoratorAiNode(Entity& entity, AiTree& parentTree);
	AiNode* getChildNode();
	bool hasChildNode() const;
	void setChildNode(AiNode* node);
};
}