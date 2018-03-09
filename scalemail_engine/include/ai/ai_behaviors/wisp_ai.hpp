#pragma once

#include "ai/ai_tree.hpp"
#include "ai/ai_behavior.hpp"
#include <glm/vec2.hpp>
#include <optional>

namespace ScaleMail
{
class WispAi : public AiBehavior
{
	AiTree mAiTree;

public:
	WispAi(Entity entity);
	virtual void think(World& world, double totalElapsedSeconds) override;
};
}