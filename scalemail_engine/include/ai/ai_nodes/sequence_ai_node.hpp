#pragma once

#include "composite_ai_node.hpp"

namespace ScaleMail
{
class SequenceAiNode : public CompositeAiNode
{
	AiNode* mCurrentNode;

protected:
	void activateNextNode();
	AiNode* getCurrentNode();
	AiNode* getNextNode();
	virtual AiNodeStatus onChildFailure();
	virtual AiNodeStatus onChildSuccess();
	void setCurrentNode(AiNode* node);

public:
	SequenceAiNode(Entity& entity, AiTree* parentTree = nullptr);
	virtual AiNodeStatus execute(World& world) override;
};
}