#include "ai/ai_nodes/fire_at_target_ai_node.hpp"
#include "ai/ai_whiteboard.hpp"
#include "actor_util.hpp"
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
FireAtTargetAiNode::FireAtTargetAiNode(Entity& entity, AiTree* parentTree,
const std::string& targetValueName)
: AiNode(entity, parentTree), mTargetValueName(targetValueName) {
}

//	============================================================================
AiNodeStatus FireAtTargetAiNode::execute(World& world,
										 [[maybe_unused]]float elapsedSeconds) {
	Entity entity = this->getEntity();

	if (!actorCanFire(entity, world)) {
		return AiNodeStatus::FAILURE;
	}

	AiWhiteboard& wb = this->getWhiteboard();

	const std::vector<Entity>& entities = wb.getEntities(mTargetValueName);

	std::vector<Entity> targets(entities);

	const BurySystem& burySystem = world.getBurySystem();

	//	Ignore buried entities
	targets.erase(
		std::remove_if(targets.begin(), targets.end(),
			[&burySystem](const Entity& e) {
				 return entityIsBuried(e, burySystem);
			}),
		targets.end());

	if (targets.size() > 0) {
		Random& random = world.getRandom();

		std::optional<Entity> target =
			random.getRandomOptionalElement(targets);

		if (target.has_value()) {
			PhysicsSystem& physicsSystem = world.getPhysicsSystem();

			PhysicsComponent targetPhysicsCmpnt =
				physicsSystem.getComponent(target.value());

			glm::vec2 targetPosition =
				physicsSystem.getPosition(targetPhysicsCmpnt);

			GunSystem& gunSystem = world.getGunSystem();
			GunComponent gunCmpnt = gunSystem.getComponent(entity);

			//	Fire bullets at foe
			gunSystem.setTarget(gunCmpnt, targetPosition);
			gunSystem.setFire(gunCmpnt, true);

			return AiNodeStatus::SUCCESS;
		}
	}

	return AiNodeStatus::FAILURE;
}

//	============================================================================
void FireAtTargetAiNode::setTargetValueName(const std::string& name) {
	mTargetValueName = name;
}
}