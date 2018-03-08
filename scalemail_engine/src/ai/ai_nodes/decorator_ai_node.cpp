#include "ai/ai_nodes/decorator_ai_node.hpp"

namespace ScaleMail
{
//	============================================================================
DecoratorAiNode::DecoratorAiNode(Entity& entity, AiTree* parentTree)
: AiNode(entity, parentTree), mChildNode(nullptr) {
}

//	============================================================================
AiNode* DecoratorAiNode::getChildNode() {
	return mChildNode;
}

//	============================================================================
bool DecoratorAiNode::hasChildNode() const {
	return mChildNode != nullptr;
}

//	============================================================================
void DecoratorAiNode::setChildNode(AiNode* node) {
	mChildNode = node;
}
}