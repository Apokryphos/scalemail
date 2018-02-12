#pragma once

#include "ai_behavior.hpp"

namespace ScaleMail
{
class VampireAi : public AiBehavior
{
	float mTransformCooldown;
	float mTransformCooldownDuration;

public:
	VampireAi();
	virtual void think(World& world, float elapsedSeconds) override;
};
}