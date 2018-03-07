#include "ai/ai_behaviors/blob_ai.hpp"
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
const float MIN_LOOT_RANGE = 256.0f;
const float NORMAL_SPEED = 16.0f;
const float LOOT_CHASE_SPEED = 64.0f;

//  ============================================================================
std::vector<Entity> getEntitiesInRange(
	const std::vector<Entity>& entities,
	PhysicsSystem& physicsSystem,
	glm::vec2 position,
	float distance) {
	std::vector<Entity> inRange;

	const float distanceSquared = distance * distance;

	for (Entity entity : entities) {
		if (physicsSystem.hasComponent(entity)) {
			PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);

			glm::vec2 entityPosition = physicsSystem.getPosition(physicsCmpnt);

			if (glm::distance2(position, entityPosition) <= distanceSquared) {
				inRange.push_back(entity);
			}
		}
	}

	return inRange;
}

//  ============================================================================
std::optional<Entity> getRandomLootInRange(World& world, const glm::vec2& position,
							float range) {
	std::vector<Entity> loot = world.getLootSystem().getEntities();
	std::vector<Entity> inRange =
		getEntitiesInRange(loot, world.getPhysicsSystem(), position, range);
	return world.getRandom().getRandomOptionalElement(inRange);
}

//	============================================================================
BlobAi::BlobAi(Entity entity) : AiBehavior(entity) {
}

//	============================================================================
void BlobAi::think(World& world, [[maybe_unused]] float elapsedSeconds) {
	const Entity entity = this->getEntity();

	if (!actorCanMove(entity, world)) {
		return;
	}

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

	InventorySystem& inventorySystem = world.getInventorySystem();
	InventoryComponent inventoryCmpnt = inventorySystem.getComponent(entity);

	//	Chase loot if inventory isn't full
	if (inventorySystem.getItemCount(inventoryCmpnt) < MAX_ITEM_CARRY) {
		if (!mTargetEntity.has_value() ||
			!world.getLootSystem().hasComponent(mTargetEntity.value())) {
			//	Assign a new target
			mTargetEntity = getRandomLootInRange(world, position, MIN_LOOT_RANGE);
		}

		if (mTargetEntity.has_value() &&
			world.getLootSystem().hasComponent(mTargetEntity.value())) {
			PhysicsComponent targetPhysicsCmpnt =
				physicsSystem.getComponent(mTargetEntity.value());
			glm::vec2 targetPosition = physicsSystem.getPosition(targetPhysicsCmpnt);

			//	Increase speed
			physicsSystem.setSpeed(physicsCmpnt, LOOT_CHASE_SPEED);

			//	Set move direction towards target
			AiSystem& aiSystem = world.getAiSystem();
			AiComponent aiCmpnt = aiSystem.getComponent(entity);
			aiSystem.setMoveDirection(aiCmpnt, targetPosition - position);

			//	Stop wandering
			aiSystem.setWander(aiCmpnt, false);

			//	Stop running AI
			return;
		}
	}

	//	Set speed to normal
	physicsSystem.setSpeed(physicsCmpnt, NORMAL_SPEED);

	//	Wander
	AiSystem& aiSystem = world.getAiSystem();
	AiComponent aiCmpnt = aiSystem.getComponent(entity);
	aiSystem.setWander(aiCmpnt, true);
}
}
