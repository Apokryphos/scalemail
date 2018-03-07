#pragma once

#include "ai_whiteboard.hpp"
#include "entity.hpp"
#include <memory>

namespace ScaleMail
{
class AiNode;

class AiTree
{
	Entity mEntity;
	std::shared_ptr<AiNode> mRootNode;
	AiWhiteboard mWhiteboard;

public:
	AiTree(Entity entity, AiNode* rootNode = nullptr);
	Entity getEntity();
	AiNode* getRootNode();
	AiWhiteboard& getWhiteboard();
	void setRootNode(std::shared_ptr<AiNode> node);
};
}