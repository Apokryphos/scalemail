#pragma once

#include "ai/ai_node.hpp"
#include <string>

namespace ScaleMail
{
class TargetAttackerAiNode : public AiNode
{
	std::string mTargetValueName;

public:
	TargetAttackerAiNode(Entity& entity, AiTree& parentTree,
					 const std::string& targetValueName = "TARGET");
	virtual AiNodeStatus execute(World& world) override;
	void setTargetValueName(const std::string& name);
};
}