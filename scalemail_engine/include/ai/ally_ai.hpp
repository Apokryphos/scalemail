#pragma once

#include "ai_behavior.hpp"
#include <glm/vec2.hpp>
#include <optional>

namespace ScaleMail
{
class AllyAi : public AiBehavior
{
	float mMoveCooldown;
	float mMoveCooldownDuration;

public:
	AllyAi();
	virtual void think(World& world, float elapsedSeconds) override;
};
}