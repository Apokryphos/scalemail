#include "ai/ai_nodes/fire_at_foes_ai_node.hpp"
#include "entity_util.hpp"
#include "gun_system.hpp"
#include "math_util.hpp"
#include "physics_system.hpp"
#include "world.hpp"
#include <glm/vec2.hpp>
#include <algorithm>

namespace ScaleMail
{
//	============================================================================
FireAtFoesAiNode::FireAtFoesAiNode(Entity& entity, AiTree* parentTree, float range)
: AiNode(entity, parentTree), mRange(range) {
}

//	============================================================================
AiNodeStatus FireAtFoesAiNode::execute(World& world,
									   [[maybe_unused]]float elapsedSeconds) {
	Entity entity = this->getEntity();

	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);

	Random& random = world.getRandom();

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);

	glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

	TeamSystem& teamSystem = world.getTeamSystem();
	TeamComponent teamCmpnt = teamSystem.getComponent(entity);

	std::vector<Entity> foes;
	world.getTeamSystem().getFoes(teamCmpnt, foes);

	std::vector<Entity> inRange;
	getEntitiesInRange(foes, physicsSystem, position, mRange,
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
		std::optional<Entity> foeEntity =
			random.getRandomOptionalElement(inRange);

		if (foeEntity.has_value()) {
			PhysicsComponent targetPhysicsCmpnt =
				physicsSystem.getComponent(foeEntity.value());

			glm::vec2 targetPosition =
				physicsSystem.getPosition(targetPhysicsCmpnt);

			//	Fire bullets at foe
			gunSystem.setTarget(gunCmpnt, targetPosition);
			gunSystem.setFire(gunCmpnt, true);

			return AiNodeStatus::SUCCESS;
		}
	}

	return AiNodeStatus::FAILURE;
}

//	============================================================================
void FireAtFoesAiNode::setRange(float range) {
	mRange = std::max(range, 0.0f);
}
}