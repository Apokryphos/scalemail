#pragma once

#include "ai_node.hpp"

namespace ScaleMail
{
class World;

class FailureAiNode : public AiNode
{
public:
	FailureAiNode(Entity& entity) : AiNode(entity) {
	}

	virtual AiNodeStatus execute(World& world, float elapsedSeconds) override {
		return AiNodeStatus::FAILURE;
	}
};
}