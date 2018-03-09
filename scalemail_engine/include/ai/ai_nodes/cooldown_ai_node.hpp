#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class World;

class CooldownAiNode : public AiNode
{
	double mDuration;
	double mTicks;
	double mLastTicks;

public:
	CooldownAiNode(Entity& entity, AiTree& parentTree,
				   float duration = 0.0f);
	virtual AiNodeStatus execute(World& world) override;
	void setDuration(float duration);
};
}