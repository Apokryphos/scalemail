#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class SeekFoeAiNode : public AiNode
{
	float mRange;
	Entity mTargetEntity;

public:
	SeekFoeAiNode(Entity& entity, AiTree* parentTree = nullptr,
				  float range = 64.0f);
	virtual AiNodeStatus execute(World& world, float elapsedSeconds) override;
	void setRange(float range);
};
}