#pragma once

#include "ai_behavior.hpp"
#include <glm/vec2.hpp>
#include <optional>

namespace ScaleMail
{
class BlobAi : public AiBehavior
{
	std::optional<Entity> mTargetEntity;
	glm::vec2 mMoveDirection;

public:
	BlobAi(Entity entity);
	virtual void think(World& world, float elapsedSeconds) override;
};
}