#include "ai/ai_behaviors/blob_ai.hpp"
#include "ai/ai_nodes/cooldown_ai_node.hpp"
#include "ai/ai_nodes/entity_count_ai_node.hpp"
#include "ai/ai_nodes/function_ai_node.hpp"
#include "ai/ai_nodes/random_move_direction_ai_node.hpp"
#include "ai/ai_nodes/seek_target_ai_node.hpp"
#include "ai/ai_nodes/selector_ai_node.hpp"
#include "ai/ai_nodes/sequence_ai_node.hpp"
#include "ai/ai_nodes/target_range_ai_node.hpp"
#include "ai/ai_nodes/wander_ai_node.hpp"
#include "ai_system.hpp"
#include "actor_util.hpp"
#include "inventory_system.hpp"
#include "math_util.hpp"
#include "physics_system.hpp"
#include "player.hpp"
#include "player_util.hpp"
#include "world.hpp"
#include <glm/gtx/norm.hpp>
#include <algorithm>
#include <vector>

namespace ScaleMail
{
static const int MAX_ITEM_CARRY = 3;
static const float MAX_LOOT_RANGE = 256.0f;
static const float NORMAL_SPEED = 16.0f;
static const float LOOT_CHASE_SPEED = 64.0f;
static const float MOVE_DIRECTION_CHANGE_INTERVAL = 2.25f;

//	============================================================================
BlobAi::BlobAi(Entity entity) : AiBehavior(entity), mAiTree(entity) {
	auto rootNode = std::make_shared<SelectorAiNode>(entity, mAiTree);
	mAiTree.setRootNode(rootNode);

	//	Don't chase loot if at max capacity.
	auto maxCarry = std::make_shared<FunctionAiNode>(entity, mAiTree);
	maxCarry->setFunction(
		[](AiNode& aiNode, World& world) {
			Entity entity = aiNode.getEntity();

			InventorySystem& inventorySystem = world.getInventorySystem();

			InventoryComponent inventoryCmpnt =
				inventorySystem.getComponent(entity);

			const int itemCount = inventorySystem.getItemCount(inventoryCmpnt);

			return
				itemCount < MAX_ITEM_CARRY ?
				AiNodeStatus::SUCCESS :
				AiNodeStatus::FAILURE;
		}
	);

	auto hasTarget = std::make_shared<EntityCountAiNode>(entity, mAiTree);
	hasTarget->setGreaterThanOrEqualTo(1);

	//	Target loot in range of this entity
	auto targetLoot = std::make_shared<TargetRangeAiNode>(entity, mAiTree);
	targetLoot->setRange(MAX_LOOT_RANGE);
	targetLoot->setTargetType(TargetType::LOOT);

	auto targetSelector = std::make_shared<SelectorAiNode>(entity, mAiTree);
	targetSelector->addChildNode(hasTarget);
	targetSelector->addChildNode(targetLoot);

	//	Increase speed when chasing loot
	auto speedUp = std::make_shared<FunctionAiNode>(entity, mAiTree);
	speedUp->setFunction(
		[](AiNode& aiNode, World& world) {
			Entity entity = aiNode.getEntity();

			PhysicsSystem& physicsSystem = world.getPhysicsSystem();

			PhysicsComponent physicsCmpnt =
				physicsSystem.getComponent(entity);

			physicsSystem.setMaxSpeed(physicsCmpnt, LOOT_CHASE_SPEED);

			return AiNodeStatus::SUCCESS;
		}
	);

	//	Seek loot target
	auto seekLoot = std::make_shared<SeekTargetAiNode>(entity, mAiTree);

	auto chaseLoot = std::make_shared<SequenceAiNode>(entity, mAiTree);
	chaseLoot->addChildNode(maxCarry);
	chaseLoot->addChildNode(targetSelector);
	chaseLoot->addChildNode(speedUp);
	chaseLoot->addChildNode(seekLoot);

	//	Stop seeking
	auto stopSeek = std::make_shared<FunctionAiNode>(entity, mAiTree);
	stopSeek->setFunction(
		[](AiNode& aiNode, World& world) {
			Entity entity = aiNode.getEntity();

			AiSystem& aiSystem = world.getAiSystem();

			AiComponent aiCmpnt = aiSystem.getComponent(entity);

			aiSystem.setSeek(aiCmpnt, false);

			return AiNodeStatus::FAILURE;
		}
	);

	auto chaseLootSelector = std::make_shared<SelectorAiNode>(entity, mAiTree);
	rootNode->addChildNode(chaseLootSelector);
	chaseLootSelector->addChildNode(chaseLoot);
	chaseLootSelector->addChildNode(stopSeek);

	//	Decrease speed when wandering
	auto speedDown = std::make_shared<FunctionAiNode>(entity, mAiTree);
	speedDown->setFunction(
		[](AiNode& aiNode, World& world) {
			Entity entity = aiNode.getEntity();

			PhysicsSystem& physicsSystem = world.getPhysicsSystem();

			PhysicsComponent physicsCmpnt =
				physicsSystem.getComponent(entity);

			physicsSystem.setMaxSpeed(physicsCmpnt, NORMAL_SPEED);

			return AiNodeStatus::SUCCESS;
		}
	);

	//	Wander if there's no loot to chase
	auto wander = std::make_shared<WanderAiNode>(entity, mAiTree);

	//	==================================================
	//	Move in a random direction after cooldown period
	//	==================================================
	auto cooldown = std::make_shared<CooldownAiNode>(entity, mAiTree);
	cooldown->setDuration(MOVE_DIRECTION_CHANGE_INTERVAL);

	auto randomMoveDirection =
		std::make_shared<RandomMoveDirectionAiNode>(entity, mAiTree);

	auto randomMove = std::make_shared<SequenceAiNode>(entity, mAiTree);
	randomMove->addChildNode(cooldown);
	randomMove->addChildNode(randomMoveDirection);

	auto wanderSequence = std::make_shared<SequenceAiNode>(entity, mAiTree);
	rootNode->addChildNode(wanderSequence);
	wanderSequence->addChildNode(speedDown);
	wanderSequence->addChildNode(wander);
	wanderSequence->addChildNode(randomMove);
}

//	============================================================================
void BlobAi::think(World& world, double totalElapsedSeconds) {
	mAiTree.execute(world, totalElapsedSeconds);
}
}
