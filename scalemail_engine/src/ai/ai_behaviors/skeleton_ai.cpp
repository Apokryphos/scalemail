#include "ai/ai_behaviors/skeleton_ai.hpp"
#include "ai/ai_nodes/seek_target_ai_node.hpp"
#include "ai/ai_nodes/selector_ai_node.hpp"
#include "ai/ai_nodes/sequence_ai_node.hpp"
#include "ai/ai_nodes/target_attacker_ai_node.hpp"
#include "ai/ai_nodes/target_range_ai_node.hpp"

namespace ScaleMail
{
const float MIN_PLAYER_RANGE = 128.0f;

//	============================================================================
SkeletonAi::SkeletonAi(Entity entity) : AiBehavior(entity), mAiTree(entity) {
	//	Target entities that attack this entity
	auto targetAttacker =
		std::make_shared<TargetAttackerAiNode>(entity, mAiTree);

	//	Target entities in range of this entity
	auto targetFoes = std::make_shared<TargetRangeAiNode>(entity, mAiTree);
	targetFoes->setRange(MIN_PLAYER_RANGE);
	targetFoes->setTargetTeamAlignment(TeamAlignment::FOE);

	auto targetSelector = std::make_shared<SelectorAiNode>(entity, mAiTree);

	targetSelector->addChildNode(targetAttacker);
	targetSelector->addChildNode(targetFoes);

	//	Chase target
	std::shared_ptr<SeekTargetAiNode> seekFoe =
		std::make_shared<SeekTargetAiNode>(entity, mAiTree);

	std::shared_ptr<SequenceAiNode> rootNode =
		std::make_shared<SequenceAiNode>(entity, mAiTree);

	mAiTree.setRootNode(rootNode);
	rootNode->addChildNode(targetSelector);
	rootNode->addChildNode(seekFoe);
}

//	============================================================================
void SkeletonAi::think(World& world, double totalElapsedSeconds) {
	mAiTree.execute(world, totalElapsedSeconds);
}
}
