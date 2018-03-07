#pragma once

#include "ai/ai_node.hpp"
#include <memory>
#include <vector>

namespace ScaleMail
{
class CompositeAiNode : public AiNode
{
	std::vector<std::shared_ptr<AiNode>> mChildNodes;

public:
	CompositeAiNode(Entity& entity, AiTree* parentTree = nullptr)
	: AiNode(entity, parentTree) {
	}

	void addChildNode(std::shared_ptr<AiNode> node) {
		if (node != nullptr) {
			mChildNodes.push_back(node);
		}
	}

	void addChildNodes(std::vector<std::shared_ptr<AiNode>> nodes) {
		for (auto node : nodes) {
			this->addChildNode(node);
		}
	}

	bool hasChildNodes() const {
		return mChildNodes.size() > 0;
	}

	std::vector<std::shared_ptr<AiNode>> getChildNodes() {
		return mChildNodes;
	}
};
}