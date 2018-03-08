#include "ai/ai_behaviors/vampire_ai.hpp"
#include "ai/ai_nodes/cooldown_ai_node.hpp"
#include "ai/ai_nodes/entity_count_ai_node.hpp"
#include "ai/ai_nodes/fire_at_target_ai_node.hpp"
#include "ai/ai_nodes/random_move_direction_ai_node.hpp"
#include "ai/ai_nodes/selector_ai_node.hpp"
#include "ai/ai_nodes/sequence_ai_node.hpp"
#include "ai/ai_nodes/success_ai_node.hpp"
#include "ai/ai_nodes/target_attacker_ai_node.hpp"
#include "ai/ai_nodes/target_range_ai_node.hpp"

namespace ScaleMail
{
const float MIN_PLAYER_RANGE = 128.0f;
static const float MOVE_DIRECTION_CHANGE_INTERVAL = 2.25f;

//	============================================================================
VampireAi::VampireAi(Entity entity) : AiBehavior(entity), mAiTree(entity) {
	auto rootNode = std::make_shared<SequenceAiNode>(entity, &mAiTree);
	mAiTree.setRootNode(rootNode);

	//	==================================================
	//	Move in a random direction after cooldown period
	//	==================================================
	auto cooldown = std::make_shared<CooldownAiNode>(entity, &mAiTree);
	cooldown->setDuration(MOVE_DIRECTION_CHANGE_INTERVAL);

	auto randomMoveDirection =
		std::make_shared<RandomMoveDirectionAiNode>(entity, &mAiTree);

	auto randomMoveSuccess = std::make_shared<SuccessAiNode>(entity, &mAiTree);

	auto randomMove =
		std::make_shared<SequenceAiNode>(entity, &mAiTree);
	rootNode->addChildNode(randomMove);
	randomMove->addChildNode(cooldown);
	randomMove->addChildNode(randomMoveDirection);
	randomMove->addChildNode(randomMoveSuccess);

	//	==================================================
	//	Fire at entities
	//	==================================================
	//	Target entities that attack this entity
	auto targetAttacker =
		std::make_shared<TargetAttackerAiNode>(entity, &mAiTree);

	auto hasTarget = std::make_shared<EntityCountAiNode>(entity, &mAiTree);
	hasTarget->setGreaterThanOrEqualTo(1);

	//	Target entities in range of this entity
	auto targetFoes = std::make_shared<TargetRangeAiNode>(entity, &mAiTree);
	targetFoes->setRange(MIN_PLAYER_RANGE);
	targetFoes->setTargetTeamAlignment(TeamAlignment::FOE);

	auto targetSelector = std::make_shared<SelectorAiNode>(entity, &mAiTree);
	rootNode->addChildNode(targetSelector);
	targetSelector->addChildNode(targetAttacker);
	targetSelector->addChildNode(targetFoes);

	//	Fire at target
	auto fireAtTarget = std::make_shared<FireAtTargetAiNode>(entity, &mAiTree);

	auto fireSelector = std::make_shared<SelectorAiNode>(entity, &mAiTree);
	rootNode->addChildNode(fireSelector);
	fireSelector->addChildNode(fireAtTarget);
}

//	============================================================================
void VampireAi::think(World& world, float elapsedSeconds) {
	mAiTree.execute(world, elapsedSeconds);
}
}
