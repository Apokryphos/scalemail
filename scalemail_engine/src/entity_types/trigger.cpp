#include "trigger_system.hpp"
#include "world.hpp"

namespace ScaleMail
{
//  ============================================================================
Entity createTrigger(World& world, const glm::vec2& position,
					 const glm::vec2& size, const std::string& targetName) {
	Entity entity = world.createEntity();

	TriggerSystem& triggerSystem = world.getTriggerSystem();
	triggerSystem.addComponent(entity);
	TriggerComponent triggerCmpnt = triggerSystem.getComponent(entity);
	triggerSystem.setPosition(triggerCmpnt, position);
	triggerSystem.setSize(triggerCmpnt, size);
	triggerSystem.setTargetName(triggerCmpnt, targetName);

	return entity;
}
}