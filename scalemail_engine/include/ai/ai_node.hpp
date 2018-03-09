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
	AiTree& mParentTree;
	Entity mEntity;

public:
	AiNode(Entity& entity, AiTree& parentTree);
	AiNode(const AiNode&) = delete;
	AiNode& operator=(const AiNode&) = delete;
	virtual ~AiNode() {};
	virtual AiNodeStatus execute(World& world) = 0;
	Entity getEntity();
	AiTree& getParentTree();
	AiWhiteboard& getWhiteboard();
};
}