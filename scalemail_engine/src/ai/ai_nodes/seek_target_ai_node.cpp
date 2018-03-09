#include "ai/ai_nodes/seek_target_ai_node.hpp"
#include "ai/ai_whiteboard.hpp"
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
SeekTargetAiNode::SeekTargetAiNode(Entity& entity, AiTree& parentTree,
								   const std::string& targetValueName)
: AiNode(entity, parentTree), mTargetValueName(targetValueName) {
}

//	============================================================================
AiNodeStatus SeekTargetAiNode::execute(World& world) {
	Entity entity = this->getEntity();

	if (!actorCanMove(entity, world)) {
		return AiNodeStatus::FAILURE;
	}

	AiWhiteboard& wb = this->getWhiteboard();

	const std::vector<Entity>& targets = wb.getEntities(mTargetValueName);

	AiSystem& aiSystem = world.getAiSystem();
	AiComponent aiCmpnt = aiSystem.getComponent(entity);

	if (targets.size() > 0) {
		Random& random = world.getRandom();

		std::optional<Entity> target =
			random.getRandomOptionalElement(targets);

		if (target.has_value()) {
			PhysicsSystem& physicsSystem = world.getPhysicsSystem();

			PhysicsComponent targetPhysicsCmpnt =
				physicsSystem.getComponent(target.value());

			glm::vec2 targetPosition = physicsSystem.getPosition(targetPhysicsCmpnt);

			//	Set move direction towards target
			aiSystem.setSeek(aiCmpnt, true);
			aiSystem.setSeekTarget(aiCmpnt, targetPosition);

			return AiNodeStatus::SUCCESS;
		}
	}

	//	Stop seeking
	aiSystem.setSeek(aiCmpnt, false);

	return AiNodeStatus::FAILURE;
}

//	============================================================================
void SeekTargetAiNode::setTargetValueName(const std::string& name) {
	mTargetValueName = name;
}
}