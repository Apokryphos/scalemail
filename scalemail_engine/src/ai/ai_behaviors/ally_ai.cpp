#include "ai/ai_behaviors/ally_ai.hpp"
#include "ai/ai_nodes/cooldown_ai_node.hpp"
#include "ai/ai_nodes/entity_count_ai_node.hpp"
#include "ai/ai_nodes/fire_at_target_ai_node.hpp"
#include "ai/ai_nodes/random_move_direction_ai_node.hpp"
#include "ai/ai_nodes/selector_ai_node.hpp"
#include "ai/ai_nodes/sequence_ai_node.hpp"
#include "ai/ai_nodes/target_attacker_ai_node.hpp"
#include "ai/ai_nodes/target_range_ai_node.hpp"
#include "ai/ai_nodes/wander_ai_node.hpp"
#include <memory>

namespace ScaleMail
{
static const double MOVE_DIRECTION_CHANGE_INTERVAL_MIN = 1.0;
static const double MOVE_DIRECTION_CHANGE_INTERVAL_MAX = 3.0;
static const float MIN_VILLAIN_RANGE = 128.0f;

//	============================================================================
AllyAi::AllyAi(Entity entity) : AiBehavior(entity), mAiTree(entity) {
	auto rootNode = std::make_shared<SelectorAiNode>(entity, mAiTree);
	mAiTree.setRootNode(rootNode);

	//	==================================================
	//	Move in a random direction after cooldown period
	//	==================================================
	auto cooldown = std::make_shared<CooldownAiNode>(entity, mAiTree);
	cooldown->setRandomDuration(
		MOVE_DIRECTION_CHANGE_INTERVAL_MIN,
		MOVE_DIRECTION_CHANGE_INTERVAL_MAX);

	auto randomMoveDirection =
		std::make_shared<RandomMoveDirectionAiNode>(entity, mAiTree);

	//	Wander to add variation to movement
	auto wander =
		std::make_shared<WanderAiNode>(entity, mAiTree);

	auto randomMove = std::make_shared<SequenceAiNode>(entity, mAiTree);
	rootNode->addChildNode(randomMove);
	randomMove->addChildNode(cooldown);
	randomMove->addChildNode(randomMoveDirection);
	randomMove->addChildNode(wander);

	//	==================================================
	//	Fire at foes
	//	==================================================
	//	Target entities that attack this entity
	auto targetAttacker =
		std::make_shared<TargetAttackerAiNode>(entity, mAiTree);

	auto hasTarget = std::make_shared<EntityCountAiNode>(entity, mAiTree);
	hasTarget->setGreaterThanOrEqualTo(1);

	//	Target entities in range of this entity
	auto targetFoes = std::make_shared<TargetRangeAiNode>(entity, mAiTree);
	targetFoes->setRange(MIN_VILLAIN_RANGE);
	targetFoes->setTargetTeamAlignment(TeamAlignment::FOE);

	auto targetSelector = std::make_shared<SelectorAiNode>(entity, mAiTree);
	targetSelector->addChildNode(hasTarget);
	targetSelector->addChildNode(targetAttacker);
	targetSelector->addChildNode(targetFoes);

	//	Fire at target
	auto fireAtTarget = std::make_shared<FireAtTargetAiNode>(entity, mAiTree);

	auto fireSequence = std::make_shared<SequenceAiNode>(entity, mAiTree);
	rootNode->addChildNode(fireSequence);
	fireSequence->addChildNode(targetSelector);
	fireSequence->addChildNode(fireAtTarget);
}

//	============================================================================
void AllyAi::think(World& world, double totalElapsedSeconds) {
	mAiTree.execute(world, totalElapsedSeconds);
}
}
