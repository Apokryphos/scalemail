#include "ai/ai_nodes/composite_ai_node.hpp"

namespace ScaleMail
{
//	============================================================================
CompositeAiNode::CompositeAiNode(Entity& entity, AiTree& parentTree)
: AiNode(entity, parentTree) {
}

//	============================================================================
void CompositeAiNode::addChildNode(std::shared_ptr<AiNode> node) {
	if (node != nullptr) {
		mChildNodes.push_back(node);
	}
}

//	============================================================================
void CompositeAiNode::addChildNodes(std::vector<std::shared_ptr<AiNode>> nodes) {
	for (auto node : nodes) {
		this->addChildNode(node);
	}
}

//	============================================================================
bool CompositeAiNode::hasChildNodes() const {
	return mChildNodes.size() > 0;
}

//	============================================================================
std::vector<std::shared_ptr<AiNode>> CompositeAiNode::getChildNodes() {
	return mChildNodes;
}
}