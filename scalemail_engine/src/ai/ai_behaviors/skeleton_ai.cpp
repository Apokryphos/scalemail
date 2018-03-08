#include "ai/ai_behaviors/skeleton_ai.hpp"
#include "ai/ai_nodes/seek_foe_ai_node.hpp"
#include "ai_system.hpp"
#include "world.hpp"

namespace ScaleMail
{
const float MIN_PLAYER_RANGE = 128.0f;

//	============================================================================
SkeletonAi::SkeletonAi(Entity entity) : AiBehavior(entity), mAiTree(entity) {
	//	==================================================
	//	Chase foes in range
	//	==================================================
	std::shared_ptr<SeekFoeAiNode> seekFoe =
		std::make_shared<SeekFoeAiNode>(entity, &mAiTree);

	seekFoe->setRange(MIN_PLAYER_RANGE);

	mAiTree.setRootNode(seekFoe);
}

//	============================================================================
void SkeletonAi::think(World& world, [[maybe_unused]]float elapsedSeconds) {
	const Entity entity = this->getEntity();

	AiSystem& aiSystem = world.getAiSystem();
	AiComponent aiCmpnt = aiSystem.getComponent(entity);
	aiSystem.setSeek(aiCmpnt, false);

	mAiTree.getRootNode()->execute(world, elapsedSeconds);
}
}
