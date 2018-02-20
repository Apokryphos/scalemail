#include "ai/blob_ai.hpp"
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
const float MIN_LOOT_RANGE = 128.0f;
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
BlobAi::BlobAi() {
}

//	============================================================================
void BlobAi::think(World& world, float elapsedSeconds) {
	const Entity entity = this->getEntity();

	if (!actorCanMove(entity, world)) {
		return;
	}

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

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
	} else {
		Random& random = world.getRandom();

		//	Set speed to normal
		physicsSystem.setSpeed(physicsCmpnt, NORMAL_SPEED);

		//	Pick random direction
		glm::vec2 direction =
			rotateVec2(glm::vec2(1.0f, 0.0f), random.nextFloat(0.0f, TWO_PI));

		//	Set move direction
		AiSystem& aiSystem = world.getAiSystem();
		AiComponent aiCmpnt = aiSystem.getComponent(entity);
		aiSystem.setMoveDirection(aiCmpnt, direction);
	}
}
}
