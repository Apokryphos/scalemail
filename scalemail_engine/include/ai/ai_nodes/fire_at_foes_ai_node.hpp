#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class FireAtFoesAiNode : public AiNode
{
	float mRange;

public:
	FireAtFoesAiNode(Entity& entity, AiTree* parentTree = nullptr,
					 float range = 64.0f);
	virtual AiNodeStatus execute(World& world, float elapsedSeconds) override;
	void setRange(float range);
};
}