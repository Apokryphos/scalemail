#pragma once

#include "ai_node_status.hpp"
#include "entity.hpp"

namespace ScaleMail
{
class AiTree;
class AiWhiteboard;
class World;

class AiNode
{
	AiTree* mParentTree;
	Entity mEntity;

public:
	AiNode(Entity& entity, AiTree* parentTree = nullptr);
	virtual ~AiNode() {};
	virtual AiNodeStatus execute(World& world) = 0;
	Entity getEntity();
	AiTree* getParentTree();
	void setParentTree(AiTree* parentTree);
	AiWhiteboard& getWhiteboard();
};
}