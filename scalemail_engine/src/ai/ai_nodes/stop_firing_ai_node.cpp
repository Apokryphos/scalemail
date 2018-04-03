#include "ai/ai_nodes/stop_firing_ai_node.hpp"
#include "gun_system.hpp"
#include "world.hpp"

namespace ScaleMail
{
//	============================================================================
StopFiringAiNode::StopFiringAiNode(Entity& entity, AiTree& parentTree)
: AiNode(entity, parentTree) {
}

//	============================================================================
AiNodeStatus StopFiringAiNode::execute(World& world) {
	Entity entity = this->getEntity();

	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);

	gunSystem.setFire(gunCmpnt, false);

	return AiNodeStatus::SUCCESS;
}
}