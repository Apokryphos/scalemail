#pragma once

#include "ai_whiteboard.hpp"
#include "entity.hpp"
#include <memory>

namespace ScaleMail
{
class AiNode;
class World;

class AiTree
{
	double mTotalElapsedSeconds;
	Entity mEntity;
	std::shared_ptr<AiNode> mRootNode;
	AiWhiteboard mWhiteboard;

public:
	AiTree(Entity entity, AiNode* rootNode = nullptr);
	AiTree(const AiTree&) = delete;
	AiTree& operator=(const AiTree&) = delete;
	void execute(World& world, double totalElapsedSeconds);
	double getElapsedSeconds(double lastTick) const;
	Entity getEntity() const;
	std::shared_ptr<AiNode> getRootNode();
	double getTotalElapsedSeconds() const;
	AiWhiteboard& getWhiteboard();
	void setRootNode(std::shared_ptr<AiNode> node);
};
}