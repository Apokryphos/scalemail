#include "ai_behavior_factory.hpp"
#include "string_util.hpp"
#include "ai/vampire_ai.hpp"

namespace ScaleMail
{
//  ============================================================================
std::shared_ptr<AiBehavior> AiBehaviorFactory::createAiBehavior(std::string name) {
	name = toLowercase(name);

	if (name == "vampire") {
		return std::make_shared<VampireAi>();
	}

	return nullptr;
}
}