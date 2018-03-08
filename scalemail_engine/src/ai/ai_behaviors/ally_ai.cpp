#include "ai/ai_behaviors/ally_ai.hpp"
#include "ai/ai_nodes/cooldown_ai_node.hpp"
#include "ai/ai_nodes/fire_at_foes_ai_node.hpp"
#include "ai/ai_nodes/random_move_direction_ai_node.hpp"
#include "ai/ai_nodes/selector_ai_node.hpp"
#include "ai/ai_nodes/sequence_ai_node.hpp"
#include "gun_system.hpp"
#include "world.hpp"
#include <memory>

namespace ScaleMail
{
static const float MOVE_DIRECTION_CHANGE_INTERVAL = 2.5f;
static const float MIN_VILLAIN_RANGE = 128.0f;

//	============================================================================
AllyAi::AllyAi(Entity entity) : AiBehavior(entity), mAiTree(entity) {
	//	==================================================
	//	Move in a random direction after cooldown period
	//	==================================================
	std::shared_ptr<CooldownAiNode> cooldown =
		std::make_shared<CooldownAiNode>(entity, &mAiTree);

	std::shared_ptr<RandomMoveDirectionAiNode> randomMoveDirection =
		std::make_shared<RandomMoveDirectionAiNode>(entity, &mAiTree);

	std::shared_ptr<SequenceAiNode> randomMove =
		std::make_shared<SequenceAiNode>(entity, &mAiTree);

	randomMove->addChildNode(cooldown);
	randomMove->addChildNode(randomMoveDirection);

	//	==================================================
	//	Fire at foes
	//	==================================================
	std::shared_ptr<FireAtFoesAiNode> fireAtFoes =
		std::make_shared<FireAtFoesAiNode>(entity, &mAiTree);

	std::shared_ptr<SelectorAiNode> rootNode =
		std::make_shared<SelectorAiNode>(entity, &mAiTree);

	mAiTree.setRootNode(rootNode);
	rootNode->addChildNode(randomMove);
	rootNode->addChildNode(fireAtFoes);

	cooldown->setDuration(MOVE_DIRECTION_CHANGE_INTERVAL);
	fireAtFoes->setRange(MIN_VILLAIN_RANGE);
}

//	============================================================================
void AllyAi::think(World& world, float elapsedSeconds) {
	const Entity entity = this->getEntity();

	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);

	gunSystem.setFire(gunCmpnt, false);

	mAiTree.getRootNode()->execute(world, elapsedSeconds);
}
}
