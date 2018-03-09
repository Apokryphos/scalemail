#include "ai/ai_nodes/wander_ai_node.hpp"
#include "actor_util.hpp"
#include "ai_system.hpp"
#include "world.hpp"

namespace ScaleMail
{
//	============================================================================
WanderAiNode::WanderAiNode(Entity& entity, AiTree& parentTree)
: AiNode(entity, parentTree) {
}

//	============================================================================
AiNodeStatus WanderAiNode::execute(World& world) {
	const Entity entity = this->getEntity();

	if (!actorCanMove(entity, world)) {
		return AiNodeStatus::FAILURE;
	}

	//	Wander
	AiSystem& aiSystem = world.getAiSystem();
	AiComponent aiCmpnt = aiSystem.getComponent(entity);
	aiSystem.setWander(aiCmpnt, true);

	return AiNodeStatus::FAILURE;
}
}