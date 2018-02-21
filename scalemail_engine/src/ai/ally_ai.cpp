#include "ai/ally_ai.hpp"
#include "gun_system.hpp"
#include "math_util.hpp"
#include "physics_system.hpp"
#include "player.hpp"
#include "player_util.hpp"
#include "world.hpp"
#include <algorithm>
#include <vector>

namespace ScaleMail
{
const float MIN_VILLAIN_RANGE = 128.0f;

//	============================================================================
AllyAi::AllyAi() {
	mMoveCooldown = 0.0f;
	mMoveCooldownDuration = 3.0f;
}

//	============================================================================
void AllyAi::think(World& world, float elapsedSeconds) {
	const Entity entity = this->getEntity();

	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);

	gunSystem.setFire(gunCmpnt, false);

	mMoveCooldown -= elapsedSeconds;

	if (mMoveCooldown <= 0.0f) {
		Random& random = world.getRandom();

		//	Pick random direction
		glm::vec2 direction =
			rotateVec2(glm::vec2(1.0f, 0.0f), random.nextFloat(0.0f, TWO_PI));

		AiSystem& aiSystem = world.getAiSystem();
		AiComponent aiCmpnt = aiSystem.getComponent(entity);
		aiSystem.setMoveDirection(aiCmpnt, direction);

		//	Reset cooldown
		mMoveCooldown = mMoveCooldownDuration;
	}

	//	Fire at the nearest villain
	// PhysicsSystem& physicsSystem = world.getPhysicsSystem();

	// PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	// glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

	// std::optional<Entity> villain =
	// 	getRandomVillainInRange(world, position, MIN_VILLAIN_RANGE);

	// if (villain.has_value()) {
	// 	PhysicsComponent targetPhysicsCmpnt =
	// 		physicsSystem.getComponent(player->entity);

	// 	glm::vec2 targetPosition =
	// 		physicsSystem.getPosition(targetPhysicsCmpnt);

	// 	//	Fire bullets at villain
	// 	gunSystem.setTarget(gunCmpnt, targetPosition);
	// 	gunSystem.setFire(gunCmpnt, true);
	// }
}
}
