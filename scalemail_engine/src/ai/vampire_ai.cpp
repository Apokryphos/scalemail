#include "ai/vampire_ai.hpp"
#include "gun_system.hpp"
#include "physics_system.hpp"
#include "player.hpp"
#include "player_util.hpp"
#include "world.hpp"
#include <algorithm>
#include <vector>

namespace ScaleMail
{
//	============================================================================
VampireAi::VampireAi() {
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
		Random& random = world.getRandom();

		const float minPlayerRange = 128.0f;

		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
		glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

		std::vector<Player*> allPlayers = world.getPlayers();

		std::vector<Player*> players =
			getPlayersInRange(allPlayers, physicsSystem, position, minPlayerRange);

		Player* player = random.getRandomElement(players);

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
		//	Transform into bats

		//	Reset cooldown
		mTransformCooldown = mTransformCooldownDuration;
	}
}
}
