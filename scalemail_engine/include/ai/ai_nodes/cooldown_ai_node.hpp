#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class World;

class CooldownAiNode : public AiNode
{
	float mDuration;
	float mTicks;

public:
	CooldownAiNode(Entity& entity, AiTree* parentTree = nullptr,
				   float duration = 0.0f);
	virtual AiNodeStatus execute(World& world, float elapsedSeconds) override;
	void setDuration(float duration);
};
}