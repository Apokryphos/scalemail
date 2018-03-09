#include "ai/ai_behaviors/wisp_ai.hpp"
#include "ai/ai_nodes/cooldown_ai_node.hpp"
#include "ai/ai_nodes/random_move_direction_ai_node.hpp"
#include "ai/ai_nodes/selector_ai_node.hpp"
#include "ai/ai_nodes/sequence_ai_node.hpp"
#include "ai/ai_nodes/wander_ai_node.hpp"

namespace ScaleMail
{
static const double MOVE_DIRECTION_CHANGE_INTERVAL_MIN = 1.0;
static const double MOVE_DIRECTION_CHANGE_INTERVAL_MAX = 3.0;

//	============================================================================
WispAi::WispAi(Entity entity) : AiBehavior(entity), mAiTree(entity) {
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
}

//	============================================================================
void WispAi::think(World& world, double totalElapsedSeconds) {
	mAiTree.execute(world, totalElapsedSeconds);
}
}
