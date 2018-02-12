#pragma once

#include "ai_behavior.hpp"
#include <memory>

namespace ScaleMail
{
class AiBehaviorFactory
{
public:
	std::shared_ptr<AiBehavior> createAiBehavior(std::string name);
};
}