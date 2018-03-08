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
	Entity mEntity;
	std::shared_ptr<AiNode> mRootNode;
	AiWhiteboard mWhiteboard;

public:
	AiTree(Entity entity, AiNode* rootNode = nullptr);
	void execute(World& world, float elapsedSeconds);
	Entity getEntity();
	AiNode* getRootNode();
	AiWhiteboard& getWhiteboard();
	void setRootNode(std::shared_ptr<AiNode> node);
};
}