#pragma once

#include "ai/ai_tree.hpp"
#include "ai/ai_behavior.hpp"
#include <glm/vec2.hpp>
#include <optional>

namespace ScaleMail
{
class BlobAi : public AiBehavior
{
	std::optional<Entity> mTargetEntity;
	glm::vec2 mMoveDirection;
	AiTree mAiTree;

public:
	BlobAi(Entity entity);
	virtual void think(World& world, double totalElapsedSeconds) override;
};
}