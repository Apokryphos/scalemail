#include "ai/ai_behaviors/ally_ai.hpp"
#include "ai/ai_behaviors/bat_ai.hpp"
#include "ai/ai_behaviors/blob_ai.hpp"
#include "ai/ai_behaviors/skeleton_ai.hpp"
#include "ai/ai_behaviors/skeleton_warrior_ai.hpp"
#include "ai/ai_behaviors/vampire_ai.hpp"
#include "ai/ai_behavior_factory.hpp"
#include "string_util.hpp"

namespace ScaleMail
{
//  ============================================================================
std::shared_ptr<AiBehavior> AiBehaviorFactory::createAiBehavior(
	std::string name, Entity entity) {
	name = toLowercase(name);

	if (name == "ally") {
		return std::make_shared<AllyAi>(entity);
	} else if (name == "bat") {
		return std::make_shared<BatAi>(entity);
	} else if (name == "blob") {
		return std::make_shared<BlobAi>(entity);
	} else if (name == "skeleton") {
		return std::make_shared<SkeletonAi>(entity);
	} else if (name == "skeleton_warrior") {
		return std::make_shared<SkeletonWarriorAi>(entity);
	} else if (name == "vampire") {
		return std::make_shared<VampireAi>(entity);
	}

	return nullptr;
}
}