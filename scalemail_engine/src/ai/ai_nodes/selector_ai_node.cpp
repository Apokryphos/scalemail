#include "ai/ai_nodes/selector_ai_node.hpp"

namespace ScaleMail
{
//	============================================================================
SelectorAiNode::SelectorAiNode(Entity& entity, AiTree& parentTree)
: SequenceAiNode(entity, parentTree) {
}

//	============================================================================
AiNodeStatus SelectorAiNode::onChildFailure() {
	//	Continue to next node until end of list or success
	this->activateNextNode();

	//	No child nodes left
	if (this->getCurrentNode() == nullptr) {
		return AiNodeStatus::FAILURE;
	}

	//	Continue until a child node succeeds
	return AiNodeStatus::RUNNING;
}

//	============================================================================
AiNodeStatus SelectorAiNode::onChildSuccess() {
	//	Restart at first node
	this->setCurrentNode(nullptr);

	//	End on first child node success
	return AiNodeStatus::SUCCESS;
}
}