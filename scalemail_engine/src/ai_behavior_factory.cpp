#include "ai_behavior_factory.hpp"
#include "string_util.hpp"
#include "ai/ally_ai.hpp"
#include "ai/blob_ai.hpp"
#include "ai/skeleton_ai.hpp"
#include "ai/skeleton_warrior_ai.hpp"
#include "ai/vampire_ai.hpp"

namespace ScaleMail
{
//  ============================================================================
std::shared_ptr<AiBehavior> AiBehaviorFactory::createAiBehavior(std::string name) {
	name = toLowercase(name);

	if (name == "ally") {
		return std::make_shared<AllyAi>();
	} else if (name == "blob") {
		return std::make_shared<BlobAi>();
	} else if (name == "skeleton") {
		return std::make_shared<SkeletonAi>();
	} else if (name == "skeleton_warrior") {
		return std::make_shared<SkeletonWarriorAi>();
	} else if (name == "vampire") {
		return std::make_shared<VampireAi>();
	}

	return nullptr;
}
}