#include "ai/ai_nodes/cooldown_ai_node.hpp"

namespace ScaleMail
{
//	============================================================================
CooldownAiNode::CooldownAiNode(Entity& entity, AiTree* parentTree,
							   const float duration)
: AiNode(entity, parentTree), mDuration(duration), mTicks(0) {
}

//	============================================================================
AiNodeStatus CooldownAiNode::execute([[maybe_unused]]World& world,
									[[maybe_unused]]float elapsedSeconds) {
	mTicks -= elapsedSeconds;
	if (mTicks <= 0.0f) {
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