#pragma once

#include "sequence_ai_node.hpp"

namespace ScaleMail
{
class SelectorAiNode : public SequenceAiNode
{
protected:
	virtual AiNodeStatus onChildFailure() override;
	virtual AiNodeStatus onChildSuccess() override;

public:
	SelectorAiNode(Entity& entity, AiTree& parentTree);
};
}