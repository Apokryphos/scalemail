#include "ai/ai_nodes/target_range_ai_node.hpp"
#include "ai/ai_whiteboard.hpp"
#include "entity_util.hpp"
#include "world.hpp"
#include <glm/vec2.hpp>
#include <algorithm>

namespace ScaleMail
{
//	============================================================================
TargetRangeAiNode::TargetRangeAiNode(Entity& entity, AiTree& parentTree,
									 float range,
									 TeamAlignment targetTeamAlignment,
									 const std::string& targetValueName)
: AiNode(entity, parentTree), mTargetType(TargetType::ACTOR),
  mTargetTeamAlignment(targetTeamAlignment), mRange(range),
  mTargetValueName(targetValueName) {
}

//	============================================================================
AiNodeStatus TargetRangeAiNode::execute(World& world) {
	Entity entity = this->getEntity();

	AiWhiteboard& wb = this->getWhiteboard();

	//	Check if targets already exist
	if (wb.getEntityCount(mTargetValueName) > 0) {
		return AiNodeStatus::SUCCESS;
	}

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();

	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

	TeamSystem& teamSystem = world.getTeamSystem();
	TeamComponent teamCmpnt = teamSystem.getComponent(entity);

	std::vector<Entity> entities;

	switch (mTargetType) {
		case TargetType::ACTOR:
			world.getTeamSystem().getEntitiesByTeamAlignment(
				teamCmpnt, mTargetTeamAlignment, entities);
				break;

		case TargetType::LOOT:
			world.getLootSystem().getEntities(entities);
				break;

		default:
			throw std::runtime_error("Case not implemented.");
	}

	std::vector<Entity> inRange;
	getEntitiesInRange(entities, physicsSystem, position, mRange, inRange);

	if (inRange.size() > 0) {
		std::optional<Entity> target =
			world.getRandom().getRandomOptionalElement(inRange);

		wb.addEntity(mTargetValueName, target.value());

		return AiNodeStatus::SUCCESS;
	}

	return AiNodeStatus::FAILURE;
}

//	============================================================================
void TargetRangeAiNode::setRange(float range) {
	mRange = std::max(0.0f, range);
}

//	============================================================================
void TargetRangeAiNode::setTargetTeamAlignment(const TeamAlignment teamAlignment) {
	mTargetTeamAlignment = teamAlignment;
}

//	============================================================================
void TargetRangeAiNode::setTargetType(const TargetType targetType) {
	mTargetType = targetType;
}

//	============================================================================
void TargetRangeAiNode::setTargetValueName(const std::string& name) {
	mTargetValueName = name;
}
}