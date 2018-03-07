#pragma once

#include "ai_node.hpp"

namespace ScaleMail
{
class DecoratorAiNode : public AiNode
{
	AiNode* mChildNode;

public:
	DecoratorAiNode(Entity& entity) : AiNode(entity), mChildNode(nullptr) {
	}

	AiNode* getChildNode() {
		return mChildNode;
	}

	bool hasChildNode() const {
		return mChildNode != nullptr;
	}

	void setChildNode(AiNode* node) {
		mChildNode = node;
	}
};
}