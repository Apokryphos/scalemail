#include "ai/ai_nodes/sequence_ai_node.hpp"

namespace ScaleMail
{
//	============================================================================
SequenceAiNode::SequenceAiNode(Entity& entity, AiTree* parentTree)
: CompositeAiNode(entity, parentTree), mCurrentNode(nullptr) {
}

//	============================================================================
void SequenceAiNode::activateNextNode() {
	if (!this->hasChildNodes()) {
		return;
	}

	mCurrentNode = this->getNextNode();
}

//	============================================================================
AiNodeStatus SequenceAiNode::execute(World& world, float elapsedSeconds) {
 	if (mCurrentNode == nullptr) {
		this->activateNextNode();
	}

	if (mCurrentNode != nullptr) {
		//	Execute current node
		AiNodeStatus status = mCurrentNode->execute(world, elapsedSeconds);

		if (status == AiNodeStatus::SUCCESS)	{
			AiNodeStatus derivedStatus = this->onChildSuccess();

			if (derivedStatus == AiNodeStatus::SUCCESS) {
				mCurrentNode = nullptr;
			}

			return derivedStatus;
		}

		if (status == AiNodeStatus::FAILURE)	{
			AiNodeStatus derivedStatus = this->onChildFailure();

			if (derivedStatus == AiNodeStatus::SUCCESS) {
				mCurrentNode = nullptr;
			}

			return derivedStatus;
		}

		return AiNodeStatus::RUNNING;
	}

	//	Sequence failed
	return AiNodeStatus::FAILURE;
}

//	============================================================================
AiNode* SequenceAiNode::getCurrentNode() {
	return mCurrentNode;
}

//	============================================================================
AiNode* SequenceAiNode::getNextNode() {
	const auto& childNodes = this->getChildNodes();

	if (mCurrentNode == nullptr) {
		//	Return first child node or null if empty
		return childNodes.size() > 0 ? childNodes[0].get() : nullptr;
	}

	for (size_t n = 0; n < childNodes.size() - 1; ++n) {
		if (childNodes[n].get() == mCurrentNode) {
			//	Return next child node after current node
			return childNodes[n + 1].get();
		}
	}

	//	Reached end of sequence
	return nullptr;
}

//	============================================================================
AiNodeStatus SequenceAiNode::onChildFailure() {
	mCurrentNode = nullptr;
	return AiNodeStatus::FAILURE;
}

//	============================================================================
AiNodeStatus SequenceAiNode::onChildSuccess() {
	this->activateNextNode();

	if (mCurrentNode == nullptr) {
		//	Sequence completed successfully
		return AiNodeStatus::SUCCESS;
	}

	return AiNodeStatus::RUNNING;
}

//	============================================================================
void SequenceAiNode::setCurrentNode(AiNode* node) {
	mCurrentNode = node;
}
}