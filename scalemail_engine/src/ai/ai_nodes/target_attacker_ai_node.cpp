#include "ai/ai_nodes/target_attacker_ai_node.hpp"
#include "ai/ai_whiteboard.hpp"
#include "entity_util.hpp"
#include "damage_system.hpp"
#include "world.hpp"
#include <glm/vec2.hpp>
#include <algorithm>

namespace ScaleMail
{
//	============================================================================
TargetAttackerAiNode::TargetAttackerAiNode(Entity& entity, AiTree* parentTree,
const std::string& targetValueName)
: AiNode(entity, parentTree), mTargetValueName(targetValueName) {
}

//	============================================================================
AiNodeStatus TargetAttackerAiNode::execute(World& world) {
	Entity entity = this->getEntity();

	AiWhiteboard& wb = this->getWhiteboard();

	//	Check if targets already exists
	if (wb.getEntityCount(mTargetValueName) > 0) {
		return AiNodeStatus::SUCCESS;
	}

	//	Check if an entity damaged this entity
	DamageSystem& damageSystem = world.getDamageSystem();
	DamageComponent damageCmpnt = damageSystem.getComponent(entity);

	const auto& sourceEntities = damageSystem.getSourceEntities(damageCmpnt);

	if (sourceEntities.size() > 0) {
		Entity targetEntity = world.getRandom().getRandomElement(sourceEntities);

		wb.addEntity(mTargetValueName, targetEntity);

		return AiNodeStatus::SUCCESS;
	}

	return AiNodeStatus::FAILURE;
}

//	============================================================================
void TargetAttackerAiNode::setTargetValueName(const std::string& name) {
	mTargetValueName = name;
}
}