#include "ai/ai_behaviors/bat_ai.hpp"
#include "ai/ai_nodes/wander_ai_node.hpp"
#include "ai_system.hpp"
#include "actor_util.hpp"
#include "world.hpp"

namespace ScaleMail
{
//	============================================================================
BatAi::BatAi(Entity entity) : AiBehavior(entity), mAiTree(entity) {
	auto wander = std::make_shared<WanderAiNode>(entity, mAiTree);
	mAiTree.setRootNode(wander);
}

//	============================================================================
void BatAi::think(World& world, double totalElapsedSeconds) {
	mAiTree.execute(world, totalElapsedSeconds);
}
}