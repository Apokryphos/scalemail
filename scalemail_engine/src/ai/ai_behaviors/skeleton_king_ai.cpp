#include "ai/ai_behaviors/skeleton_king_ai.hpp"
#include "ai/ai_nodes/fire_at_target_ai_node.hpp"
#include "ai/ai_nodes/seek_target_ai_node.hpp"
#include "ai/ai_nodes/selector_ai_node.hpp"
#include "ai/ai_nodes/sequence_ai_node.hpp"
#include "ai/ai_nodes/target_attacker_ai_node.hpp"
#include "ai/ai_nodes/target_range_ai_node.hpp"
#include "actor_util.hpp"

namespace ScaleMail
{
static const float MIN_PLAYER_RANGE = 128.0f;

//	============================================================================
SkeletonKingAi::SkeletonKingAi(Entity entity)
: AiBehavior(entity), mAiTree(entity) {
	auto rootNode = std::make_shared<SequenceAiNode>(entity, mAiTree);
	mAiTree.setRootNode(rootNode);

	//	Target entities that attack this entity
	auto targetAttacker =
		std::make_shared<TargetAttackerAiNode>(entity, mAiTree);

	//	Target entities in range of this entity
	auto targetFoes = std::make_shared<TargetRangeAiNode>(entity, mAiTree);
	targetFoes->setRange(MIN_PLAYER_RANGE);
	targetFoes->setTargetTeamAlignment(TeamAlignment::FOE);

	auto targetSelector = std::make_shared<SelectorAiNode>(entity, mAiTree);
	rootNode->addChildNode(targetSelector);
	targetSelector->addChildNode(targetAttacker);
	targetSelector->addChildNode(targetFoes);

	//	Fire at target
	auto fireAtFoes = std::make_shared<FireAtTargetAiNode>(entity, mAiTree);

	auto fireSelector = std::make_shared<SelectorAiNode>(entity, mAiTree);
	rootNode->addChildNode(fireSelector);
	fireSelector->addChildNode(fireAtFoes);

	//	Chase target
	auto seekFoe = std::make_shared<SeekTargetAiNode>(entity, mAiTree);

	auto seekSelector = std::make_shared<SelectorAiNode>(entity, mAiTree);
	rootNode->addChildNode(seekSelector);
	seekSelector->addChildNode(seekFoe);
}

//	============================================================================
void SkeletonKingAi::think(World& world, double totalElapsedSeconds) {
	mAiTree.execute(world, totalElapsedSeconds);
}
}
