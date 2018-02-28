#pragma once

#include "ai_behavior.hpp"
#include <glm/vec2.hpp>
#include <optional>

namespace ScaleMail
{
class BatAi : public AiBehavior
{
	std::optional<Entity> mTargetEntity;
	glm::vec2 mMoveDirection;

public:
	BatAi();
	virtual void think(World& world, float elapsedSeconds) override;
};
}