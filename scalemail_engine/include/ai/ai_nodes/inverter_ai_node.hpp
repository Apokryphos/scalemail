#pragma once

#include "decorator_ai_node.hpp"

namespace ScaleMail
{
class World;

class InverterAiNode : public DecoratorAiNode
{
	AiNode& mChildNode;

public:
	InverterAiNode(Entity& entity) : DecoratorAiNode(entity) {
	}

	virtual AiNodeStatus execute(World& world, float elapsedSeconds) override {
		if (!this->hasChildNode()) {
			return AiNodeStatus::FAILURE;
		}

		AiNodeStatus status =
			this->getChildNode()->execute(world, elapsedSeconds);

		switch (status) {
			case AiNodeStatus::FAILURE:
				return AiNodeStatus::SUCCESS;
			case AiNodeStatus::SUCCESS:
				return AiNodeStatus::FAILURE;
			case AiNodeStatus::RUNNING:
				return AiNodeStatus::RUNNING;
		}
	}
};
}