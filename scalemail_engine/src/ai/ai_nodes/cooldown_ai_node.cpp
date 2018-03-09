#include "ai/ai_nodes/cooldown_ai_node.hpp"
#include "ai/ai_tree.hpp"
#include "world.hpp"
#include <algorithm>

namespace ScaleMail
{
//	============================================================================
CooldownAiNode::CooldownAiNode(Entity& entity, AiTree& parentTree,
							   double duration)
: AiNode(entity, parentTree), mDurationMin(0), mDurationMax(0), mTicks(0),
  mLastTicks(0) {
	this->setDuration(duration);
}

//	============================================================================
AiNodeStatus CooldownAiNode::execute([[maybe_unused]]World& world) {
	const AiTree& aiTree = this->getParentTree();

	//	Calculate elapsed seconds since this node won't be executed every frame
	double elapsedSeconds = aiTree.getElapsedSeconds(mLastTicks);
	mLastTicks = aiTree.getTotalElapsedSeconds();

	mTicks -= elapsedSeconds;
	if (mTicks <= 0) {
		double duration =
			world.getRandom().nextDouble(mDurationMin, mDurationMax);

		mTicks = duration;
		return AiNodeStatus::SUCCESS;
	}

	return AiNodeStatus::FAILURE;
}

//	============================================================================
void CooldownAiNode::setDuration(double duration) {
	duration = std::max(0.0, duration);
	mDurationMin = duration;
	mDurationMax = duration;
}

//	============================================================================
void CooldownAiNode::setRandomDuration(double min, double max) {
	mDurationMin = std::max(min, 0.0);
	mDurationMax = std::max(0.0, max);
}
}