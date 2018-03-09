#pragma once

#include "ai/ai_node.hpp"
#include <string>

namespace ScaleMail
{
class FireAtTargetAiNode : public AiNode
{
	std::string mTargetValueName;

public:
	FireAtTargetAiNode(Entity& entity, AiTree& parentTree,
					   const std::string& targetValueName = "TARGET");
	virtual AiNodeStatus execute(World& world) override;
	void setTargetValueName(const std::string& name);
};
}