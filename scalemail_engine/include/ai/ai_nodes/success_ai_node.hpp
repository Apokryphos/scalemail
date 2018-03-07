#pragma once

#include "ai_node.hpp"

namespace ScaleMail
{
class World;

class SuccessAiNode : public AiNode
{
public:
	SuccessAiNode(Entity& entity) : AiNode(entity) {
	}

	virtual AiNodeStatus execute(World& world, float elapsedSeconds) override {
		return AiNodeStatus::SUCCESS;
	}
};
}