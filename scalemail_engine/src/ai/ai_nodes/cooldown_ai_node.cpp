#include "ai/ai_nodes/cooldown_ai_node.hpp"
#include "ai/ai_tree.hpp"

namespace ScaleMail
{
//	============================================================================
CooldownAiNode::CooldownAiNode(Entity& entity, AiTree& parentTree,
							   const float duration)
: AiNode(entity, parentTree), mDuration(duration), mTicks(0), mLastTicks(0) {
}

//	============================================================================
AiNodeStatus CooldownAiNode::execute([[maybe_unused]]World& world) {
	const AiTree& aiTree = this->getParentTree();

	//	Calculate elapsed seconds since this node won't be executed every frame
	double elapsedSeconds = aiTree.getElapsedSeconds(mLastTicks);
	mLastTicks = aiTree.getTotalElapsedSeconds();

	mTicks -= elapsedSeconds;
	if (mTicks <= 0) {
		mTicks = mDuration;
		return AiNodeStatus::SUCCESS;
	}

	return AiNodeStatus::FAILURE;
}

//	============================================================================
void CooldownAiNode::setDuration(const float duration) {
	mDuration = duration;
}
}