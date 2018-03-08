#include "ai/ai_behaviors/blob_ai.hpp"
#include "ai/ai_nodes/entity_count_ai_node.hpp"
#include "ai/ai_nodes/function_ai_node.hpp"
#include "ai/ai_nodes/seek_target_ai_node.hpp"
#include "ai/ai_nodes/selector_ai_node.hpp"
#include "ai/ai_nodes/sequence_ai_node.hpp"
#include "ai/ai_nodes/target_range_ai_node.hpp"
#include "ai/ai_nodes/wander_ai_node.hpp"
#include "ai_system.hpp"
#include "actor_util.hpp"
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
const int MAX_ITEM_CARRY = 3;
const float MAX_LOOT_RANGE = 256.0f;
const float NORMAL_SPEED = 16.0f;
const float LOOT_CHASE_SPEED = 64.0f;

//	============================================================================
BlobAi::BlobAi(Entity entity) : AiBehavior(entity), mAiTree(entity) {
	auto rootNode = std::make_shared<SelectorAiNode>(entity, &mAiTree);
	mAiTree.setRootNode(rootNode);

	//	Don't chase loot if at max capacity.
	auto maxCarry = std::make_shared<FunctionAiNode>(entity, &mAiTree);
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

	auto hasTarget = std::make_shared<EntityCountAiNode>(entity, &mAiTree);
	hasTarget->setGreaterThanOrEqualTo(1);

	//	Target loot in range of this entity
	auto targetLoot = std::make_shared<TargetRangeAiNode>(entity, &mAiTree);
	targetLoot->setRange(MAX_LOOT_RANGE);
	targetLoot->setTargetType(TargetType::LOOT);

	auto targetSelector = std::make_shared<SelectorAiNode>(entity, &mAiTree);
	targetSelector->addChildNode(hasTarget);
	targetSelector->addChildNode(targetLoot);

	//	Increase speed when chasing loot
	auto speedUp = std::make_shared<FunctionAiNode>(entity, &mAiTree);
	speedUp->setFunction(
		[](AiNode& aiNode, World& world) {
			Entity entity = aiNode.getEntity();

			PhysicsSystem& physicsSystem = world.getPhysicsSystem();

			PhysicsComponent physicsCmpnt =
				physicsSystem.getComponent(entity);

			physicsSystem.setSpeed(physicsCmpnt, LOOT_CHASE_SPEED);

			return AiNodeStatus::SUCCESS;
		}
	);

	//	Seek loot target
	auto seekLoot = std::make_shared<SeekTargetAiNode>(entity, &mAiTree);

	auto chaseLoot = std::make_shared<SequenceAiNode>(entity, &mAiTree);
	rootNode->addChildNode(chaseLoot);
	chaseLoot->addChildNode(maxCarry);
	chaseLoot->addChildNode(targetSelector);
	chaseLoot->addChildNode(speedUp);
	chaseLoot->addChildNode(seekLoot);

	//	Decrease speed when wandering
	auto speedDown = std::make_shared<FunctionAiNode>(entity, &mAiTree);
	speedDown->setFunction(
		[](AiNode& aiNode, World& world) {
			Entity entity = aiNode.getEntity();

			PhysicsSystem& physicsSystem = world.getPhysicsSystem();

			PhysicsComponent physicsCmpnt =
				physicsSystem.getComponent(entity);

			physicsSystem.setSpeed(physicsCmpnt, NORMAL_SPEED);

			return AiNodeStatus::SUCCESS;
		}
	);

	//	Wander if there's no loot to chase
	auto wander = std::make_shared<WanderAiNode>(entity, &mAiTree);

	auto wanderSequence = std::make_shared<SequenceAiNode>(entity, &mAiTree);
	rootNode->addChildNode(wanderSequence);
	wanderSequence->addChildNode(speedDown);
	wanderSequence->addChildNode(wander);
}

//	============================================================================
void BlobAi::think(World& world, double totalElapsedSeconds) {
	mAiTree.execute(world, totalElapsedSeconds);

	// const Entity entity = this->getEntity();

	// if (!actorCanMove(entity, world)) {
	// 	return;
	// }

	// PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	// PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	// glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

	// InventorySystem& inventorySystem = world.getInventorySystem();
	// InventoryComponent inventoryCmpnt = inventorySystem.getComponent(entity);

	// //	Chase loot if inventory isn't full
	// if (inventorySystem.getItemCount(inventoryCmpnt) < MAX_ITEM_CARRY) {
	// 	if (!mTargetEntity.has_value() ||
	// 		!world.getLootSystem().hasComponent(mTargetEntity.value())) {
	// 		//	Assign a new target
	// 		mTargetEntity = getRandomLootInRange(world, position, MAX_LOOT_RANGE);
	// 	}

	// 	if (mTargetEntity.has_value() &&
	// 		world.getLootSystem().hasComponent(mTargetEntity.value())) {
	// 		PhysicsComponent targetPhysicsCmpnt =
	// 			physicsSystem.getComponent(mTargetEntity.value());
	// 		glm::vec2 targetPosition = physicsSystem.getPosition(targetPhysicsCmpnt);

	// 		//	Increase speed
	// 		physicsSystem.setSpeed(physicsCmpnt, LOOT_CHASE_SPEED);

	// 		//	Set move direction towards target
	// 		AiSystem& aiSystem = world.getAiSystem();
	// 		AiComponent aiCmpnt = aiSystem.getComponent(entity);
	// 		aiSystem.setMoveDirection(aiCmpnt, targetPosition - position);

	// 		//	Stop wandering
	// 		aiSystem.setWander(aiCmpnt, false);

	// 		//	Stop running AI
	// 		return;
	// 	}
	// }

	// //	Set speed to normal
	// physicsSystem.setSpeed(physicsCmpnt, NORMAL_SPEED);

	// //	Wander
	// AiSystem& aiSystem = world.getAiSystem();
	// AiComponent aiCmpnt = aiSystem.getComponent(entity);
	// aiSystem.setWander(aiCmpnt, true);
}
}
