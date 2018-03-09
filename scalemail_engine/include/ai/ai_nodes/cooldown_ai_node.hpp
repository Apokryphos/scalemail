#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class World;

class CooldownAiNode : public AiNode
{
	double mDurationMin;
	double mDurationMax;
	double mTicks;
	double mLastTicks;

public:
	CooldownAiNode(Entity& entity, AiTree& parentTree, double duration = 0);
	virtual AiNodeStatus execute(World& world) override;
	void setDuration(double duration);
	void setRandomDuration(double min, double max);
};
}