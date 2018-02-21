#include "ai/ally_ai.hpp"
#include "entity_util.hpp"
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

	Random& random = world.getRandom();

	if (mMoveCooldown <= 0.0f) {

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
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);

	// PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

	std::vector<Entity> villains;
	world.getTeamSystem().getEntitiesByTeam(Team::VILLAIN, villains);

	std::vector<Entity> inRange;
	getEntitiesInRange(villains, physicsSystem, position, MIN_VILLAIN_RANGE,
					   inRange);

	const BurySystem& burySystem = world.getBurySystem();

	//	Ignore buried entities
	inRange.erase(
		std::remove_if(inRange.begin(), inRange.end(),
			[&burySystem](const Entity& e) {
				 return entityIsBuried(e, burySystem);
			}),
		inRange.end());

	if (inRange.size() > 0) {
		std::optional<Entity> villainEntity =
			random.getRandomOptionalElement(inRange);

		if (villainEntity.has_value()) {
			PhysicsComponent targetPhysicsCmpnt =
				physicsSystem.getComponent(villainEntity.value());

			glm::vec2 targetPosition =
				physicsSystem.getPosition(targetPhysicsCmpnt);

			//	Fire bullets at villain
			gunSystem.setTarget(gunCmpnt, targetPosition);
			gunSystem.setFire(gunCmpnt, true);
		}
	}
}
}
