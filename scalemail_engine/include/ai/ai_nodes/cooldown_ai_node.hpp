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
	CooldownAiNode(Entity& entity, AiTree* parentTree = nullptr, float duration = 0.0f)
	: AiNode(entity, parentTree), mDuration(duration), mTicks(0) {
	}

	virtual AiNodeStatus execute(World& world, float elapsedSeconds) override {
		mTicks -= elapsedSeconds;
		if (mTicks <= 0.0f) {
			mTicks = mDuration;
			return AiNodeStatus::SUCCESS;
		}

		return AiNodeStatus::FAILURE;
	}

	void setDuration(float duration) {
		mDuration = duration;
	}
};
}