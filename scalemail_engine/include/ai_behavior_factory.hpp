#pragma once

#include <memory>
#include <string>

namespace ScaleMail
{
class AiBehavior;

class AiBehaviorFactory
{
public:
	std::shared_ptr<AiBehavior> createAiBehavior(std::string name);
};
}