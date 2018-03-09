#include "ai/ai_nodes/decorator_ai_node.hpp"

namespace ScaleMail
{
//	============================================================================
DecoratorAiNode::DecoratorAiNode(Entity& entity, AiTree& parentTree)
: AiNode(entity, parentTree), mChildNode(nullptr) {
}

//	============================================================================
std::shared_ptr<AiNode> DecoratorAiNode::getChildNode() {
	return mChildNode;
}

//	============================================================================
bool DecoratorAiNode::hasChildNode() const {
	return mChildNode != nullptr;
}

//	============================================================================
void DecoratorAiNode::setChildNode(std::shared_ptr<AiNode> node) {
	mChildNode = node;
}
}