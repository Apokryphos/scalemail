#pragma once

#include "entity.hpp"
#include <memory>
#include <string>

namespace ScaleMail
{
class AiBehavior;

class AiBehaviorFactory
{
public:
	AiBehaviorFactory() = default;
	AiBehaviorFactory(const AiBehaviorFactory&) = delete;
	AiBehaviorFactory& operator=(const AiBehaviorFactory&) = delete;
	std::shared_ptr<AiBehavior> createAiBehavior(std::string name, Entity entity);
};
}