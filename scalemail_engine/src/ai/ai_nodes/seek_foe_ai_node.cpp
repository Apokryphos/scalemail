#include "ai/ai_nodes/seek_foe_ai_node.hpp"
#include "actor_util.hpp"
#include "entity_util.hpp"
#include "math_util.hpp"
#include "physics_system.hpp"
#include "world.hpp"
#include <glm/vec2.hpp>
#include <algorithm>

namespace ScaleMail
{
//	============================================================================
SeekFoeAiNode::SeekFoeAiNode(Entity& entity, AiTree* parentTree, float range)
: AiNode(entity, parentTree), mRange(range) {
}

//	============================================================================
AiNodeStatus SeekFoeAiNode::execute(World& world,
									[[maybe_unused]]float elapsedSeconds) {
	Entity entity = this->getEntity();

	if (!actorCanMove(entity, world)) {
		return AiNodeStatus::FAILURE;
	}

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();

	if (!actorIsAlive(mTargetEntity, world)) {
		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
		glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

		TeamSystem& teamSystem = world.getTeamSystem();
		TeamComponent teamCmpnt = teamSystem.getComponent(entity);

		std::vector<Entity> foes;
		world.getTeamSystem().getFoes(teamCmpnt, foes);

		std::vector<Entity> inRange;
		getEntitiesInRange(foes, physicsSystem, position, mRange, inRange);

		if (inRange.size() > 0) {
			Random& random = world.getRandom();

			std::optional<Entity> foeEntity =
				random.getRandomOptionalElement(inRange);

			if (foeEntity.has_value()) {
				mTargetEntity = foeEntity.value();
				return AiNodeStatus::RUNNING;
			}
		}
	} else {
		PhysicsComponent targetPhysicsCmpnt =
			physicsSystem.getComponent(mTargetEntity);

		glm::vec2 targetPosition = physicsSystem.getPosition(targetPhysicsCmpnt);

		AiSystem& aiSystem = world.getAiSystem();
		AiComponent aiCmpnt = aiSystem.getComponent(entity);

		//	Set move direction towards target
		aiSystem.setSeek(aiCmpnt, true);
		aiSystem.setSeekTarget(aiCmpnt, targetPosition);

		return AiNodeStatus::SUCCESS;
	}

	return AiNodeStatus::FAILURE;
}

//	============================================================================
void SeekFoeAiNode::setRange(float range) {
	mRange = std::max(range, 0.0f);
}
}