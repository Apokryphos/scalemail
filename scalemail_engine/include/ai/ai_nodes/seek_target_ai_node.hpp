#pragma once

#include "ai/ai_node.hpp"

namespace ScaleMail
{
class SeekTargetAiNode : public AiNode
{
	std::string mTargetValueName;

public:
	SeekTargetAiNode(Entity& entity, AiTree* parentTree = nullptr,
					 const std::string& targetValueName = "TARGET");
	virtual AiNodeStatus execute(World& world, float elapsedSeconds) override;
	void setTargetValueName(const std::string& name);
};
}