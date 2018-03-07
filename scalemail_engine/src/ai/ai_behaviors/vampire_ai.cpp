#include "ai/ai_behaviors/vampire_ai.hpp"
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
const float MIN_PLAYER_RANGE = 128.0f;

//	============================================================================
VampireAi::VampireAi(Entity entity) : AiBehavior(entity) {
	mTransformCooldown = 0.0f;
	mTransformCooldownDuration = 3.0f;
}

//	============================================================================
void VampireAi::think(World& world, float elapsedSeconds) {
	const Entity entity = this->getEntity();

	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);

	gunSystem.setFire(gunCmpnt, false);

	mTransformCooldown -= elapsedSeconds;

	if (mTransformCooldown > 0.0f) {
		//	Fire at the nearest player
		PhysicsSystem& physicsSystem = world.getPhysicsSystem();

		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
		glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

		Player* player =
			getRandomPlayerInRange(world, position, MIN_PLAYER_RANGE);

		if (player != nullptr) {
			PhysicsComponent targetPhysicsCmpnt =
				physicsSystem.getComponent(player->entity);

			glm::vec2 targetPosition =
				physicsSystem.getPosition(targetPhysicsCmpnt);

			//	Fire bullets at player
			gunSystem.setTarget(gunCmpnt, targetPosition);
			gunSystem.setFire(gunCmpnt, true);
		}
	} else {
		Random& random = world.getRandom();

		// Pick random direction
		glm::vec2 direction =
			rotateVec2(glm::vec2(1.0f, 0.0f), random.nextFloat(0.0f, TWO_PI));

		AiSystem& aiSystem = world.getAiSystem();
		AiComponent aiCmpnt = aiSystem.getComponent(entity);

		aiSystem.setWander(aiCmpnt, true);

		aiSystem.setMoveDirection(aiCmpnt, direction);

		//	Transform into bats

		//	Reset cooldown
		mTransformCooldown = mTransformCooldownDuration;
	}
}
}
