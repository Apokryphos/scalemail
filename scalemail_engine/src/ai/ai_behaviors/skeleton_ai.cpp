#include "ai/ai_behaviors/skeleton_ai.hpp"
#include "ai_system.hpp"
#include "actor_util.hpp"
#include "physics_system.hpp"
#include "player.hpp"
#include "player_util.hpp"
#include "world.hpp"
#include <algorithm>
#include <vector>

namespace ScaleMail
{
const float MIN_PLAYER_RANGE = 128.0f;

//	============================================================================
SkeletonAi::SkeletonAi(Entity entity) : AiBehavior(entity) {
}

//	============================================================================
void SkeletonAi::think(World& world, [[maybe_unused]]float elapsedSeconds) {
	const Entity entity = this->getEntity();

	if (!actorCanMove(entity, world)) {
		return;
	}

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

	AiSystem& aiSystem = world.getAiSystem();
	AiComponent aiCmpnt = aiSystem.getComponent(entity);

	if (!actorIsAlive(mTargetEntity, world)) {
		//	Assign a new target
		Player* player = getRandomPlayerInRange(world, position, MIN_PLAYER_RANGE);
		if (player != nullptr) {
			mTargetEntity = player->entity;
		}

		aiSystem.setSeek(aiCmpnt, false);
	} else {
		PhysicsComponent targetPhysicsCmpnt =
			physicsSystem.getComponent(mTargetEntity);
		glm::vec2 targetPosition = physicsSystem.getPosition(targetPhysicsCmpnt);

		//	Set move direction towards target
		aiSystem.setSeek(aiCmpnt, true);
		aiSystem.setSeekTarget(aiCmpnt, targetPosition);
	}
}
}