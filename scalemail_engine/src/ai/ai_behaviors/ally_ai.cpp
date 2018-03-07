#include "ai/ai_behaviors/ally_ai.hpp"
#include "ai/ai_nodes/cooldown_ai_node.hpp"
#include "ai/ai_nodes/random_move_direction_ai_node.hpp"
#include "ai/ai_nodes/sequence_ai_node.hpp"
#include "entity_util.hpp"
#include "gun_system.hpp"
#include "math_util.hpp"
#include "physics_system.hpp"
#include "player.hpp"
#include "player_util.hpp"
#include "world.hpp"
#include <algorithm>
#include <memory>
#include <vector>

namespace ScaleMail
{
const float MOVE_DIRECTION_CHANGE_INTERVAL = 2.5f;
const float MIN_VILLAIN_RANGE = 128.0f;

//	============================================================================
AllyAi::AllyAi(Entity entity) : AiBehavior(entity), mAiTree(entity) {
	std::shared_ptr<CooldownAiNode> cooldown =
		std::make_shared<CooldownAiNode>(entity, &mAiTree);

	std::shared_ptr<RandomMoveDirectionAiNode> randomMoveDirection =
		std::make_shared<RandomMoveDirectionAiNode>(entity, &mAiTree);

	std::shared_ptr<SequenceAiNode> randomMove =
		std::make_shared<SequenceAiNode>(entity, &mAiTree);

	mAiTree.setRootNode(randomMove);
	randomMove->addChildNode(cooldown);
	randomMove->addChildNode(randomMoveDirection);

	cooldown->setDuration(MOVE_DIRECTION_CHANGE_INTERVAL);
}

//	============================================================================
void AllyAi::think(World& world, float elapsedSeconds) {
	const Entity entity = this->getEntity();

	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);

	gunSystem.setFire(gunCmpnt, false);

	mAiTree.getRootNode()->execute(world, elapsedSeconds);

	Random& random = world.getRandom();

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
