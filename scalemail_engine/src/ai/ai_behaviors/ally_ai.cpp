#include "ai/ai_behaviors/ally_ai.hpp"
#include "ai/ai_nodes/cooldown_ai_node.hpp"
#include "ai/ai_nodes/fire_at_target_ai_node.hpp"
#include "ai/ai_nodes/random_move_direction_ai_node.hpp"
#include "ai/ai_nodes/selector_ai_node.hpp"
#include "ai/ai_nodes/sequence_ai_node.hpp"
#include "ai/ai_nodes/target_attacker_ai_node.hpp"
#include "ai/ai_nodes/target_range_ai_node.hpp"
#include <memory>

namespace ScaleMail
{
static const float MOVE_DIRECTION_CHANGE_INTERVAL = 2.5f;
static const float MIN_VILLAIN_RANGE = 128.0f;

//	============================================================================
AllyAi::AllyAi(Entity entity) : AiBehavior(entity), mAiTree(entity) {
	auto rootNode = std::make_shared<SelectorAiNode>(entity, &mAiTree);
	mAiTree.setRootNode(rootNode);

	//	==================================================
	//	Move in a random direction after cooldown period
	//	==================================================
	auto cooldown = std::make_shared<CooldownAiNode>(entity, &mAiTree);
	cooldown->setDuration(MOVE_DIRECTION_CHANGE_INTERVAL);

	auto randomMoveDirection =
		std::make_shared<RandomMoveDirectionAiNode>(entity, &mAiTree);

	auto randomMove = std::make_shared<SequenceAiNode>(entity, &mAiTree);

	rootNode->addChildNode(randomMove);
	randomMove->addChildNode(cooldown);
	randomMove->addChildNode(randomMoveDirection);

	//	==================================================
	//	Fire at foes
	//	==================================================
	//	Target entities that attack this entity
	auto targetAttacker =
		std::make_shared<TargetAttackerAiNode>(entity, &mAiTree);

	//	Target entities in range of this entity
	auto targetFoes = std::make_shared<TargetRangeAiNode>(entity, &mAiTree);
	targetFoes->setRange(MIN_VILLAIN_RANGE);
	targetFoes->setTargetTeamAlignment(TeamAlignment::FOE);

	auto targetSelector = std::make_shared<SelectorAiNode>(entity, &mAiTree);
	targetSelector->addChildNode(targetAttacker);
	targetSelector->addChildNode(targetFoes);

	auto fireAtFoes = std::make_shared<FireAtTargetAiNode>(entity, &mAiTree);

	auto fireSequence = std::make_shared<SequenceAiNode>(entity, &mAiTree);

	rootNode->addChildNode(fireSequence);
	fireSequence->addChildNode(targetSelector);
	fireSequence->addChildNode(fireAtFoes);
}

//	============================================================================
void AllyAi::think(World& world, float elapsedSeconds) {
	mAiTree.execute(world, elapsedSeconds);
}
}
