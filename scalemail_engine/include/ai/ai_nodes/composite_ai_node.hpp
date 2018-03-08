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
	CompositeAiNode(Entity& entity, AiTree* parentTree = nullptr);
	void addChildNode(std::shared_ptr<AiNode> node);
	void addChildNodes(std::vector<std::shared_ptr<AiNode>> nodes);
	bool hasChildNodes() const;
	std::vector<std::shared_ptr<AiNode>> getChildNodes();
};
}