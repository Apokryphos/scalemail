#include "ai/ai_nodes/selector_ai_node.hpp"

namespace ScaleMail
{
//	============================================================================
SelectorAiNode::SelectorAiNode(Entity& entity, AiTree* parentTree)
: SequenceAiNode(entity, parentTree) {
}

//	============================================================================
AiNodeStatus SelectorAiNode::onChildFailure() {
	this->activateNextNode();

	if (this->getCurrentNode() == nullptr) {
		//	No child nodes left
		return AiNodeStatus::FAILURE;
	}

	//	Continue until a child node succeeds
	return AiNodeStatus::RUNNING;
}

//	============================================================================
AiNodeStatus SelectorAiNode::onChildSuccess() {
	//	End on first child node success
	return AiNodeStatus::SUCCESS;
}
}