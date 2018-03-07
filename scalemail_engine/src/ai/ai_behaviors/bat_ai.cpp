#include "ai/ai_behaviors/bat_ai.hpp"
#include "ai_system.hpp"
#include "actor_util.hpp"
#include "world.hpp"

namespace ScaleMail
{
//	============================================================================
BatAi::BatAi(Entity entity) : AiBehavior(entity) {
}

//	============================================================================
void BatAi::think(World& world, [[maybe_unused]] float elapsedSeconds) {
	const Entity entity = this->getEntity();

	if (!actorCanMove(entity, world)) {
		return;
	}

	//	Wander
	AiSystem& aiSystem = world.getAiSystem();
	AiComponent aiCmpnt = aiSystem.getComponent(entity);
	aiSystem.setWander(aiCmpnt, true);
}
}
